#define __rb_is_black(node) ((node) && rb_is_red(node))

void rb_insert_color(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *parent, *gparent;
	/* NOTE: it seems like `other_right` is the one that is used
	 * predominantly, so it might make sensed to change the meaning of
	 * the field to be "sibling index" rather than "my index" */
	/* PERFORMANCE_NOTE: Either of these can be trivially
	 * rematerialized from the other. Double-check that the compiler
	 * isn't doing something stupid. */
	int which_left, other_right;

	/* NOTE: loop could pretty easily be rearranged to avoid this. */
	goto inside:

	/* UNDERSTAND: Basically what we do in this loop is to go up the
	 * tree splitting 4-nodes (i.e. doing color flips while our uncle
	 * (and parent of course) is red) */
	while (true)
	{
		which_left = __rb_which_child(parent);
		other_right = __rb_other_child(which_left);

		/* NOTE: do this efficiently along with the
		 * __rb_which_child if possible. hint: we know that parent
		 * is red */
		gparent = rb_parent(parent);

		struct rb_node *const uncle = gparent->rb_children[other_right];
		/* BRANCH_NOTE: uncle being NULL. uncle being red */
		if (uncle == NULL || rb_color(uncle) == RB_COLOR_BLACK) {
			/* NOTE: leave the loop; we can finish with
			 * constant work now */
			goto choose_endgame;
		}
		/* UNDERSTAND: Begin color flip. To be continued below. */
		rb_set_black(uncle);
		rb_set_black(parent);
		node = gparent;
		/* UNDERSTAND: finish color flip and continue up the tree */
		/* XXX: This will make a NULL parent become barely non-NULL. */
		rb_set_red(node);
inside:
		parent = rb_parent(node);
		if (parent == NULL) {
			/* XXX: This is sticky, and I'm not sure how best
			 * to resolve it. On the one hand, we can just shut
			 * our eyes and say that the root (the node with
			 * the NULL parent) is always black. On the other
			 * hand, we can mandate that "black" means that the
			 * color bit is clear. This choice might not be a
			 * problem if it turns out that for performance it
			 * is best to have the low bit clear mean black,
			 * but it may turn out to be faster in other places
			 * to have that mean red. TBD.
			 */
			return;
		}
		if (rb_color(parent) == RB_COLOR_BLACK)
			return;
	}

choose_endgame:
	/* NOTE: Most (all?) other red-black trees with parent links merge
	 * these two endgames into one, since you can think of the first
	 * rotation of the "two rotation" case (case 2 in CLRS terminology)
	 * to simply be reducing it to the "one rotation" case (case 3 in
	 * CLRS terminology). Off the top of my head I can think of at
	 * least two extra (wasted) memory writes which are caused by that
	 * division, namely interlinking node and gparent, which is a waste
	 * of time since eventually gparent becomes the child of node
	 * anyway. In this implementation, we implement each case
	 * separately. As a result, other than 1 or 2 NULL-checking
	 * branches for the 1-rot or 2-rot case (respectively), the endgame
	 * should be branch-free. */
	/* UNDERSTAND: semantically, what we care about here is whether we
	 * are the "inner" or "outer" child. */
	if (__rb_which_child(node) == other_right)
		goto endgame_2_rot; /* "inner" */
	else
		goto endgame_1_rot; /* "outer" */

endgame_1_rot:

	const uintptr_t g_pcw = g->parent_color_which;
	p->parent_color_which = g_pcw;
	struct rb_node **replace;
	/* NOTE: assumes that NULL pcw doesn't have any low bits set. */
	if (g_pcw)
		replace = &pcw_parent(g_pcw)->rb_children[pcw_which(g_pcw)];
	else /* NOTE: new root */
		replace = &root->rb_node;
	*replace = p;

	/* NOTE: one of make_red or make_black will be a nop; which one it
	 * is will depend on the distribution and importance of code paths
	 * like this in which an instruction or two can be shaved off by
	 * doing that. */
	uintptr_t new_g_pcw = (uintptr_t)p;
	make_red(new_g_pcw);
	make_which(new_g_pcw, other_right);
	g->parent_color_which = new_g_pcw;

	struct rb_node **p_right = &p->rb_children[other_right];
	struct rb_node *t3 = *p_right;
	*p_right = g;
	if (t3) {
		uintptr_t new_t3_pcw = (uintptr_t)g;
		make_black(new_t3_pcw);
		make_which(new_t3_pcw, which_left);
		t3->parent_color_which = new_t3_pcw;
	}

	g->rb_children[which_left] = t3;

	return;

engame_2_rot:
	/* NOTE: we know very precisely the structure of the tree
	 * here. */
}
/* The tree has the following structure:
 * (* means red)
 *
 *                     g
 *                    / \
 *                   p*  t4
 *                 /  \
 *                t1   n*
 *                    /  \
 *                   t2   t3
 *
 * (t2 and t3 can be non-NULL since we may have moved up
 * the tree in the loop above)
 *
 * t1 is black because p is red.
 * t2 and t3 are black because they are either NULL right
 * after insertion or the result of a color flip.
 * t4 is black because that is the condition that breaks us
 * out of the loop above.
 *
 * What we want is the following:
 *
 *                      n
 *                    /   \
 *                   p*    g*
 *                  /\     /\
 *                t1  t2  t3 t4
 */
