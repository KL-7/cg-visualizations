window.VERTICES_COUNT = 500

window.refresh = ->
  [width, height] = [1100, 700]

  d3.selectAll('svg').remove()
  canvas = d3.select('#canvas').append('svg').attr('width', width).attr('height', height)
  reference = d3.select('#reference').append('svg').attr('width', width).attr('height', height)

  points = d3.range(VERTICES_COUNT - 4)
    .map(-> [Math.random() * width, Math.random() * height])
    .concat([[0, 0], [0, height], [width, 0], [width, height]])

  drawTriangles = (svg, triangles) ->
    svg.selectAll('path')
      .data(triangles)
      .enter().append('path')
      .attr('d', (d) -> 'M' + d.join('L') + 'Z')
      .attr('class', (d, i) -> 'color9-' + (i % 9))

  d = new Date
  drawTriangles(canvas, new Delaunay(points).build())
  console.log('algorithm: ' + (new Date - d))

  d = new Date
  drawTriangles(reference, d3.geom.delaunay(points))
  console.log('reference: ' + (new Date - d))

d3.select('#refresh').on 'click', window.refresh

window.refresh()