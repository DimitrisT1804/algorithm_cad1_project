#include "probabilities_annotations.h"

DdNode ***all_paths;
DdNode **path;
int path_size;
int all_paths_size;
int *nodes_array;
double probability_gatepin;

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

void print_paths()
{
    int i;

    for(i = 0; i < ghash_added_size; i++)
    {
        // for(j = 0; all_paths[i][j] != NULL; j++)
        // {
        //     printf("%d ", Cudd_NodeReadIndex(all_paths[i][j]));
        // }
        // printf("\n");
        if(nodes_array[i] == 0)
        {
            printf("0 ");
        }
        else if(nodes_array[i] == 1)
        {
            printf("1 ");
        }
        else
        {
            printf("- ");
        }
    }
    printf("\n");

}

double number_of_paths;

void traverse_cudd(DdNode *node)
{
    int index;
    DdNode *NT, *NE;
    if(node == NULL)
    {
        return;
    }

    if(Cudd_IsConstant(node))   // value 1 final node //
    {
        if(cuddV(node) == 0)
        {
            return;
        }
        

        // Allocate memory for the new path and copy the current path into it
        // DdNode **new_path = malloc( (path_size + 1) * sizeof(DdNode *) );
        // memcpy(new_path, path, path_size * sizeof(DdNode *));
        // new_path[path_size] = NULL;

        // all_paths = realloc(all_paths, sizeof(DdNode **) * (all_paths_size + 1));
        // all_paths[all_paths_size] = new_path;
        // all_paths_size++;
        // for(int i = 0; i < ghash_added_size; i++)
        // {
        //     nodes_array[i] = -1;
        // }

        // for(i = 0; i < path_size; i++)
        // {
        //     nodes_array[Cudd_NodeReadIndex(path[i])] = 1;
        // }
        // if(path_size > 0)
        // {
            probability_gatepin += calculate_probabilities(nodes_array);
        // }

        number_of_paths++;
        return;
    }
    // insert_node(node, &path);

    NT = Cudd_T(node);
    NE = Cudd_E(node);
	if (Cudd_IsComplement(node)) 
    {
	    NT  = Cudd_Not(NT);
	    NE = Cudd_Not(NE);
	}
    index = Cudd_NodeReadIndex(node);
    nodes_array[index] = 0;

    // traverse_cudd(Cudd_T(node));
    // traverse_cudd(Cudd_Not( Cudd_E(node) ) );
    traverse_cudd(NT);
    nodes_array[index] = 1;
    traverse_cudd(NE);
    nodes_array[index] = -1;

    // remove node from path because it is not a final node to constant 1 //
    // remove_node(&path, node);

    // printf("Path: \n");

    // for (int i = 0; i < path_size; i++)
    // {
    //     printf("%d ", Cudd_NodeReadIndex(path[i]));
    // }
    // printf("\n");
}

/* Given a binary tree, print its nodes in inorder*/
void printInorder(DdNode* N)
{
    DdNode *node, *Nt, *Ne;

    node = Cudd_Regular(N);

    if(cuddIsConstant(node))
    {
        return;
    }
 

    Nt = cuddT(node);
    Ne = cuddE(node);
    if (node != N) 
    {
	    Nt = Cudd_Not(Nt);
        Ne = Cudd_Not(Ne);
    }
    /* first recur on left child */
    printInorder(Nt);
 
    /* then print the data of node */
    // printf("%d ", Cudd_NodeReadIndex(node));
 
    /* now recur on right child */
    printInorder(Ne);
}
 

double my_ddCountPathsToNonZero(DdNode * N, st_table * table)
{

    DdNode	*node, *Nt, *Ne;
    int index;

    node = Cudd_Regular(N);
    if (cuddIsConstant(node)) 
    {
        // if( (Cudd_IsComplement(N)) || (cuddV(node)==DD_ZERO_VAL) )
        // {
        //     return 0;
        // }
        if(cuddV(node) == DD_ZERO_VAL)
        {
            return 0;
        }

        probability_gatepin += calculate_probabilities(nodes_array);
	    // return((double) !(Cudd_IsComplement(N) || cuddV(node)==DD_ZERO_VAL));
        // print_paths();
        return 1;
    }
    // if (st_lookup(table, N, &dummy)) 
    // {
    //     paths = *(double *) dummy;
    //     return(paths);
    // }

    Nt = cuddT(node);
    Ne = cuddE(node);
    if (node != N) 
    {
	    Nt = Cudd_Not(Nt);
        Ne = Cudd_Not(Ne);
    }
    index = Cudd_NodeReadIndex(node);
    nodes_array[index] = 1;

    my_ddCountPathsToNonZero(Nt,table);
    nodes_array[index] = 0;
    // if (paths1 == (double)CUDD_OUT_OF_MEM)
    // {
    //     return((double)CUDD_OUT_OF_MEM);
    // }
    my_ddCountPathsToNonZero(Ne,table);
    nodes_array[index] = -1;
    // if (paths2 == (double)CUDD_OUT_OF_MEM)
    // {
    //     return((double)CUDD_OUT_OF_MEM);
    // }
    // paths = paths1 + paths2;

    // ppaths = malloc(sizeof(double));
    // if (ppaths == NULL) 
    // {
	//     return((double)CUDD_OUT_OF_MEM);
    // }

    // *ppaths = paths;

    // if (st_add_direct(table, N, ppaths) == ST_OUT_OF_MEM) 
    // {
    //     free(ppaths);
    //     ppaths = NULL;
    //     return((double)CUDD_OUT_OF_MEM);
    // }
    // // if(ppaths != NULL)
    // // {
    // //     free(ppaths);
    // // }
    // return(paths);
    return 0;

} /* end of ddCountPathsToNonZero */

