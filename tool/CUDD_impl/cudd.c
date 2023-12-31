//
#include "cudd.h"

int main()
{
    printf("Hello world!\n");

    DdManager *gbm;

    DdNode *bdd;
    DdNode *var_a;
    DdNode *var_b;
    DdNode *var_c;

    gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    var_a = Cudd_bddNewVar ( gbm ) ;
    var_b = Cudd_bddNewVar ( gbm ) ;
    var_c = Cudd_bddNewVar ( gbm ) ;

    bdd = Cudd_bddAnd ( gbm , var_a , var_b ) ; 
    bdd = Cudd_bddOr ( gbm , bdd , var_c ) ; 

    const char *varNames[] = {"Var_A", "Var_B", "Var_C"};

    Cudd_Ref(bdd);

    FILE *dotFile;
    dotFile = fopen("bdd.dot", "w");
    Cudd_DumpDot(gbm, 1, &bdd, (char **)varNames, NULL, dotFile);
    fclose(dotFile);

    Cudd_Quit(gbm);

    return 0;
}