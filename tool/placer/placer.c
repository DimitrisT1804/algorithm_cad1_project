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

    double x_new = 0.0;
    double y_new = 0.0;
    double x_new_connection = 0.0;
    double y_new_connection = 0.0;

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

                x_new = compslocation[chash].x[cdepth] + (libhash[lhash].width[ldepth] / 2.0);

                y_new = compslocation[chash].y[cdepth] + (libhash[lhash].height[ldepth] / 2.0);
                for(k = 0; k < gatepinhash[ghash].connections_size[gdepth]; k++)
                {
                    ghash_connection = gatepinhash[ghash].pinConn[gdepth][k];
                    gdepth_connection = gatepinhash[ghash].pinConnDepth[gdepth][k];

                    chash_connection = gatepinhash[ghash_connection].parentComponent[gdepth_connection];
                    cdepth_connection = gatepinhash[ghash_connection].parentComponentDepth[gdepth_connection];

                    lhash_connection = comphash[chash_connection].lib_type[cdepth_connection];
                    ldepth_connection = comphash[chash_connection].lib_type_depth[cdepth_connection];

                    x_new_connection = compslocation[chash_connection].x[cdepth_connection] + (libhash[lhash_connection].width[ldepth_connection] / 2.0);

                    y_new_connection = compslocation[chash_connection].y[cdepth_connection] + (libhash[lhash_connection].height[ldepth_connection] / 2.0);

                    diff_x = abs( x_new_connection - x_new );
                    diff_y = abs( y_new_connection - y_new );

                    hpwl = hpwl + sqrt( pow(diff_x, 2) + pow(diff_y, 2)  );

                    // diff_x = 0.0;
                    // diff_y = 0.0;
                }
            }
            // else if(gatepinhash[ghash].type[gdepth] == IO_TYPE)
            // {
            //     for(k = 0; k < gatepinhash[ghash].connections_size[gdepth]; k++)
            //     {
            //         ghash_connection = gatepinhash[ghash].pinConn[gdepth][k];
            //         gdepth_connection = gatepinhash[ghash].pinConnDepth[gdepth][k];

            //         chash_connection = gatepinhash[ghash_connection].parentComponent[gdepth_connection];
            //         cdepth_connection = gatepinhash[ghash_connection].parentComponentDepth[gdepth_connection];

            //         lhash_connection = comphash[chash_connection].lib_type[cdepth_connection];
            //         ldepth_connection = comphash[chash_connection].lib_type_depth[cdepth_connection];

            //         diff_x = abs( gatepinhash[ghash].location_x[gdepth] - compslocation[ghash_connection].x[gdepth_connection] );
            //         diff_x = diff_x + (libhash[lhash_connection].width[ldepth_connection] / 2);

            //         diff_y = abs ( gatepinhash[ghash].location_y[gdepth] - compslocation[ghash_connection].y[gdepth_connection] );
            //         diff_y = diff_y + (libhash[lhash_connection].height[ldepth_connection] / 2);

            //         hpwl = hpwl + sqrt( (pow(diff_x, 2) + pow(diff_y, 2) ) );

            //         diff_x = 0.0;
            //         diff_y = 0.0;
            //     }
            // }

        }
    }

    return hpwl;
}

double random_double(double min, double max) 
{
    // Generate a random number between 0 and 1
    double random = (double)rand() / RAND_MAX;
    
    // Scale and shift the random number to be between min and max
    return min + random * (max - min);
}

void random_placer()
{
    int i;
    int j;
    double max_cell_width = 0.0;
    double max_cell_height = 0.0;

    for(i = 0; i < libhash_size; i++)
    {
        for(j = 0; j < HASHDEPTH; j++)
        {
            if(libhash[i].hashpresent[j] == 0)
            {
                continue;
            }

            if(libhash[i].width[j] > max_cell_width)
            {
                max_cell_width = libhash[i].width[j];
            }
            if(libhash[i].height[j] > max_cell_height)
            {
                max_cell_height = libhash[i].height[j];
            }
        }
    }

    for(i = 0; i < comphash_size; i++)
    {
        for(j = 0; j < HASHDEPTH; j++)
        {
            if(comphash[i].hashpresent[j] == 0)
            {
                continue;
            }

            compslocation[i].x[j] = random_double(0, (coresite[0].core_width - max_cell_width) );
            compslocation[i].y[j] = random_double(0, (coresite[0].core_height - max_cell_height) );
        }
    }
}