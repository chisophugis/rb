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

Dir{"sgi_stl",
  desc = [[
    Follows the CLRS code pretty closely. Uses real NULL pointers as nil
    nodes.

    One interesting feature of this code is that its tree header node is
    just a regular node that stores the root of the tree as it "parent",
    the leftmost element of the tree as its "left child" and rightmost
    element of the tree as its "right child". It turns out that maintaining
    these leftmost and rightmost pointers actually is actually pretty
    cheap. For insert, just a comparison. For delete, it is just a
    comparison and possibly a tree (min|max)imum operation (if the right
    child is non-nil). It is interesting that the Linux red-black tree
    doesn't keep min and max naturally, but due to its "toolbox" approach,
    some users keep this information themselves (e.g. `timerqueue_(add|del)`)
    and it does not appear to be significantly less efficient (although I
    think that it nonetheless could be improved).
  ]]
}

Dir{"boost_intrusive",
  desc = [[
    Based on the sgi_stl code, so it follows CLRS pretty closely. The only
    major difference is that it is for intrusive containers, which means
    that none of the leftmost or rightmost caching takes place.

    The action takes place in `rebalance_after_erasure` and
    `rebalance_after_insertion`. Beware that there is the usual Boost
    template/namespace gobbledygook; e.g. pretty much all operations happen
    through a `NodeTraits` template parameter, so that you get code like
    `if(NodeTraits::get_color(w) == NodeTraits::red()){`. Ironically, for
    all of its "genericity", the design doesn't really let you abstract
    over anything interesting.
  ]]
}

Dir{"libcxx",
  desc = [[
    This is probably the best among all the CLRS-following red-black trees
    I have seen.

    Tree basic operations are all factored out very nicely and cleanly. For
    example, look at how well `__tree_is_left_child` and `__tree_min` come
    together to make `__tree_next` superbly clean and clear.

    The code, being standard library code (and for an internal class!), has
    underscores before basically everything (so you'll see a local called
    `__x` instead of just `x`).  Other than that (which can be easily fixed
    with sed), the code is very easy to read (well, as long as you are
    comfortable with C++). One caveat is how the actual node type is put
    together.  There is `__tree_end_node` which holds just a left pointer,
    and then `__tree_node_base` inherits from that and adds right and
    parent pointers. This is so that `__tree` can embed just a
    `__tree_end_node` inside itself as the header, and use that as a
    "parent" of the tree root (similar to the dummy node of the sgi_stl
    code).
  ]]
}

Dir{"kazlib",
  desc = [[
    Seems to more or less be a transcription of the CLRS code, including
    having a real nil-node. Extremely similar to the sgi_stl implementation
    except that it uses standard C ADT patterns like `void*` keys/values
    and function pointer comparators.

    One pretty cool thing about this red-black tree is the `main` function
    that it has at the bottom, which implements a simple command
    interpreter for interactively using the library or driving tests from a
    script. I really like that.
  ]]
}

Dir{"linux",
  desc = [[
    This is one of my favorites; exquisite "mechanism, not policy" design.

    Also includes augmented rbtree routines. These routines
    have recently been moved to having a duplicated implementation in a
    header file so that the invariant restoring functions can be inlined.
    The previous design was quite elegant (but slower) and didn't involve
    modifying the insert and erase routines to make them aware of the
    augmentation.
  ]]
}
