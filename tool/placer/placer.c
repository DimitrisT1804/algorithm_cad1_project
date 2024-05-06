// placer //

#include "placer.h"

double calculate_HPWL()
{
    double hpwl = 0.0;
    int ghash;
    int gdepth;
    int ghash_connection;
    int gdepth_connection;
    int chash;
    int cdepth;
    int k;
    int chash_connection;
    int cdepth_connection;
    int lhash;
    int ldepth;
    int lhash_connection;
    int ldepth_connection;
    double diff_x = 0.0;
    double diff_y = 0.0;

    for(ghash = 0; ghash < gatepinhash_size; ghash++)
    {
        for(gdepth = 0; gdepth < HASHDEPTH; gdepth++)
        {
            if(gatepinhash[ghash].hashpresent[gdepth] == 0)
            {
                continue;
            }

            if(gatepinhash[ghash].type[gdepth] == WIRE)
            {
                chash = gatepinhash[ghash].parentComponent[gdepth];
                cdepth = gatepinhash[ghash].parentComponentDepth[gdepth];

                lhash = comphash[chash].lib_type[cdepth];
                ldepth = comphash[chash].lib_type_depth[cdepth];

                for(k = 0; k < gatepinhash[ghash].connections_size[gdepth]; k++)
                {
                    ghash_connection = gatepinhash[ghash].pinConn[gdepth][k];
                    gdepth_connection = gatepinhash[ghash].pinConnDepth[gdepth][k];

                    chash_connection = gatepinhash[ghash_connection].parentComponent[gdepth_connection];
                    cdepth_connection = gatepinhash[ghash_connection].parentComponentDepth[gdepth_connection];

                    lhash_connection = comphash[chash_connection].lib_type[cdepth_connection];
                    ldepth_connection = comphash[chash_connection].lib_type_depth[cdepth_connection];

                    diff_x = abs( compslocation[chash].x[cdepth] - compslocation[chash_connection].x[cdepth_connection] );
                    diff_x = diff_x - (libhash[lhash].width[ldepth] / 2) + (libhash[lhash_connection].width[ldepth_connection] / 2);

                    diff_y = abs ( compslocation[chash].y[cdepth] - compslocation[chash_connection].y[cdepth_connection] );
                    diff_y = diff_y - (libhash[lhash].height[ldepth] / 2) + (libhash[lhash_connection].height[ldepth_connection] / 2);

                    hpwl = hpwl + sqrt( (pow(diff_x, 2) + pow(diff_y, 2) ) );
                }
            }

            // need to fix it to calculate also from PI //

        }
    }

    return hpwl;
}