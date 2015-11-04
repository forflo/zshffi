#ifndef NARY_TREE
#define NARY_TREE

/*
 ---useful defines---
 */
#define MK_CONTENT struct node_content *c = \
    (struct node_content*) malloc(sizeof(struct node_content));

#define MK_LEAF struct nary_node *leaf = \
	new_node((void *) c);

struct nary_node {
	void *content; //pointer to the content
    int node_type;
    char name[20];
	struct nary_node **nodes; //array of pointers to child nodes
	int leaf; //1 if leaf 0 if node. If leaf = 0 => nnode > 0
	int nnode; //number of following childnodes 
};

struct nary_node *new_node(void *content);
int add_node(struct nary_node *parent, void *child_content);
int free_node(struct nary_node *node);
int add_child(struct nary_node *parent, struct nary_node *child);
int destroy_tree(struct nary_node *tree);

int traverse_preorder(struct nary_node *node, 
					int (*callback)(void *c, void *u), void *userparam);
int traverse_postorder(struct nary_node *node, 
					int (*callback)(void *c, void *u), void *u);


#endif /* NARY_TREE */