// double my_ddCountPathsToNonZero(DdNode* N, st_table* table) 
// {
//     DdNode* node, *Nt, *Ne;
//     double paths, *ppaths, paths1, paths2;
//     void* dummy;
//     int index;

//     node = Cudd_Regular(N);
//     if (cuddIsConstant(node)) {
//         if (cuddV(node) == DD_ZERO_VAL) {
//             return 0;
//         }
//         probability_gatepin += calculate_probabilities(nodes_array);
//         return 1;
//     }

//     // if (st_lookup(table, N, &dummy)) {
//     //     paths = *(double*)dummy;
//     //     return paths;
//     // }

//     Nt = cuddT(node);
//     Ne = cuddE(node);
//     if (node != N) {
//         Nt = Cudd_Not(Nt);
//         Ne = Cudd_Not(Ne);
//     }
//     index = Cudd_NodeReadIndex(node);
//     nodes_array[index] = 0;


//     // Parallelize the recursive calls using OpenMP tasks
//     #pragma omp parallel
//     {
//         #pragma omp single nowait
//         {
//             #pragma omp task
//             {
//                 paths1 = my_ddCountPathsToNonZero(Nt, table);
//                 nodes_array[index] = 1;
//                 // nodes_array[index] = 0; // Reset the value
//             }

//             #pragma omp task
//             {
//                 paths2 = my_ddCountPathsToNonZero(Ne, table);
//                 nodes_array[index] = -1;
//                 // nodes_array[index] = 0; // Reset the value
//             }
//         }
//     }

//     // Wait for all tasks to complete before continuing
//     #pragma omp taskwait

//     // paths = paths1 + paths2;

//     // ppaths = malloc(sizeof(double));
//     // if (ppaths == NULL) {
//     //     return (double)CUDD_OUT_OF_MEM;
//     // }

//     // *ppaths = paths;

//     // if (st_add_direct(table, N, ppaths) == ST_OUT_OF_MEM) {
//     //     free(ppaths);
//     //     ppaths = NULL;
//     //     return (double)CUDD_OUT_OF_MEM;
//     // }

//     // return paths;
//     return 0;
// }



void write_minterms(int ghash, int gdepth)
{
    freopen("minterms.txt", "w", stdout);

    Cudd_PrintMinterm(gbm, gatepinhashv[ghash].gatepin_bdd[gdepth]);

    freopen("/dev/tty", "w", stdout);

    // read_minterms(gatepinhash[ghash].name[gdepth]);
}

double calculate_probabilities(int *vars_value)
{
    int i;
    double probability = 1.0;

    for(i = 0; i < ghash_added_size; i++)
    {
        #ifdef DEBUG
        printf("Variable: %s has value %d\n", NamesDot[i], vars_value[i]);
        #endif
        if(vars_value[i] == 0)
        {
            probability = probability * gatepinhash_prob[ghash_added[i]].zero_prob[gdepth_added[i]];
        }
        else if(vars_value[i] == 1)
        {
            probability = probability * gatepinhash_prob[ghash_added[i]].one_prob[gdepth_added[i]];
        }
    }

    return probability;

}

void read_minterms(char *gatepin_name)
{
    int read_length;
    size_t line_size = 0;
    char *line = NULL;
    FILE *filename = fopen("minterms.txt", "r");
    int *vars_value = NULL;
    int i;
    double probability_gatepin = 0.0;

    if(gatepin_name == NULL)
    {
        printf("Gatepin name is NULL\n");
        return;
    }

    while((read_length = (getline(&line, &line_size, filename) ) )!= -1)
    {
        #ifdef DEBUG
        printf("Line: %s\n", line);
        #endif 

        for(i = 0; i < read_length; i++)
        {
            if(line[i] == ' ')
            {
                break;
            }
            else if(line[i] == '0')
            {
                vars_value = realloc(vars_value, sizeof(int) * (i + 1));
                vars_value[i] = 0;
            }
            else if(line[i] == '1')
            {
                vars_value = realloc(vars_value, sizeof(int) * (i + 1));
                vars_value[i] = 1;
            }
            else if(line[i] == '-')
            {
                vars_value = realloc(vars_value, sizeof(int) * (i + 1));
                vars_value[i] = -1; // '-' means that the variable is not present in the minterm //
            }
        }

        probability_gatepin += calculate_probabilities(vars_value);
    }
    remove("minterms.txt");
    printf(ANSI_COLOR_GREEN "Probability of %s to be on logic-1 is: %lf\n" ANSI_COLOR_RESET, gatepin_name, probability_gatepin);
    free(vars_value);
}
