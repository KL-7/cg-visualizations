window.VERTICES_COUNT = 500

window.refresh = ->
  [width, height] = [1100, 700]

  d3.selectAll('svg').remove()
  svg = d3.select('#canvas').append('svg').attr('width', width).attr('height', height)
  referenceSvg = d3.select('#reference').append('svg').attr('width', width).attr('height', height)

  points = d3.range(VERTICES_COUNT - 4)
    .map(-> [Math.random() * width, Math.random() * height])
    .concat([[0, 0], [0, height], [width, 0], [width, height]])

  d = new Date
  referenceSvg.selectAll('path')
    .data(d3.geom.delaunay(points))
    .enter().append('path')
    .attr('d', (d) -> 'M' + d.join('L') + 'Z')
    .attr('class', (d, i) -> 'color9-' + (i % 9))
  console.log('reference: ' + (new Date - d))

  d = new Date
  svg.selectAll('path')
    .data(new Delaunay(points).build())
    .enter().append('path')
    .attr('d', (edge) -> 'M' + edge.toArray().join('L'))
  console.log('alg: ' + (new Date - d))

d3.select('#refresh').on 'click', window.refresh

window.refresh()