/*

This file typed in by hand looking at:
	+ linux/include/linux/rbtree.h
	+ linux/lib/rbtree.c
It is almost identical.
Same licensing applies.

*/

#include "linux.h"


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
	/* NOTE: Any time that we would be rotating around the root, we
	 * have a special case that lets us terminate early. So basically
	 * having this check here is misguided, because we should never be
	 * rotating around the root */
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
			parent->rb_left = left;
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



static void __rb_erase_color(struct rb_node *node, struct rb_node *parent,
		struct rb_root *root)
{
	struct rb_node *other;

	while ((!node || rb_is_black(node)) && node != root->rb_node)
	{
		/* BRANCH_NOTE: don't know which child we are; also, can
		 * fold symmetric cases */
		if (parent->rb_left == node) {
			other = parent->rb_right;
			if (rb_is_red(other))
			{
				/* NOTE: pointless memory accesses. these
				 * are going to be accessed while rotating
				 * anyway. should set them then */
				rb_set_black(other);
				rb_set_red(parent);
				/* NOTE: we can terminate after this
				 * rotation if parent is the root */
				__rb_rotate_left(parent, root);
				/* NOTE: we already have this in a
				 * register while rotating! no need to
				 * fetch it! */
				other = parent->rb_right;
				/* NOTE: after this, we finish in constant
				 * time */
			}
			/* NOTE:
			 *	Outer	Inner	Action
			 *	R	B	case 4
			 *	R	R	case 4
			 *	B	R	case 3 falling into case 4
			 *	B	B	move up and continue loop
			 *
			 * This means that we need to test the outer child
			 * of our sibling first. If so, we can directly go
			 * to case 4.
			 * */
			if ((!other->rb_left || rb_is_black(other->rb_left)) &&
			    (!other->rb_right || rb_is_black(other->rb_right)))
			{
				rb_set_red(other);
				node = parent;
				parent = rb_parent(node);
				/* NOTE: This is the only place from which
				 * we continue the loop. */
			}
			else
			{
				/* NOTE: if sibling's outer child is not
				 * red, make it red. */
				/* BRANCH_NOTE: we already did this test
				 * above. */
				if (!other->rb_right || rb_is_black(other->rb_right))
				{
					rb_set_black(other->rb_left);
					rb_set_red(other);
					__rb_rotate_right(other, root);
					other = parent->rb_right;
				}
				rb_set_color(other, rb_color(parent));
				rb_set_black(parent);
				rb_set_black(other->rb_right);
				__rb_rotate_left(parent, root);
				node = root->rb_node;
				break;
			}
		} else {
			other = parent->rb_left;
			if (rb_is_red(other)) {
				rb_set_black(other);
				rb_set_red(parent);
				__rb_rotate_left(parent, root);
				other = parent->rb_left;
			}
			if ((!other->rb_left || rb_is_black(other->rb_left)) &&
			    (!other->rb_right || rb_is_black(other->rb_right)))
			{
				rb_set_red(other);
				node = parent;
				parent = rb_parent(node);
			}
			else
			{
				if (!other->rb_left || rb_is_black(other->rb_left))
				{
					rb_set_black(other->rb_right);
					rb_set_red(other);
					__rb_rotate_left(other, root);
					other = parent->rb_left;
				}
				rb_set_color(other, rb_color(parent));
				rb_set_black(parent);
				rb_set_black(other->rb_left);
				__rb_rotate_right(parent, root);
				node = root->rb_node;
				break;
			}
		}
	}
	if (node)
		rb_set_black(node);
}


void rb_erase(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *child, *parent;
	int color;

	if (!node->rb_left)
		child = node->rb_right;
	else if (!node->rb_right)
		child = node->rb_left;
	else
	{
		struct rb_node *old = node, *left;

		/* NOTE: find minimum in right subtree */
		node = node->rb_right;
		while ((left = node->rb_left) != NULL)
			node = left;

		/* BRANCH_NOTE: NULL check (for root) */
		if (rb_parent(old)) {
			/* BRANCH_NOTE: don't know which child we are */
			if (rb_parent(old)->rb_left == old)
				rb_parent(old)->rb_left = node;
			else
				rb_parent(old)->rb_right = node;
		} else
			root->rb_node = node;

		/* NOTE: this seems inefficient; we already have (or easily
		 * could have) this information in a register somewhere. */
		child = node->rb_right;
		parent = rb_parent(node);
		color = rb_color(node);

		/* BRANCH_NOTE: don't know if right subtree's minimum is
		 * the direct child */
		if (parent == old) {
			parent = node;
		} else {
			/* BRANCH_NOTE: NULL check */
			if (child)
				rb_set_parent(child, parent);
			parent->rb_left = child;

			node->rb_right = old->rb_right;
			rb_set_parent(old->rb_right, node);
		}

		node->rb_parent_color = old->rb_parent_color;
		node->rb_left = old->rb_left;
		rb_set_parent(old->rb_left, node);

		goto color;
	}

	parent = rb_parent(node);
	color = rb_color(node);

	/* BRANCH_NOTE: NULL check */
	if (child)
		rb_set_parent(child, parent);

	/* BRANCH_NOTE: NULL check (for root)*/
	if (parent)
	{
		/* BRANCH_NOTE: don't know which child we are */
		if (parent->rb_left == node)
			parent->rb_left = child;
		else
			parent->rb_right = child;
	} else
		root->rb_node = child;

color:
	if (color == RB_BLACK)
		__rb_erase_color(child, parent, root);
}

