width = 700
height = 400
verticesCount = 100

svg = d3.select('#canvas').append('svg').attr('width', width).attr('height', height)

vertices = d3.range(verticesCount - 4)
  .map(-> [Math.random() * width, Math.random() * height])
  .concat([[0, 0], [0, height], [width, 0], [width, height]])

svg.selectAll('circle')
  .data(vertices)
  .enter().append('circle')
  .attr('r', 2)
  .attr('transform', (d) -> 'translate(' + d + ')')

svg.selectAll('path')
  .data(d3.geom.delaunay(vertices))
  .enter().append('path')
  .attr('d', (d) -> 'M' + d.join('L') + 'Z')
  .attr('class', (d, i) -> 'color9-' + (i % 9))