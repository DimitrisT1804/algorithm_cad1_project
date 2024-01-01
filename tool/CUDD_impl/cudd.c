//
#include "cudd.h"

DdManager *gbm;
// const char varNames[10];

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
    else
    {
        return 0;
    }
}


DdNode *generate_bdd(char *infix)
{
    int i;
    int size = 0;
    int result;
    int var_size = 0;
    int found_operator = 0;
    char *postfix = NULL;

    postfix = parse_infix(infix);

    DdNode *bdd;
    DdNode **vars;

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

        if(result == 0)
        {
            // var_size++;
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
    }

    return bdd;
}

int main()
{
    printf("Hello world!\n");

    DdNode *bdd;
    // DdNode *var_a;
    // DdNode *var_b;
    // DdNode *var_c;

    gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    // var_a = Cudd_bddNewVar ( gbm ) ;
    // var_b = Cudd_bddNewVar ( gbm ) ;
    // var_c = Cudd_bddNewVar ( gbm ) ;


    // bdd = Cudd_bddAnd ( gbm , var_a , var_b ) ; 
    // bdd = Cudd_bddOr ( gbm , bdd , var_c ) ; 

    

    bdd = generate_bdd("(A*B*C)+D");

    Cudd_Ref(bdd);

    bdd = Cudd_BddToAdd(gbm, bdd);

    FILE *dotFile;
    dotFile = fopen("bdd.dot", "w");
    Cudd_DumpDot(gbm, 1, &bdd, NULL, NULL, dotFile);
    fclose(dotFile);

    // write_dd(gbm, bdd, "bdd.dot");

    print_dd(bdd, 2, 1, "test");

    Cudd_PrintMinterm(gbm, bdd);

    Cudd_Quit(gbm);

    return 0;
}