#include <string.h>
#include <stdlib.h>
#include "linklist.h"
#include "simple_logger.h"

Node * linklist_new_node()
{
	Node *node;
	node = (Node *)malloc(sizeof(Node));
	if (!node)
	{
		slog("unable to allocate new linklist node");
		return NULL;
	}
	memset(node,0,sizeof(Node));
	return node;
}

int linklist_free_node(Node *node)
{
	if (!node)
	{
		slog("attempting to free a node that points to data");
		return -2;
	}
	if (node->data != NULL)
	{
		slog("attempting to free a node that points to data");
		return -1;
	}
	free(node);
	return 0;
}

void linklist_free(Node *head)
{
	Node *current;
	Node *next;
	if (!head)return;
	for (current = head; next != NULL; current = next)
	{
		next = current->next;
		linklist_free_node(current);
	}
}

/*eol@eof*/
