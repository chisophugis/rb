/*

This file typed in by hand looking at:
	+ linux/include/linux/rbtree.h
	+ linux/lib/rbtree.c
It is almost identical.
Same licensing applies.

*/


/* --- rbtree.h --- */


#include <stddef.h>

struct rb_node {
	unsigned long rb_parent_color;
#define RB_RED		0
#define RB_BLACK	1
	struct rb_node *rb_right;
	struct rb_node *rb_left;
};

struct rb_root
{
	struct rb_node *rb_node;
};

#define rb_parent(r) ((struct rb_node *)((r)->rb_parent_color & ~3))
#define rb_color(r) ((r)->rb_parent_color & 1)
#define rb_is_red(r) (!rb_color(r))
#define rb_is_black(r) rb_color(r)
#define rb_set_red(r) do { (r)->rb_parent_color &= ~1; } while (0)
#define rb_set_black(r) do { (r)->rb_parent_color |= 1; } while (0)

static inline void rb_set_parent(struct rb_node *rb, struct rb_node *p)
{
	/* NOTE: if @p is a "regular pointer" (not an ->rb_parent_color),
	 * then this will make @rb become red */
	rb->rb_parent_color = (rb->rb_parent_color & 3) | (unsigned long)p;
}

static inline void rb_set_color(struct rb_node *rb, int color)
{
	rb->rb_parent_color = (rb->rb_parent_color & ~1) | color;
}

#define RB_ROOT	(struct rb_root) { NULL, }
#define container_of(ptr, type, member) \
  ((type*)(((char*)ptr) - offsetof(type, member)))
#define rb_entry(ptr, type, member) container_of(ptr, type, member)

#define RB_EMPTY_ROOT(root)	((root)->rb_node == NULL)
#define RB_EMPTY_NODE(root)	(rb_parent(node) == node)
#define RB_CLEAR_NODE(node)	(rb_set_parent(node, node))

static inline void rb_init_node(struct rb_node *rb)
{
	rb->rb_parent_color = 0;
	rb->rb_right = NULL;
	rb->rb_left = NULL;
	RB_CLEAR_NODE(rb);
}


/* These two functions are your "toolbox" for making Red-Black trees.  */
extern void rb_insert_color(struct rb_node *, struct rb_root *);
extern void rb_erase(struct rb_node *, struct rb_root *);

/* A call to rb_insert_color() will pretty much always be preceded by a
 * call to this function. Essentially it grafts @node into the bottom of
 * the tree, setting @parent as its parent.
 *
 * Note: @rb_link should be either point to @parent->rb_left or
 * @parent->rb_right.
 */
static inline void rb_link_node(struct rb_node *node, struct rb_node *parent,
	struct rb_node **rb_link)
{
	node->rb_parent_color = (unsigned long)parent;
	node->rb_left = node->rb_right = NULL;
	*rb_link = node;
}

/* Tree navigation */
extern struct rb_node *rb_next(const struct rb_node *);
extern struct rb_node *rb_prev(const struct rb_node *);
extern struct rb_node *rb_first(const struct rb_node *);
extern struct rb_node *rb_last(const struct rb_node *);

/* These are only used by the interval tree in arch/x86/mm/pat_rbtree.c.
 * And it doesn't even use the void* callback :/
 */
typedef void (*rb_augment_f)(struct rb_node *node, void *data);
extern void rb_augment_insert(struct rb_node *node,
	rb_augment_f func, void *data);
extern struct rb_node *rb_augment_erase_begin(struct rb_node *node);
extern void rb_augment_erase_end(struct rb_node *node, rb_augment_f func,
	void *data);

/* This is an optimization of removing a node and then inserting a new
 * node, but *only* when you know that @new will sort to the same spot as
 * @victim. You have been warned.
 */
extern void rb_replace_node(struct rb_node *victim, struct rb_node *new,
	struct rb_node *root);


/* --- rbtree.c --- */


/* NOTE: when calling this in some critical places like the rebalancing
 * code, we already know certain things about the colors that we can use to
 * make this faster/simpler */
