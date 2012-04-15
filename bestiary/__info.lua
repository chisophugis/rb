Note[[
  All of the code in each of these subdirectories is under its own license.
  This is just a collection (hence the name "bestiary") of red-black tree
  implementations. They are ripped straight out of their native
  environment, so it's unlikely that any of them can just be compiled and
  work. They are here for looking at the code, not running.

  All of the trees that I'm testing are implemented by me, all to the same
  interface, to prevent there from being any confounding variables when
  benchmarking. Also this lets me have strict control over things like
  memory allocation for the purposes of being able to test how different
  trees fare with respect to memory access patterns.
]]

Dir{"jsw_rbtree",
  link = "http://eternallyconfuzzled.com/tuts/datastructures/jsw_tut_rbtree.aspx",
  desc = [[
    One of the best implementations I have seen. Does not slavishly follow
    the CLRS code at all. In fact, the author claims to have not even
    looked at the CLRS code and figured out all of the cases on their own.
    Child pointers are stored in an array, allowing symmetric cases to be
    folded. Also, I love the rotation code (although the simplicity comes
    mostly from not having parent pointers).

    I recommend checking out the webpage. The explanation of the algorithms
    is good and surprisingly fresh.
  ]]
}

Dir{"nginx",
  desc = [[
    Slavishly follows CLRS, even down to actually using a real node as a
    sentinel, instead of NULL. Not very interesting.
  ]]
}

Dir{"netbsd",
  desc = [[
    This red-black tree code is the finest in this bestiary.

    This code uses an array for the child nodes allowing symmetric cases to
    be folded.

    This code is most interesting for how it has factored the cases of
    insertion rebalance and remove rebalance. The code in CLRS has all the
    cases inside an extremely convoluted while loop whose condition is used
    to terminate all the cases, even ones where we know we're done after
    executing them. However, this code has factored the cases so that the
    only thing inside the loop is the stuff that actually needs to be in
    the loop (for the most part; the loop in `rb_tree_removal_rebalance`
    seems to still be "sharing the while"). A good example of this
    factoring is in `rb_tree_insert_rebalance`, which factors "case 2" and
    "case 3" (as CLRS calls them) out of the loop (which just goes up the
    tree splitting 4-nodes by doing color-flips).

    It stores which child a node is along with the color. This is accessed
    with the macro `RB_POSITION()`. I had also been thinking about doing
    this. However, this implementation doesn't seem to take full advantage
    of having this information. For example, the test in rb.c:407 (which
    causes an extra memory access) could be avoided.
  ]]
}
