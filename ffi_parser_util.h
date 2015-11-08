#ifndef PARSER_UTIL
#define PARSER_UTIL

#include <stdarg.h>

/* Variadic convenience functionlfor syntax tree generation 
	Param: operation = The use of the node e.g. an if node would have
			P_OP_IF
		v = An valid pointer to an instance of struct value
		childs = The number of additional parameters
		... = The additional parameters have to be of type struct nary_node *
			These Pointers will be added to the generated node as childs in
			exactly the order they have been specified. 

	Return: Valid instance of struct nary_node or NULL if error occured 

	Example:
	-------
	The call make_node(P_OP_NOOP, NULL, a, b, c);
	generates a new node and adds a as his first child, b as
	his second and c as his last child. */
struct nary_node *make_node(int cnt, int operation, void *v, int childs, ...);

struct token_value { void *value; int length; };


#endif /* PARSER_UTIL */
