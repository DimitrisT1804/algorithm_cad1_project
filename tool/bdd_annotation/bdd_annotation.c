#include "bdd_annotation.h"

DdManager *gbm;
char **NamesDot = NULL;
int *ghash_added = NULL;
int *gdepth_added = NULL;
int ghash_added_size = 0;

void annotate_bdds()
{
    int i;
    int j;
    int level;
    int pghash;
    int pgdepth;
    DdNode **IO_vars = NULL;
    DdNode **vars = NULL;
    int IO_vars_size = 0;
    int vars_size = 0;
    int chash;
    int cdepth;
    int lhash;
    int ldepth;
    char *curr_pin = NULL;
    int k;
    int m;
    int l;
    char **varNames = NULL;
    char **vars_row = NULL;
    int size_of_vars = 0;
    char *postfix = NULL;
    int pchash;
    int pchdepth;
    int plhash;
    int pldepth;
    int vars_row_size = 0;
    int size = 0;
    int f;

    gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    for(i = 0; i < gatepinhash_size; i++)
    {
        for(j = 0; j < HASHDEPTH; j++)
        {
            if(gatepinhash[i].hashpresent[j] != 0)
            {
                if(gatepinhashv[i].level[j] == 0)
                {
                    IO_vars = (DdNode **)realloc(IO_vars, (IO_vars_size + 1) * sizeof(DdNode *));
                    IO_vars[IO_vars_size] = Cudd_bddNewVar(gbm);
                    Cudd_Ref(IO_vars[IO_vars_size]);
                    IO_vars_size++; 

                    ghash_added = (int *)realloc(ghash_added, (ghash_added_size + 1) * sizeof(int));
                    gdepth_added = (int *)realloc(gdepth_added, (ghash_added_size + 1) * sizeof(int));
                    ghash_added[ghash_added_size] = i;
                    gdepth_added[ghash_added_size] = j;
                    ghash_added_size++;

                    if(gatepinhash[i].type[j] == WIRE)
                    {
                        chash = gatepinhash[i].parentComponent[j];
                        cdepth = gatepinhash[i].parentComponentDepth[j];
                        lhash = comphash[chash].lib_type[cdepth];
                        ldepth = comphash[chash].lib_type_depth[cdepth];
                        
                        if(strcmp(libhash[lhash].function[ldepth][0], "1") == 0)     // Logic 1 //
                        {
                            gatepinhashv[i].gatepin_bdd[j] = Cudd_ReadOne(gbm);
                            gatepinhash_prob[i].one_prob[j] = 1.0;
                            gatepinhash_prob[i].zero_prob[j] = 0.0;
                            continue;
                        }
                        if(strcmp(libhash[lhash].function[ldepth][0], "0") == 0)    // Logic 0 //
                        {
                            gatepinhashv[i].gatepin_bdd[j] = Cudd_Not(Cudd_ReadOne(gbm));
                            gatepinhash_prob[i].one_prob[j] = 0.0;
                            gatepinhash_prob[i].zero_prob[j] = 1.0;
                            continue;
                        }
                    }
                }
            }
        }
    }

    for(i = 0; i < ghash_added_size; i++)
    {
        printf("IO_vars[%d] = %s\n", i, gatepinhash[ghash_added[i]].name[gdepth_added[i]]);
    }

    for(level = 1; level < max_design_level; level++)
    {
        for(i = 0; i < gatepinhash_size; i++)
        {
            for(j = 0; j < HASHDEPTH; j++)
            {             
                if(gatepinhash[i].hashpresent[j] != 0 && gatepinhash[i].type[j] == WIRE)
                {
                    if (check_gatepin_type(i, j) == 1)  // it is Output //
                    {
                        if(gatepinhashv[i].level[j] == level)
                        {
                            chash = gatepinhash[i].parentComponent[j];
                            cdepth = gatepinhash[i].parentComponentDepth[j];
                            lhash = comphash[chash].lib_type[cdepth];
                            ldepth = comphash[chash].lib_type_depth[cdepth];
                            
                            for(f = 0; f < libhash[lhash].out_pins_count[ldepth]; f++)
                            {
                                postfix = seperate_variables(libhash[lhash].function[ldepth][f], &varNames, &vars_row, &size_of_vars, &vars_row_size);

                                vars = (DdNode **) realloc(vars, size_of_vars * sizeof(DdNode *));
                                for(k = 0; k < libhash[lhash].pin_count[ldepth]; k++)
                                {
                                    if(libhash[lhash].pin_type[ldepth][k] == INPUT)
                                    {
                                        curr_pin = (char *) calloc(strlen(comphash[chash].name[cdepth]) + 1 + strlen(libhash[lhash].pin_names[ldepth][k]), sizeof(char));
                                        strcpy(curr_pin, comphash[chash].name[cdepth]);
                                        strcat(curr_pin, libhash[lhash].pin_names[ldepth][k]);

                                        for(l = 1; l < size_of_vars; l++)
                                        {
                                            if(strcmp(libhash[lhash].pin_names[ldepth][k] + 1, varNames[l]) == 0)
                                            {
                                                break;
                                            }
                                        }
                                        
                                        get_predecessors_pin(curr_pin, &pghash, &pgdepth);
                                        
                                        if(gatepinhash[pghash].type[pgdepth] == IO_TYPE)
                                        {
                                            for(m = 0; m < ghash_added_size; m++)
                                            {
                                                if(pghash == ghash_added[m] && pgdepth == gdepth_added[m])
                                                {
                                                    break;
                                                }
                                            }
                                            if(m == ghash_added_size)
                                            {
                                                printf("Error IO DdNode not found!\n");
                                            }
                                            vars[l - 1] = IO_vars[m];
                                            vars_size++;  
                                        }
                                        else
                                        {
                                            pchash = gatepinhash[pghash].parentComponent[pgdepth];
                                            pchdepth = gatepinhash[pghash].parentComponentDepth[pgdepth];
                                            plhash = comphash[pchash].lib_type[pchdepth];
                                            pldepth = comphash[pchash].lib_type_depth[pchdepth];

                                            if(libhash[plhash].cell_type[pldepth] == SEQUENTIAL)
                                            {
                                                if(check_gatepin_type(pghash, pgdepth) == 1) // it is Output pin of flip flop //
                                                {
                                                    for(m = 0; m < ghash_added_size; m++)
                                                    {
                                                        if(pghash == ghash_added[m] && pgdepth == gdepth_added[m])
                                                        {
                                                            break;
                                                        }
                                                    }
                                                    if(m == ghash_added_size)
                                                    {
                                                        printf("Error IO DdNode not found!\n");
                                                    }
                                                    vars[l - 1] = IO_vars[m];
                                                    vars_size++;  
                                                }
                                            }
                                            else
                                            {
                                                vars[l - 1] = gatepinhashv[pghash].gatepin_bdd[pgdepth];
                                                vars_size++;
                                            }
                                        }


                                        free(curr_pin);
                                        curr_pin = NULL;    
                                    }
                                }

                                if(vars != NULL)
                                {
                                    size = vars_size;
                                    gatepinhashv[i].gatepin_bdd[j] = concat_bdds(vars, varNames, vars_row, postfix, size, vars_row_size);
                                    printf("write bdd to gatepin %s\n", gatepinhash[i].name[j]);
                                    free(vars);
                                    vars = NULL;
                                    vars_size = 0;
                                }
                                
                                postfix = NULL;

                                // Cudd_ReduceHeap(gbm, CUDD_REORDER_SYMM_SIFT, 0);
                                Cudd_ReduceHeap(gbm, 0, 0);
                                // gatepinhashv[i].gatepin_bdd[j] = Cudd_ReduceHeap(gbm, CUDD_REORDER_SAME, 0);
                                varNames = NULL;
                                vars_row = NULL;
                                size_of_vars = 0;
                            }
                        }
                    }

                }
            }
        }   
    }
    // edo mallon prepei na kano free ton manager i allios sto clear_design //

    for(i = 0; i < gatepinhash_size; i++)
    {
        for(j = 0; j < HASHDEPTH; j++)
        {
            if(gatepinhash[i].hashpresent[j] != 0)
            {
                if(gatepinhash[i].type[j] == WIRE)
                {
                    if(check_gatepin_type(i, j) == 0)  // it is Output //
                    {
                        continue;;
                    }
                    chash = gatepinhash[i].parentComponent[j];
                    cdepth = gatepinhash[i].parentComponentDepth[j];
                    lhash = comphash[chash].lib_type[cdepth];
                    ldepth = comphash[chash].lib_type_depth[cdepth];
                    if(libhash[lhash].cell_type[ldepth] == COMBINATIONAL)
                    {
                        continue;;
                    }
                    if(curr_pin != NULL)
                    {
                        free(curr_pin);
                        curr_pin = NULL;
                    }
                    curr_pin = (char *) calloc(strlen(comphash[chash].name[cdepth]) + 3, sizeof(char));
                    strcpy(curr_pin, comphash[chash].name[cdepth]);
                    strcat(curr_pin, "/D");

                    get_predecessors_pin(curr_pin, &pghash, &pgdepth);

                    gatepinhashv[i].gatepin_bdd[j] = gatepinhashv[pghash].gatepin_bdd[pgdepth];

                    free(curr_pin);
                    curr_pin = NULL;
                }
            }
        }
    }

    for(i = 0; i < gatepinhash_size; i++)
    {
        for(j = 0; j < HASHDEPTH; j++)
        {
            if(gatepinhash[i].hashpresent[j] != 0)
            {
                if(gatepinhash[i].type[j] == PO)
                {
                    get_predecessors_pin(gatepinhash[i].name[j], &pghash, &pgdepth);
                    gatepinhashv[i].gatepin_bdd[j] = gatepinhashv[pghash].gatepin_bdd[pgdepth];
                }
            }
        }
    }

    printf("Number of vars is %d\n", Cudd_ReadSize(gbm));
    Cudd_DebugCheck(gbm);
    
    // printf ("Nodes are %d\n", Cudd_CheckZeroRef(gbm));  

    NamesDot = (char **)realloc(NamesDot, (IO_vars_size + 1) * sizeof(char *));
    for(i = 0; i < IO_vars_size; i++)
    {
        NamesDot[i] = (char *)calloc(strlen(gatepinhash[ghash_added[i]].name[gdepth_added[i]]) + 1, sizeof(char));
        strcpy(NamesDot[i], gatepinhash[ghash_added[i]].name[gdepth_added[i]]);
    }

    free(IO_vars);
}