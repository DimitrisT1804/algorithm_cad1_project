#include "probabilities_annotations.h"

DdNode ***all_paths;
DdNode **path;
int path_size;
int all_paths_size;

void insert_node(DdNode *node, DdNode ***path)
{
    (*path) = (DdNode **) realloc( (*path), sizeof(DdNode *) * (path_size + 1));
    (*path)[path_size] = node;
    path_size++;
}

void remove_node(DdNode ***path, DdNode *node)
{
    int i;

    for(i = 0; i < path_size; i++)
    {
        if((*path)[i] == node)
        {
            break;
        }
    }

    if(i >= path_size)
    {
        return;
    }

    (*path)[i] = NULL;
    (*path)[i] = (*path)[path_size - 1];
    (*path) = (DdNode **) realloc((*path), sizeof(DdNode *) * (path_size - 1));
    path_size--;
    
}

void traverse_cudd(DdNode *node)
{
    if(node == NULL)
    {
        return;
    }

    if(Cudd_IsConstant(node))   // value 1 final node //
    {
        if(Cudd_V(node) == 0)
        {
            return;
        }
        all_paths = realloc(all_paths, sizeof(DdNode **) * (all_paths_size + 1));
        all_paths[all_paths_size] = path;
        all_paths_size++;

        // path = NULL;
        // path_size = 0;
        return;
    }
    // int path_size = 0;
    insert_node(node, &path);
    // path_size++;

    traverse_cudd(Cudd_T(node));
    traverse_cudd(Cudd_Not( Cudd_E(node) ) );

    // remove node from path because it is not a final node to constant 1 //
    remove_node(&path, node);

    printf("Path: \n");
}
