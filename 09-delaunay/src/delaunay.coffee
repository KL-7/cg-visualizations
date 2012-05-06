Array.prototype.clone = -> this.slice(0)


class Vertex
  constructor: (@x, @y) ->
  toArray: -> [@x, @y]
  atan2: (vertex) -> Math.atan2(@y - vertex.y, @x - vertex.x)


class Edge
  constructor: (@v1, @v2) ->
  vertices: -> [@v1, @v2]
  incident: (v) -> @v1 == v || @v2 == v
  otherVertex: (v) -> (if v == @v1 then @v2 else @v1) if incident(v)
  toArray: -> [@v1.toArray(), @v2.toArray()]


class Delaunay
  constructor: (@points) ->

  build: (points) ->
    edges = divideAndConquer(this.sortedVertices(points))

  divideAndConquer: (vertices) ->
    if vertices.length < 2
      return []
    else if vertices.length == 2
      return [new Edge(vertices[0], vertices[1])]
    else if vertices.length == 3
      return [
        new Edge(vertices[0], vertices[1]),
        new Edge(vertices[1], vertices[2]),
        new Edge(vertices[2], vertices[0])
      ]

    [leftVertices, rightVertices] = this.splitVertices(vertices)

    leftTriangulation = this.buildDelaunay(leftVertices)
    rightTriangulation = this.buildDelaunay(rightVertices)

    [bottomEdge, topEdge] = this.limitEdges(leftSet, rightSet)
    [leftVertex, rightVertex] = bottomEdge.vertices()

    edges = [new Edge(leftVertex, rightVertex)]

    while leftVertex != topEdge.v1 && rightVertex != topEdge.v2
      leftAdjacent = this.verticesAbove(this.adjacent(leftTriangulation, leftVertex), leftVertex, rightVertex)
      rightAdjacent = this.verticesAbove(this.adjacent(rightTriangulation, rightVertex), leftVertex, rightVertex)

      newLeft = this.selectNewVertex(leftAdjacent, leftVertex, rightVertex, leftTriangulation)
      newLeft = this.selectNewVertex(rightAdjacent, leftVertex, rightVertex, rightTriangulation)

      if this.inCircle(leftVertex, rightVertex, newLeft, newRight)
        leftVertex = newLeft
      else
        rightVertex = newRight

      edges.push(new Edge(leftVertex, rightVertex))

    edges.concat(leftTriangulation).concat(rightTriangulation)

  # convert a list of points into a list of vertices sorted by (x, y)-coordinate
  sortedVertices: (points) -> _.sortBy (new Vertex(p[0], p[1]) for p in points), (v) -> v.toArray()

  # splits sorted by x-coordinate sequence of vertices in two
  splitVertices: (vertices) ->
    m = Mathh.floor(vertices.length / 2)
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

  # calculates turn: 0 - no turn, -1 - left turn, 1 - right turn
  turn: (v1, v2, v3) ->
    r = (v2.x - v1.x) * (v3.y - v1.y) - (v2.y - v1.y) * (v3.x - v1.x)
    if r > 0 then 1 else if r < 0 then -1 else 0

  # returns vertices adjacent to the vertex in the triangulation
  adjacent: (triangulation, vertex) ->
    # TODO: remove full look-up by storing a reference to adjacent vertices (or incident edges) in the vertex
    edge.otherVertex(vertex) for edge in triangulation).filter((v) -> v?

  # returnes vertices that are 'above' the line formed by left and right vertices
  verticesAbove: (vertices, left, right) -> vertices.filter (v) -> this.turn(left, right, v) == -1

  # returns true if d is within (a, b, c) triangle's circumcircle
  inCircle: (a, b, c, d) ->
    [adx, ady, ad2] = [(a.x - d.x), (a.y - d.y), (a.x * a.x - d.x * d.x + a.y * a.y - d.y * d.y)]
    [bdx, bdy, bd2] = [(b.x - d.x), (b.y - d.y), (b.x * b.x - d.x * d.x + b.y * b.y - d.y * d.y)]
    [cdx, cdy, cd2] = [(c.x - d.x), (c.y - d.y), (c.x * c.x - d.x * d.x + c.y * c.y - d.y * d.y)]
    adx * (bdy * cd2 - cdy * bd2) - bdx * (ady * cd2 - cdy * ad2) + cdx * (ady * bd2 - bdy * ad2) > 0


window[cls] = eval(cls) for cls in ['Edge', 'Vertex', 'Delaunay']