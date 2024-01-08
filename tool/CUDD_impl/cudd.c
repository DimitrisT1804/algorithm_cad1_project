//
#include "cudd.h"
char **varNames;

/**
 * Print a dd summary
 * pr = 0 : prints nothing
 * pr = 1 : prints counts of nodes and minterms
 * pr = 2 : prints counts + disjoint sum of product
 * pr = 3 : prints counts + list of nodes
 * pr > 3 : prints counts + disjoint sum of product + list of nodes
 */

void print_dd (DdManager *gbm, DdNode *dd, int n, int pr, char *name)
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
    else if(operator == '(' || operator == ')')
    {
        return -2;
    }
    else
    {
        return 0;
    }
}

void generate_bdd(char *infix, char *cell_name)
{
    int i;
    int j;
    int k;
    int size = 0;
    int result;
    int var_size = 0;
    int found_operator = 0;
    char *postfix = NULL;
    int string_size = 1;
    char *out_name = NULL;
    int var_num = 1;
    int pos = 0;
    char *temp_name = NULL;
    int exists = 0;
    char **vars_row = NULL;
    int seperate_vars = 1;
    int need_reverse = 0;
    int not_eval = 0;
    int is_variable = 0;
    

    DdManager *gbm;

    postfix = parse_infix(infix);

    DdNode *bdd;
    DdNode *temp_bdd[2];
    int temp_bdd_num = 0;
    DdNode **vars;

    gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    Cudd_AutodynEnable(gbm, CUDD_REORDER_SYMM_SIFT);
    // Cudd_ReduceHeap(gbm, CUDD_REORDER_SYMM_SIFT, 3000);
    // Cudd_AutodynDisable(gbm);

    bdd = Cudd_bddNewVar(gbm);
    temp_bdd[0] = Cudd_bddNewVar(gbm);
    temp_bdd[1] = Cudd_bddNewVar(gbm);

    varNames = (char **) malloc(sizeof(char *) * 2);
    // varNames[var_num] = (char *) malloc(sizeof(char) * 5);
    temp_name = (char *) malloc(sizeof(char) * 5);
    for(i = 0; i < strlen(infix); i++)
    {
        if(identify_symbol(infix[i]) == 0 || identify_symbol(infix[i]) == -1)
        {
            // varNames[var_num] = (char *) realloc(varNames[var_num], sizeof(char) * (pos + 2));
            // varNames[var_num][pos] = infix[i];
            temp_name[pos] = infix[i];
            pos++;
        }
        else if(identify_symbol(infix[i]) != -2)  // it is operator //
        {
            temp_name[pos] = '\0';
            exists = 0;
            //printf("Var is %s\n", varNames[var_num]);
            pos = 0;
            // var_num++;
            // varNames[var_num] = (char *) malloc(sizeof(char) * 5);
            // temp_name = (char *) malloc(sizeof(char) * 5);
            if(strcmp(temp_name, "\0") == 0)
            {
                continue;
            }
            for(j = 1; j < seperate_vars; j++)
            {
                if(varNames[j] != NULL)
                {
                    if(strcmp(varNames[j], temp_name) == 0)
                    {
                        // strcpy(varNames[var_size], temp_name);
                        exists = 1;
                    }
                }
            }
            if(exists != 1)
            {
                varNames = (char **) realloc(varNames, sizeof(char *) * (seperate_vars + 2));
                // strcpy(varNames[seperate_vars], temp_name);
                varNames[seperate_vars] = strdup(temp_name);
                
                seperate_vars++; 
            }
        }
    }
    temp_name[pos] = '\0';
    exists = 0;
    //printf("Var is %s\n", varNames[seperate_vars]);
    pos = 0;
    // seperate_vars++;
    // varNames[seperate_vars] = (char *) malloc(sizeof(char) * 5);
    // temp_name = (char *) malloc(sizeof(char) * 5);
    for(j = 1; j < seperate_vars; j++)
    {
        if(varNames[j] != NULL)
        {
            if(strcmp(varNames[j], temp_name) == 0)
            {
                // strcpy(varNames[var_size], temp_name);
                exists = 1;
            }
        }
    }
    if(exists != 1)
    {
        varNames = (char **) realloc(varNames, sizeof(char *) * (seperate_vars + 2));
        // strcpy(varNames[seperate_vars], temp_name);
        varNames[seperate_vars] = strdup(temp_name);
        
        seperate_vars++; 
    }
    varNames[seperate_vars] = NULL;
    varNames[0] = NULL;


    size = seperate_vars-1;

    vars = (DdNode **) malloc(size * sizeof(DdNode*));
    if(vars == NULL)
    {
        printf("System Failure!\n");
    }
    for(i = 0; i < size; i++)
    {
        vars[i] = Cudd_bddNewVar(gbm);
    }

    var_num = 0;
    pos = 0;
    vars_row = (char **) malloc(sizeof(char *) * 1);
    for(i = 0; i < strlen(infix); i++) // keep variables in row from infix //
    {
        if(identify_symbol(infix[i]) == 0 || identify_symbol(infix[i]) == -1)
        {
            // varNames[var_num] = (char *) realloc(varNames[var_num], sizeof(char) * (pos + 2));
            // varNames[var_num][pos] = infix[i];
            temp_name[pos] = infix[i];
            pos++;
        }
        else if(identify_symbol(infix[i]) != -2)  // it is operator //
        {
            temp_name[pos] = '\0';
            if(strcmp(temp_name, "\0") == 0)
            {
                continue;
            }
            exists = 0;
            //printf("Var is %s\n", varNames[var_num]);
            pos = 0;
            vars_row = (char **) realloc(vars_row, sizeof(char *) * (var_num + 2));
            // strcpy(varNames[var_num], temp_name);
            vars_row[var_num] = strdup(temp_name);
            
            var_num++; 
        }
    }
    vars_row = (char **) realloc(vars_row, sizeof(char *) * (var_num + 2));
    // strcpy(varNames[var_num], temp_name);
    vars_row[var_num] = strdup(temp_name);
    
    var_num++; 

    for(i = 0; i < var_num; i++)
    {
        printf("Series are %s\n", vars_row[i]);
    }
    
    bdd = Cudd_ReadOne(gbm);

    for(i = 0; i < strlen(postfix); i++)
    {
        // if(var_size >= var_num)
        // {
        //     break;
        // }
        if(temp_bdd_num == 2)
        {
            temp_bdd_num = 0;
        }
        result = identify_symbol(postfix[i]);

        if(result == 3 || result == 2 || result == 1)
        {
            var_size = var_size - not_eval;
            not_eval = 0;
        }
        Cudd_Ref(bdd);

        if(var_size < var_num)
        {
            for(j = 1; j < seperate_vars; j++)  // need to change 3
            {
                if(strcmp(vars_row[var_size], varNames[j]) == 0)
                {
                    break;
                }
            }
            if(j == seperate_vars)
            {
                printf("ERROR\n");
            }

            if(var_size + 1 < var_num)
            {
                for(k = 1; k < seperate_vars; k++)  // need to change 3
                {
                    if(strcmp(vars_row[var_size+1], varNames[k]) == 0)
                    {
                        break;
                    }
                }
                if(k == seperate_vars)
                {
                    printf("ERROR\n");
                }
            }
        }

        if(result == 0) // is variable //
        {
            need_reverse = 0;
            is_variable++;
        }
        else if (result == 3)
        {
            need_reverse = 1;
            if(is_variable == 2)
            {
                temp_bdd[temp_bdd_num] = Cudd_bddAnd ( gbm , vars[j-1] , vars[k-1] );
                temp_bdd_num++;
                var_size++;
            }
            else if(is_variable == 1)
            {
                bdd = Cudd_bddAnd ( gbm ,vars[j-1], temp_bdd[0]);
            }
            else
            {
                temp_bdd[0] = Cudd_bddAnd(gbm, temp_bdd[0], temp_bdd[1]);
                temp_bdd_num = 0;
            }

            found_operator = 1;
            var_size++;
            is_variable = 0;
        }
        else if (result == 2)
        {
            need_reverse = 1;
            
            if(is_variable == 2)
            {
                temp_bdd[temp_bdd_num] = Cudd_bddOr ( gbm , vars[j-1] , vars[k-1] );
                var_size++;
                temp_bdd_num++;
            }
            else if(is_variable == 1)
            {
                bdd = Cudd_bddOr ( gbm , vars[j-1], temp_bdd[0]);
            }
            else 
            {
                temp_bdd[0] = Cudd_bddOr ( gbm , temp_bdd[0], temp_bdd[1]);
                temp_bdd_num = 0;
            }

            found_operator = 1;
            var_size++;
            is_variable = 0;
        }
        else if (result == 1)
        {
            need_reverse = 1;
            is_variable = 0;
            if(is_variable == 2)
            {
                bdd = Cudd_bddXor ( gbm , vars[j-1] , vars[k-1] );
                var_size++;
            }
            else if(found_operator)
            {
                bdd = Cudd_bddXor ( gbm , vars[j-1], bdd );
            }

            found_operator = 1;
            var_size++;
        }
        else if (result == 4)
        {
            if (need_reverse == 0)
            {
                vars[j-1] = Cudd_Not(vars[j-1]);
                var_size++;
                not_eval++;
            }
            else if(found_operator)
            {
                bdd = Cudd_Not (bdd);
            }
            is_variable = 0;
            // else
            // {
            //     bdd = Cudd_Not (vars[j-1]);
            //     var_size++;
            // }
            // found_operator = 1;
            // var_size++;
        }
    }
    bdd = temp_bdd[0];

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
    Cudd_DumpDot(gbm, 1, &bdd, NULL, NULL, dotFile);
    fclose(dotFile);

    print_dd(gbm, bdd, 2, 2, out_name); // prints info about bdd //
    Cudd_PrintMinterm(gbm, bdd); // prints minterms of bdd //
    // Cudd_PrintDebug(gbm, bdd, 1, 3);

    free(out_name);

    Cudd_Quit(gbm);
    gbm = NULL;

    free(temp_name);
    free(postfix);
    free(vars);

    for(i = 0; i < var_num; i++)
    {
        free(vars_row[i]);
    }
    free(vars_row);

    for(i = 0; i < seperate_vars; i++)
    {
        free(varNames[i]);
    }
    free(varNames);
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