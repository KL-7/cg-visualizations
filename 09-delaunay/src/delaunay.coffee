class Vertex
  constructor: (@x, @y) -> @edges = new Set
  addEdge: (edge) -> @edges.add(edge)
  removeEdge: (edge) -> @edges.remove(edge)
  adjacent: -> _.map(@edges.toArray(), (edge) => edge.otherVertex(this))
  atan2: (vertex) -> Math.atan2(@y - vertex.y, @x - vertex.x)
  toArray: -> [@x, @y]
  toString: -> 'Vertex(' + @x + ', ' + @y + ')'
  @compare: (v1, v2) ->
    if (v1.x == v2.x && v1.y == v2.y) then 0 else (if v1.x < v2.x || v1.x == v2.x && v1.y < v2.y then -1 else 1)


class Edge
  constructor: (@v1, @v2) -> [@v1, @v2] = [@v2, @v1] if @v2.x < @v1.x
  vertices: -> [@v1, @v2]
  incident: (v) -> @v1 == v || @v2 == v
  connect: -> _.each(this.vertices(), (v) => v.addEdge(this)); this
  disconnect: -> _.each(this.vertices(), (v) => v.removeEdge(this)); this
  otherVertex: (v) -> (if v == @v1 then @v2 else @v1) if this.incident(v)
  toArray: -> [@v1.toArray(), @v2.toArray()]
  toString: -> if @_str? then @_str else @_str = 'Edge(' + this.vertices().join(', ') + ')'


class Set
  constructor: (array) ->
    @set = {}
    this.addAll(array) if array
  addAll: (array) -> _.each(array, (e) => this.add(e))
  add: (e) -> @set[e] = e
  get: (e) -> @set[e]
  include: (e) -> @set.hasOwnProperty(e)
  remove: (e) -> delete @set[e]
  toArray: -> v for own _, v of @set
  toString: -> 'Set(' + this.toArray().join(',') + ')'
  extend: (s) -> this.addAll(s.toArray()); this


class Triangle
  constructor: (v1, v2, v3) -> [@v1, @v2, @v3] = [v1, v2, v3].sort Vertex.compare
  vertices: -> [@v1, @v2, @v3]
  toString: -> 'Triangle(' + this.vertices().join(', ') + ')'
  toArray: -> _.map(this.vertices(), (v) -> v.toArray())


