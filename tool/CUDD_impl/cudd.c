//
#include "cudd.h"

DdManager *gbm;
char **varNames;

/**
 * Print a dd summary
 * pr = 0 : prints nothing
 * pr = 1 : prints counts of nodes and minterms
 * pr = 2 : prints counts + disjoint sum of product
 * pr = 3 : prints counts + list of nodes
 * pr > 3 : prints counts + disjoint sum of product + list of nodes
 */

void print_dd (DdNode *dd, int n, int pr, char *name)
{
    printf("%s\n", name);
    printf("DdManager nodes: %ld | ", Cudd_ReadNodeCount(gbm)); /*Reports the number of live nodes in BDDs and ADDs*/
    printf("DdManager vars: %d | ", Cudd_ReadSize(gbm) ); /*Returns the number of BDD variables in existance*/
    printf("DdNode nodes: %d | ", Cudd_DagSize(dd)); /*Reports the number of nodes in the BDD*/
	printf("DdNode vars: %d | ", Cudd_SupportSize(gbm, dd) ); /*Returns the number of variables in the BDD*/
    printf("DdManager reorderings: %d | ", Cudd_ReadReorderings(gbm) ); /*Returns the number of times reordering has occurred*/
    printf("DdManager memory: %ld |\n\n", Cudd_ReadMemoryInUse(gbm) ); /*Returns the memory in use by the manager measured in bytes*/
    Cudd_PrintDebug(gbm, dd, n, pr);	// Prints to the standard output a DD and its statistics: number of nodes, number of leaves, number of minterms.
}

void write_dd (DdManager *gbm, DdNode *dd, char* filename)
{
    FILE *outfile; // output file pointer for .dot file
    outfile = fopen(filename,"w");
    DdNode **ddnodearray = (DdNode**)malloc(sizeof(DdNode*)); // initialize the function array
    ddnodearray[0] = dd;
    Cudd_DumpDot(gbm, 1, ddnodearray, NULL, NULL, outfile); // dump the function to .dot file
    free(ddnodearray);
    fclose (outfile); // close the file */
}

int identify_symbol(char operator)
{
    if(operator == '!')
    {
        return 4;
    }
    else if(operator == '*')
    {
        return 3;
    }
    else if(operator == '+')
    {
        return 2;
    }
    else if(operator == '^')
    {
        return 1;
    }
    else if(isdigit(operator)) // is number //
    {
        return -1;
    }
    else
    {
        return 0;
    }
}


DdNode *generate_bdd(char *infix, char *cell_name)
{
    int i;
    int size = 0;
    int result;
    int var_size = 0;
    int found_operator = 0;
    char *postfix = NULL;
    int string_size = 1;
    char *out_name = NULL;

    postfix = parse_infix(infix);

    DdNode *bdd;
    DdNode **vars;

    gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    bdd = Cudd_bddNewVar(gbm);

    for(i = 0; i < strlen(postfix); i++)
    {
        result = identify_symbol(postfix[i]);

        if(result == 0)
        {
            size++;
        }
    }

    vars = (DdNode **) malloc(size * sizeof(DdNode*));
    if(vars == NULL)
    {
        printf("System Failure!\n");
    }
    for(i = 0; i < size; i++)
    {
        vars[i] = Cudd_bddNewVar(gbm);
    }
    printf("Size is %d\n", size);

    for(i = 0; i < strlen(postfix); i++)
    {
        result = identify_symbol(postfix[i]);

        if(result == -1)
        {
            // var_size++;
            string_size--;
            varNames[string_size][1] = postfix[i];
            varNames[string_size][2] = '\0';
            string_size++;
        }
        else if (result == 0)
        {
            varNames = (char **) realloc(varNames, (string_size+1) * sizeof(char*));
            varNames[string_size] = (char *) malloc(5 * sizeof(char));
            varNames[string_size][0] = postfix[i];
            varNames[string_size][1] = '\0';
            string_size++;
        }
        else if (result == 3)
        {
            if(found_operator)
            {
                bdd = Cudd_bddAnd ( gbm , bdd , vars[var_size] );
            }
            else
            {
                bdd = Cudd_bddAnd ( gbm , vars[var_size] , vars[var_size + 1] );
                var_size++;
            }
            found_operator = 1;
            var_size++;
        }
        else if (result == 2)
        {
            if(found_operator)
            {
                bdd = Cudd_bddOr ( gbm , bdd , vars[var_size] );
            }
            else
            {
                bdd = Cudd_bddOr ( gbm , vars[var_size] , vars[var_size + 1] );
                var_size++;
            }
            found_operator = 1;
            var_size++;
        }
        else if (result == 1)
        {
            if(found_operator)
            {
                bdd = Cudd_bddXor ( gbm , bdd , vars[var_size] );
            }
            else
            {
                bdd = Cudd_bddXor ( gbm , vars[var_size] , vars[var_size + 1] );
                var_size++;
            }
            found_operator = 1;
            var_size++;
        }

        // else if (result == 4)
        // {
        //     if(found_operator)
        //     {
        //         bdd = Cudd_Not ( gbm , bdd , vars[var_size] );
        //     }
        //     else
        //     {
        //         bdd = Cudd_bddOr ( gbm , vars[var_size] , vars[var_size + 1] );
        //         var_size++;
        //     }
        //     found_operator = 1;
        //     var_size++;
        // }
    }

    Cudd_Ref(bdd);

    bdd = Cudd_BddToAdd(gbm, bdd);

    out_name = malloc(sizeof(char) * (strlen(".dot ") + strlen(cell_name) + strlen("bdd_output/ ")) );
    strcpy(out_name, "bdd_output/");
    strcat(out_name, cell_name);
    strcat(out_name, ".dot");

    #ifdef DEBUG
    printf("The out_name is %s\n", out_name);
    #endif

    FILE *dotFile;
    dotFile = fopen(out_name, "w");
    Cudd_DumpDot(gbm, 1, &bdd, (char **) varNames, NULL, dotFile);
    fclose(dotFile);

    free(out_name);

    return bdd;
}

// int main()
// {
//     DdNode *bdd;
//     // DdNode *var_a;
//     // DdNode *var_b;
//     // DdNode *var_c;

//     gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

//     // var_a = Cudd_bddNewVar ( gbm ) ;
//     // var_b = Cudd_bddNewVar ( gbm ) ;
//     // var_c = Cudd_bddNewVar ( gbm ) ;


//     // bdd = Cudd_bddAnd ( gbm , var_a , var_b ) ; 
//     // bdd = Cudd_bddOr ( gbm , bdd , var_c ) ; 

//     bdd = generate_bdd("((A*B)+C)");
//     for(int i = 1; i < 5; i++)
//     {
//         //if(varNames[i] != NULL)
//             printf("it is %s\n", varNames[i]);
//     }
    


//     Cudd_Ref(bdd);

//     bdd = Cudd_BddToAdd(gbm, bdd);

//     FILE *dotFile;
//     dotFile = fopen("bdd.dot", "w");
//     Cudd_DumpDot(gbm, 1, &bdd, (char **) varNames, NULL, dotFile);
//     fclose(dotFile);

//     // write_dd(gbm, bdd, "bdd.dot");

//     print_dd(bdd, 2, 1, "test");

//     Cudd_PrintMinterm(gbm, bdd);

//     Cudd_Quit(gbm);

//     return 0;
// }