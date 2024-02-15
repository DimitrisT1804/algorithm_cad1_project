#include "probabilities_annotations.h"

DdNode ***all_paths;
int path_size;
int all_paths_size;

void insert_node(DdNode *node, DdNode ***path, int path_size)
{
    (*path) = realloc( (*path), sizeof(DdNode *) * (path_size + 1));
    (*path)[path_size] = node;
}

void traverse_cudd(DdNode *node, DdNode **path)
{
    // int path_size = 0;
    insert_node(node, &path, path_size);
    path_size++;

    if(node == NULL)
    {
        return;
    }

    if(Cudd_IsConstant(node))   // value 1 final node //
    {
        all_paths = realloc(all_paths, sizeof(DdNode **) * (all_paths_size + 1));
        all_paths[all_paths_size] = path;
        all_paths_size++;
        return;
    }

    traverse_cudd(Cudd_T(node), path);
    traverse_cudd(Cudd_E(node), path);

    printf("Path: \n");
}