class window.Delaunay
  constructor: (@points) ->

  build: ->
    [edges, vertices] = this.divideAndConquer(this.sortedVertices(@points))
    triangles = new Set
    _.each vertices, (vertex) ->
      incidentEdges = _.sortBy(vertex.edges.toArray(), (e) -> e.otherVertex(vertex).atan2(vertex))
      _.each incidentEdges, (edge, i) ->
        v1 = edge.otherVertex(vertex)
        v2 = incidentEdges[(i + 1) % incidentEdges.length].otherVertex(vertex)
        triangles.add(new Triangle(vertex, v1, v2)) if edges.include(new Edge(v1, v2))

    _.map(triangles.toArray(), (t) -> t.toArray())

  divideAndConquer: (vertices) ->
    if vertices.length < 2
      return [{}, vertices]
    else if vertices.length == 2
      return [new Set([new Edge(vertices[0], vertices[1]).connect()]), vertices]
    else if vertices.length == 3
      return [new Set([
        new Edge(vertices[0], vertices[1]).connect(),
        new Edge(vertices[1], vertices[2]).connect(),
        new Edge(vertices[2], vertices[0]).connect()
      ]), vertices]

    [leftVertices, rightVertices] = this.splitVertices(vertices)

    leftTriangulation = this.divideAndConquer(leftVertices)[0]
    rightTriangulation = this.divideAndConquer(rightVertices)[0]

    [bottomEdge, topEdge] = this.limitEdges(leftVertices, rightVertices)
    [leftVertex, rightVertex] = bottomEdge.vertices()

    edges = new Set([bottomEdge.connect()])

    while !(leftVertex == topEdge.v1 && rightVertex == topEdge.v2)
      leftAdjacent = this.verticesAbove(leftVertex.adjacent(), leftVertex, rightVertex)
      rightAdjacent = this.verticesAbove(rightVertex.adjacent(), leftVertex, rightVertex)

      leftAtan2 = rightVertex.atan2(leftVertex)
      rightAtan2 = leftVertex.atan2(rightVertex)

      leftAdjacent = _.sortBy leftAdjacent, (v) -> (v.atan2(leftVertex) - leftAtan2 + 360) % 360
      rightAdjacent = _.sortBy rightAdjacent, (v) -> (rightAtan2 - v.atan2(rightVertex) + 360) % 360

      newLeft = this.selectNext(leftTriangulation, leftVertex, leftAdjacent, leftVertex, rightVertex)
      newRight = this.selectNext(rightTriangulation, rightVertex, rightAdjacent, leftVertex, rightVertex)

      if !newLeft || newRight && this.inCircle(leftVertex, rightVertex, newLeft, newRight)
        rightVertex = newRight
      else
        leftVertex = newLeft

      if (leftVertex && rightVertex) then edges.add(new Edge(leftVertex, rightVertex).connect()) else break

    [edges.extend(leftTriangulation).extend(rightTriangulation), vertices]

  # convert a list of points into a list of vertices sorted by (x, y)-coordinate
  sortedVertices: (points) -> (new Vertex(p[0], p[1]) for p in points).sort Vertex.compare

  # splits sorted by x-coordinate sequence of vertices in two
  splitVertices: (vertices) ->
    m = Math.floor(vertices.length / 2)
    [vertices.slice(0, m), vertices.slice(m)]

  # returns bottom and top limit edges for two sorted sequencies of vertices
  limitEdges: (left, right) -> [this.limitEdge(left, right, -1), this.limitEdge(left, right, 1)]

  # finds limit edge by turn type (-1 - left turn, bottom edge; 1 - right turn, top edge)
  limitEdge: (leftVertices, rightVertices, turnType) ->
    left = this.convexHullHalf(leftVertices, turnType)
    right = this.convexHullHalf(rightVertices, turnType)

    [li, ri] = [left.length - 1, 0]

    while true
      [oldLi, oldRi] = [li, ri]

      li -= 1 while li - 1 >= 0 && this.turn(left[li - 1], left[li], right[ri]) != turnType
      ri += 1 while ri + 1 <= right.length - 1 && this.turn(left[li], right[ri], right[ri + 1]) != turnType

      return new Edge(left[li], right[ri]) if oldLi == li && oldRi == ri

  # return a half of convex hull corresponding to the given turn type
  # (-1 - left turn, bottom half; 1 - right turn, top half)
  convexHullHalf: (vertices, turnType) ->
    h = vertices.slice(0, 2)
    _.each vertices.slice(2), (v) =>
      h.pop() while h.length > 1 && this.turn(h[h.length - 2], h[h.length - 1], v) != turnType
      h.push(v)
    h

  # select next vertex for the bottom limit edge from the vertices adjacent to the vertex
  selectNext: (triangulation, vertex, adjacents, left, right) ->
    adjacents = adjacents.reverse()
    newVertex = adjacents.pop()
    while next = adjacents.pop()
      if this.inCircle(left, right, newVertex, next)
        triangulation.remove(new Edge(vertex, newVertex).disconnect())
        newVertex = next
      else
        break

    return newVertex

  # calculates turn: 0 - no turn, -1 - left turn, 1 - right turn
  turn: (v1, v2, v3) ->
    r = (v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x)
    if r > 0 then -1 else if r < 0 then 1 else 0

  # returnes vertices that are 'above' the line formed by left and right vertices
  verticesAbove: (vertices, left, right) -> vertices.filter (v) => this.turn(left, right, v) == -1

  # returns true if d is within (a, b, c) triangle's circumcircle
  inCircle: (a, b, c, d) ->
    [adx, ady, ad2] = [(a.x - d.x), (a.y - d.y), (a.x * a.x - d.x * d.x + a.y * a.y - d.y * d.y)]
    [bdx, bdy, bd2] = [(b.x - d.x), (b.y - d.y), (b.x * b.x - d.x * d.x + b.y * b.y - d.y * d.y)]
    [cdx, cdy, cd2] = [(c.x - d.x), (c.y - d.y), (c.x * c.x - d.x * d.x + c.y * c.y - d.y * d.y)]
    adx * (bdy * cd2 - cdy * bd2) - bdx * (ady * cd2 - cdy * ad2) + cdx * (ady * bd2 - bdy * ad2) > 0