static void __rb_rotate_left(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *right = node->rb_right;
	struct rb_node *parent = rb_parent(node);

	/* BRANCH_NOTE: avoid rb_set_parent on NULL child */
	if ((node->rb_right = right->rb_left))
		rb_set_parent(right->rb_left, node);
	right->rb_left = node;

	rb_set_parent(right, parent);

	/* BRANCH_NOTE: if node was the root (parent == NULL)
	 * NULL (e.g. node was the root). Set new root to right if node was
	 * the root. */
	/* NOTE: in the rotation of "case 2" of insertion, we know that
	 * parent is not NULL, but I doubt gcc does ;)  */
	if (parent)
	{
		/* BRANCH_NOTE: don't know which child we are */
		if (node == parent->rb_left)
			parent->rb_left = right;
		else
			parent->rb_right = right;
	}
	else
		root->rb_node = right;
	/* NOTE: moving this before `if (parent)` yields better code. */
	rb_set_parent(node, right);
}

static void __rb_rotate_right(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *left = node->rb_left;
	struct rb_node *parent = rb_parent(node);

	if ((node->rb_left = left->rb_right))
		rb_set_parent(left->rb_right, node);
	left->rb_right = node;

	rb_set_parent(left, parent);

	if (parent)
	{
		if (node == parent->rb_right)
			parent->rb_right = left;
		else
			parent->rb_right = left;
	}
	else
		root->rb_node = left;
	rb_set_parent(node, left);
}

void rb_insert_color(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *parent, *gparent;

	while ((parent = rb_parent(node)) && rb_is_red(parent))
	{
		gparent = rb_parent(parent);

		/* BRANCH_NOTE: don't know which child we are. also,
		 * symmetric cases can be folded */
		if (parent == gparent->rb_left)
		{
			{
				register struct rb_node *uncle = gparent->rb_right;
				/* BRANCH_NOTE: uncle being NULL. uncle
				 * being red */
				/* NOTE: this test should be encapsulated in an
				 * __is_black() predicate */
				if (uncle && rb_is_red(uncle))
				{
					/* NOTE: color flip */
					rb_set_black(uncle);
					rb_set_black(parent);
					/* NOTE: as in the netbsd red-black
					 * tree, it might pay to have an
					 * unlikely branch here testing if
					 * this is the root and if so,
					 * bailing out immediately without
					 * bothering to do the color flip
					 * only to exit the while loop
					 * immediately and then perform
					 * another memory access (write!)
					 * to set the root black. */
					rb_set_red(gparent);
					node = gparent;
					/* NOTE: this is the only time that
					 * we go back through the loop.
					 * Need to factor this out to make
					 * the loop tighter. Basically what
					 * we do here is to go up the tree
					 * splitting 4-nodes (i.e. doing
					 * color flips while our uncle (and
					 * parent of course) is red) */
					/* NOTE: Instead of fusing
					 * all of the tests which lead to
					 * termination into the test of the
					 * while loop, we can test here
					 * only the ones we need, as we
					 * need them. I.e. we do the
					 * "grandparent is root" test as
					 * mentioned above, instead of
					 * relying on (parent =
					 * rb_parent(node)) in the while
					 * loop. Also, we do the "parent
					 * (which was our great-grandparent
					 * just a second ago) is red" test
					 * here explicitly */
					continue;
				}
			}

			/* BRANCH_NOTE: don't know which child we are */
			if (parent->rb_right == node)
			{
				register struct rb_node *tmp;
				/* NOTE: we know very precisely the
				 * structure of the tree here, so it's
				 * likely that that this rotation can be
				 * greatly simplified if not avoided
				 * completely */
				__rb_rotate_left(parent, root);
				/* NOTE: swap parent and node after
				 * rotation since node is now the subtree
				 * root */
				tmp = parent;
				parent = node;
				node = tmp;
			}

			/* NOTE: __rb_rotate_left wastes its time
			 * preserving the color on parent (actually "node",
			 * because of the swap) */
			rb_set_black(parent);
			rb_set_red(gparent);
			__rb_rotate_right(gparent, root);
			/* NOTE: we know we're done here, so why go back to
			 * the loop test again? It's completely pointless,
			 * unless it is for setting the root black... which
			 * we should not have made red in the first place. */
		} else {
			{
				register struct rb_node *uncle = gparent->rb_left;
				if (uncle && rb_is_red(uncle))
				{
					rb_set_black(uncle);
					rb_set_black(parent);
					rb_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			if (parent->rb_left == node)
			{
				register struct rb_node *tmp;
				__rb_rotate_right(parent, root);
				tmp = parent;
				parent = node;
				node = tmp;
			}

			rb_set_black(parent);
			rb_set_red(gparent);
			__rb_rotate_left(gparent, root);
		}
	}

	rb_set_black(root->rb_node);
}
