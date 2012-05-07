window.VERTICES_COUNT = 500

window.refresh = ->
  [width, height] = [1100, 700]

  d3.selectAll('svg').remove()

  points = d3.range(VERTICES_COUNT - 4)
    .map(-> [Math.random() * width, Math.random() * height])
    .concat([[0, 0], [0, height], [width, 0], [width, height]])

  drawTriangles = (container, triangles) ->
    d = new Date
    triangles = triangles()
    console.log(container + ': ' + (new Date - d) + 'ms')

    canvas = d3.select('#' + container).append('svg').attr('width', width).attr('height', height)

    canvas.selectAll('path')
      .data(triangles)
      .enter().append('path')
      .attr('d', (d) -> 'M' + d.join('L') + 'Z')
      .attr('class', (d, i) -> 'color9-' + (i % 9))

  drawTriangles 'algorithm', -> new Delaunay(points).build()
  # drawTriangles 'reference', -> d3.geom.delaunay(points)

d3.select('#refresh').on 'click', window.refresh

window.refresh()