width = 1000
height = 800
verticesCount = 500

svg = d3.select('#canvas').append('svg').attr('width', width).attr('height', height)
referenceSvg = d3.select('#reference').append('svg').attr('width', width).attr('height', height)

window.points = d3.range(verticesCount - 4)
  # .map(-> [Math.round(Math.random() * width), Math.round(Math.random() * height)])
  .map(-> [Math.random() * width, Math.random() * height])
  .concat([[0, 0], [0, height], [width, 0], [width, height]])

# window.points = [[320, 230], [500, 120], [110, 70], [300, 310], [500, 300], [50, 250], [250, 130]]

# window.points = [
  # [0, 0], [0, 400], [700, 0], [700, 400], [350, 400]
  # [546, 364], [601, 400], [149, 302], [74, 208], [243, 123], [91, 367], [0, 0], [0, 400], [700, 0], [700, 400]
# ]

# svg.selectAll('circle')
#   .data(points)
#   .enter().append('circle')
#   .attr('r', 2)
#   .attr('transform', (d) -> 'translate(' + d + ')')
#

referenceSvg.selectAll('path')
  .data(d3.geom.delaunay(points))
  .enter().append('path')
  .attr('d', (d) -> 'M' + d.join('L') + 'Z')
  .attr('class', (d, i) -> 'color9-' + (i % 9))

delaunay = new Delaunay(points)
window.edges = delaunay.build()

svg.selectAll('path').data(edges).enter().append('path').attr 'd', (edge) -> 'M' + edge.toArray().join('L')

# _.each splited, (verts, i) ->
#   svg.selectAll('.vertex-' + i)
#     .data(verts).enter().append('circle')
#     .attr('class', 'vertex-' + i)
#     .attr('r', 2).attr('transform', (v) -> 'translate(' + v.toArray() + ')')

# _.each delaunay.limitEdges(splited[0], splited[1]), (edge) ->
#   svg.append('path').attr('d', d3.svg.line()(edge.toArray()))