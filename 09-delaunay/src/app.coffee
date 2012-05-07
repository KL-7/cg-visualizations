width = 700
height = 400
verticesCount = 6

svg = d3.select('#canvas').append('svg').attr('width', width).attr('height', height)

# window.points = d3.range(verticesCount - 4)
#   .map(-> [Math.round(Math.random() * width), Math.round(Math.random() * height)])
#   .concat([[0, 0], [0, height], [width, 0], [width, height]])

window.points = [[320, 230], [500, 120], [110, 70], [300, 310], [500, 300], [50, 250], [250, 130]]

svg.selectAll('circle')
  .data(points)
  .enter().append('circle')
  .attr('r', 2)
  .attr('transform', (d) -> 'translate(' + d + ')')

svg.selectAll('path')
  .data(d3.geom.delaunay(points))
  .enter().append('path')
  .attr('d', (d) -> 'M' + d.join('L') + 'Z')
  .attr('class', (d, i) -> 'color9-' + (i % 9))

delaunay = new Delaunay(points)
window.edges = delaunay.build()


# _.each splited, (verts, i) ->
#   svg.selectAll('.vertex-' + i)
#     .data(verts).enter().append('circle')
#     .attr('class', 'vertex-' + i)
#     .attr('r', 2).attr('transform', (v) -> 'translate(' + v.toArray() + ')')

# _.each delaunay.limitEdges(splited[0], splited[1]), (edge) ->
#   svg.append('path').attr('d', d3.svg.line()(edge.toArray()))