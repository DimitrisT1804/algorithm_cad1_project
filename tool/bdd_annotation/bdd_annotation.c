#include "bdd_annotation.h"


void annotate_bdds()
{
    int i;
    int j;
    int level;
    int pghash;
    int pgdepth;
    DdNode **IO_vars = NULL;
    int *ghash_added = NULL;
    int *gdepth_added = NULL;
    DdNode **vars = NULL;
    int IO_vars_size = 0;
    int vars_size = 0;
    int ghash_added_size = 0;
    int chash;
    int cdepth;
    int lhash;
    int ldepth;
    char *curr_pin = NULL;
    int k;
    int m;

    gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);

    DdNode *bdd = Cudd_bddNewVar(gbm);
    DdNode *temp_bdd_1 = Cudd_bddNewVar(gbm);
    DdNode *temp_bdd_2 = Cudd_bddNewVar(gbm);

    for(i = 0; i < gatepinhash_size; i++)
    {
        for(j = 0; j < HASHDEPTH; j++)
        {
            if(gatepinhash[i].hashpresent[j] == 1)
            {
                if(gatepinhash[i].type[j] == IO_TYPE)
                {
                    IO_vars = (DdNode **)realloc(IO_vars, (IO_vars_size + 1) * sizeof(DdNode *));
                    IO_vars[IO_vars_size] = Cudd_bddNewVar(gbm);
                    IO_vars_size++; 

                    ghash_added = (int *)realloc(ghash_added, (ghash_added_size + 1) * sizeof(int));
                    gdepth_added = (int *)realloc(gdepth_added, (ghash_added_size + 1) * sizeof(int));
                    ghash_added[ghash_added_size] = i;
                    gdepth_added[ghash_added_size] = j;
                    ghash_added_size++;
                }
            }
        }
    }

    for(level = 2; level < max_design_level; level++)
    {
        for(i = 0; i < gatepinhash_size; i++)
        {
            for(j = 0; j < HASHDEPTH; j++)
            {
                //for(level = 2; level < max_design_level; level++)
                //{                
                    if(gatepinhash[i].hashpresent[j] == 1 && gatepinhash[i].type[j] == WIRE)
                    {
                        if (check_gatepin_type(i, j) == 1)  // it is Output //
                        {
                            if(gatepinhashv[i].level[j] == level)
                            {
                                chash = gatepinhash[i].parentComponent[j];
                                cdepth = gatepinhash[i].parentComponentDepth[j];
                                lhash = comphash[chash].lib_type[cdepth];
                                ldepth = comphash[chash].lib_type_depth[cdepth];

                                for(k = 0; k < libhash[lhash].pin_count[ldepth]; k++)
                                {
                                    if(libhash[lhash].pin_type[ldepth][k] == INPUT)
                                    {
                                        curr_pin = (char *) calloc(strlen(comphash[chash].name[cdepth]) + 1 + strlen(libhash[lhash].pin_names[ldepth][k]), sizeof(char));
                                        strcpy(curr_pin, comphash[chash].name[cdepth]);
                                        strcat(curr_pin, libhash[lhash].pin_names[ldepth][k]);
                                        
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
                                            vars = (DdNode **)realloc(vars, (vars_size + 1) * sizeof(DdNode *));
                                            vars[vars_size] = IO_vars[m];
                                            vars_size++;  

                                        }
                                        else
                                        {
                                            vars = (DdNode **)realloc(vars, (vars_size + 1) * sizeof(DdNode *));
                                            vars[vars_size] = gatepinhashv[pghash].gatepin_bdd[pgdepth];
                                            vars_size++;
                                        }

                                        free(curr_pin);
                                        curr_pin = NULL;    
                                    }
                                }
                                char *name = (char *) calloc(50, sizeof(char));
                                sprintf(name, "kati_%d", rand());

                                gatepinhashv[i].gatepin_bdd[j] = Cudd_bddNewVar(gbm);
                                gatepinhashv[i].gatepin_bdd[j] = concat_bdds(libhash[lhash].function[ldepth][0], name, vars, bdd, temp_bdd_1, temp_bdd_2, gbm);
                                printf("write bdd to gatepin %s\n", gatepinhash[i].name[j]);
                                if(vars != NULL)
                                {
                                    free(vars);
                                    vars = NULL;
                                    vars_size = 0;
                                }

                                // only combinational cells //
                            }
                        }

                    }
                //}
            }
        }   
    }

    // edo mallon prepei na kano free ton manager i allios sto clear_design //
}