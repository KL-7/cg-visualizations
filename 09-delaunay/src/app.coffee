width = 700
height = 400
verticesCount = 100

svg = d3.select('#canvas').append('svg').attr('width', width).attr('height', height)

window.points = d3.range(verticesCount - 4)
  .map(-> [Math.random() * width, Math.random() * height])
  .concat([[0, 0], [0, height], [width, 0], [width, height]])

# svg.selectAll('circle')
#   .data(points)
#   .enter().append('circle')
#   .attr('r', 2)
#   .attr('transform', (d) -> 'translate(' + d + ')')
#
# svg.selectAll('path')
#   .data(d3.geom.delaunay(points))
#   .enter().append('path')
#   .attr('d', (d) -> 'M' + d.join('L') + 'Z')
#   .attr('class', (d, i) -> 'color9-' + (i % 9))

delaunay = new Delaunay
vertices = delaunay.sortedVertices(points)
splited = delaunay.splitVertices(vertices)

_.each splited, (verts, i) ->
  svg.selectAll('.vertex-' + i)
    .data(verts).enter().append('circle')
    .attr('class', 'vertex-' + i)
    .attr('r', 2).attr('transform', (v) -> 'translate(' + v.toArray() + ')')

# _.each delaunay.limitEdges(splited[0], splited[1]), (edge) ->
#   svg.append('path').attr('d', d3.svg.line()(edge.toArray()))