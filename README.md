# Red-Black Trees

This repo contains some investigation I did (and hope to return to some
day) surveying the variety of red-black tree implementations, with the
intent of writing a better implementation.

## The bestiary

All of the red-black trees I could find across the net are inside the
`bestiary/` directory. The file `bestiary/__info.lua` contains an annotated
description of quite a few of them (alas, I haven't had time to get to all
of them, and it has been a while since I've worked on this stuff).
This description mostly compares their implementation techniques, and
highlights particularly interesting or well-written implementation
techniques.

A surprisingly large number of the red-black trees used in practice are
basically straight copies of the pseudocode in the CLRS algorithms
textbook without thinking about what the algorithm is actually doing. See
`bestiary/__info.lua` for more information.
One particular example of this is that most implementations treat the
left-hand and right-hand cases separately, using an `if` with two basically
identical branches except switching left for right. This duplication can be
removed by having the node struct store child nodes in an array (e.g.
`struct rb_node *children[2];`) and access the children through indexes.
This change reduces alone reduces code size by about half and gets rid of
unpredictable branches.
Another giveaway that the code is probably derived from CLRS is that the
"end-game" code for insert rebalancing is *inside* the loop that goes up
the tree splitting 4-nodes (which really obscures things).

## Other files

The file `try.c` contains an experimental unfinished insertion procedure
combining what I've learned from all the implementations I've surveyed.
It may look kind of ugly (it has some nasty goto's that could be removed
fairly easily by "rotating" the main loop, but I haven't done that yet).
The core of the code is really tight and (at least for me) is extremely
clear with what the algorithm is actually doing, since the logical parts
aren't mixed together inside a huge loop (like in CLRS and implementations
derived from it).
`try.c.bak` contains some remains from a previous version, but I think it
has some explanations in it that are worth keeping.

The file `dimensions.txt` contains some interesting "implementation
dimensions" along which an implementation can vary.

The file `linux.c` (in the root of the source tree) contains an annotated
description of the linux red-black tree code (the version I annotated here
has since been replaced by a new implementation which is in the bestiary;
at a glance, it looks like the new implementation is basically a cleanup of
the code, without significantly addressing the CLRS copying).
Primarily, the annotations describe inefficiencies or other interesting
facts that the code is not taking advantage of or that might be helpful for
writing a better version.
