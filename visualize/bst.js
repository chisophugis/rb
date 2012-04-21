
"use strict";

var bst = {}

var BstNode = function (data) {
  this.data = data
  this.left = null
  this.right = null
}

var NODE_RADIUS = 2
  , TREE_LEVEL_SEP = 30

bst.draw = function (root, ctx, xmin_0, xmax_0, ybase_0) {

  function _draw (node, xmin, xmax, ybase) {
    var halfway, childpos

    halfway= xmin + (xmax - xmin) / 2
    ctx.beginPath()
    ctx.arc(halfway, ybase, NODE_RADIUS, 0, 2*Math.PI, true)
    ctx.stroke()

    // FIXME: remove this duplication
    if (node.left !== null) {
      childpos = _draw(node.left, xmin, halfway, ybase+TREE_LEVEL_SEP)
      ctx.beginPath()
      ctx.moveTo(halfway, ybase)
      ctx.lineTo(childpos.x, childpos.y)
      ctx.stroke()
    }

    if (node.right !== null) {
      childpos = _draw(node.right, halfway, xmax, ybase+TREE_LEVEL_SEP)
      ctx.beginPath()
      ctx.moveTo(halfway, ybase)
      ctx.lineTo(childpos.x, childpos.y)
      ctx.stroke()
    }

    // our position
    return {x: halfway, y: ybase}
  }

  _draw(root, xmin_0, xmax_0, ybase_0)
}

var BstSize = function BstSize (node) {
  if (node === null) return 0
  return BstSize(node.left) + BstSize(node.right) + 1
}

bst.recursive_insert = function (root, data, lt) {
  function _insert (node, newdata) {
    if (node === null)
      return new BstNode(newdata)

    if (lt(newdata, node.data))
      node.left = _insert(node.left, newdata)
    else if (lt(node.data, newdata))
      node.right = _insert(node.right, newdata)
    return node
  }
  return _insert(root, data)
}

function shuffle (arr) {
  var ret= arr.slice(0)
    , i= ret.length, rand, tmp

  while (i !== 0) {
    rand = Math.floor(Math.random() * i)
    i-= 1
    tmp = ret[i]
    ret[i] = ret[rand]
    ret[rand] = tmp
  }

  return ret
}

// TODO: break into `random_bst(size)` and `draw_bst`
function draw_random_bst (size) {
  var i
    , canvas= document.getElementById('thecanvas')
    , ctx= canvas.getContext('2d')
    , root= null
    , lt= function (i,j) { return i < j }
    , data= []

  ctx.clearRect(0, 0, canvas.width, canvas.height)

  size= size || 10

  for (i = 0; i != size; i+= 1) data.push(Math.random())

  data.forEach(function (el) {
    root= bst.recursive_insert(root, el, lt)
  })

  bst.draw(root, ctx, 0, canvas.width, 10)
}

$('document').ready(function () { draw_random_bst() })

$('#anew').on('click', function (ev) {
  draw_random_bst(30)
})
