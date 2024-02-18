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