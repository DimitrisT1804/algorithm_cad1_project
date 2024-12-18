// placer //

#include "placer.h"

// gsl_spmatrix *array_A;
// gsl_spmatrix *array_IO;
// gsl_spmatrix *array_diag;
gsl_spmatrix *laplacian_matrix;
gsl_vector *io_locationx;
gsl_vector *io_locationy;

int IOs_size = 0;

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
            else if( (gatepinhash[ghash].type[gdepth] == IO_TYPE) || (gatepinhash[ghash].type[gdepth] == PO) )
            {
                for(k = 0; k < gatepinhash[ghash].connections_size[gdepth]; k++)
                {
                    ghash_connection = gatepinhash[ghash].pinConn[gdepth][k];
                    gdepth_connection = gatepinhash[ghash].pinConnDepth[gdepth][k];

                    chash_connection = gatepinhash[ghash_connection].parentComponent[gdepth_connection];
                    cdepth_connection = gatepinhash[ghash_connection].parentComponentDepth[gdepth_connection];

                    lhash_connection = comphash[chash_connection].lib_type[cdepth_connection];
                    ldepth_connection = comphash[chash_connection].lib_type_depth[cdepth_connection];

                    diff_x = abs( gatepinhash[ghash].location_x[gdepth] - compslocation[ghash_connection].x[gdepth_connection] );
                    diff_x = diff_x + (libhash[lhash_connection].width[ldepth_connection] / 2);

                    diff_y = abs ( gatepinhash[ghash].location_y[gdepth] - compslocation[ghash_connection].y[gdepth_connection] );
                    diff_y = diff_y + (libhash[lhash_connection].height[ldepth_connection] / 2);

                    hpwl = hpwl + sqrt( (pow(diff_x, 2) + pow(diff_y, 2) ) );
                }
            }

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

            compslocation[i].x[j] = random_double(0, (coresite->core_width - max_cell_width) );
            compslocation[i].y[j] = random_double(0, (coresite->core_height - max_cell_height) );
        }
    }
}

void calculate_hpwl_new(double *net_hpwl, double *IO_hpwl, double *total_hpwl)
{
    double max_x = 0.0;
    double max_y = 0.0;
    double min_x = 0.0;
    double min_y = 0.0;
    int chash;
    int cdepth;
    int ghash;
    int gdepth;
    int ghash_connection;
    int gdepth_connection;
    int chash_connection;
    int cdepth_connection;
    int lhash;
    int ldepth;
    int lhash_connection;
    int ldepth_connection;
    int k;
    double dest_location_x = 0.0;
    double dest_location_y = 0.0;

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

                max_x = compslocation[chash].x[cdepth];
                max_y = compslocation[chash].y[cdepth];
                min_x = compslocation[chash].x[cdepth];
                min_y = compslocation[chash].y[cdepth];

                for(k = 0; k < gatepinhash[ghash].connections_size[gdepth]; k++)
                {
                    ghash_connection = gatepinhash[ghash].pinConn[gdepth][k];
                    gdepth_connection = gatepinhash[ghash].pinConnDepth[gdepth][k];

                    chash_connection = gatepinhash[ghash_connection].parentComponent[gdepth_connection];
                    cdepth_connection = gatepinhash[ghash_connection].parentComponentDepth[gdepth_connection];

                    lhash_connection = comphash[chash_connection].lib_type[cdepth_connection];
                    ldepth_connection = comphash[chash_connection].lib_type_depth[cdepth_connection];

                    dest_location_x = compslocation[chash_connection].x[cdepth_connection];
                    dest_location_y = compslocation[chash_connection].y[cdepth_connection];

                    if(dest_location_x > max_x)
                    {
                        max_x = dest_location_x;
                    }
                    if(dest_location_y > max_y)
                    {
                        max_y = dest_location_y;
                    }
                    if(dest_location_x < min_x)
                    {
                        min_x = dest_location_x;
                    }
                    if(dest_location_y < min_y)
                    {
                        min_y = dest_location_y;
                    }
                }

                *net_hpwl = *net_hpwl + fabs(max_x - min_x) + fabs(max_y - min_y);
            }
            else if(gatepinhash[ghash].type[gdepth] == IO_TYPE)
            {
                max_x = gatepinhash[ghash].location_x[gdepth];
                max_y = gatepinhash[ghash].location_y[gdepth];
                min_x = gatepinhash[ghash].location_x[gdepth];
                min_y = gatepinhash[ghash].location_y[gdepth];
                
                for(k = 0; k < gatepinhash[ghash].connections_size[gdepth]; k++)
                {
                    ghash_connection = gatepinhash[ghash].pinConn[gdepth][k];
                    gdepth_connection = gatepinhash[ghash].pinConnDepth[gdepth][k];

                    if(gatepinhash[ghash_connection].type[gdepth_connection] == IO_TYPE)
                    {
                        continue;
                    }

                    chash_connection = gatepinhash[ghash_connection].parentComponent[gdepth_connection];
                    cdepth_connection = gatepinhash[ghash_connection].parentComponentDepth[gdepth_connection];

                    lhash_connection = comphash[chash_connection].lib_type[cdepth_connection];
                    ldepth_connection = comphash[chash_connection].lib_type_depth[cdepth_connection];

                    dest_location_x = compslocation[chash_connection].x[cdepth_connection];
                    dest_location_y = compslocation[chash_connection].y[cdepth_connection];
                    if(dest_location_x > max_x)
                    {
                        max_x = dest_location_x;
                    }
                    if(dest_location_y > max_y)
                    {
                        max_y = dest_location_y;
                    }
                    if(dest_location_x < min_x)
                    {
                        min_x = dest_location_x;
                    }
                    if(dest_location_y < min_y)
                    {
                        min_y = dest_location_y;
                    }
                }
                *IO_hpwl = *IO_hpwl + fabs(max_x - min_x) + fabs(max_y - min_y);
            }
            else if( gatepinhash[ghash].type[gdepth] == PO)
            {
                max_x = gatepinhash[ghash].location_x[gdepth];
                max_y = gatepinhash[ghash].location_y[gdepth];
                min_x = gatepinhash[ghash].location_x[gdepth];
                min_y = gatepinhash[ghash].location_y[gdepth];
                
                for(k = 0; k < gatepinhash[ghash].connections_size[gdepth]; k++)
                {
                    int is_output = 0;

                    ghash_connection = gatepinhash[ghash].pinConn[gdepth][k];
                    gdepth_connection = gatepinhash[ghash].pinConnDepth[gdepth][k];

                    is_output = check_gatepin_type(ghash_connection, gdepth_connection);

                    if(is_output == -1) // it is input //
                    {
                        continue;
                    }

                    if(gatepinhash[ghash_connection].type[gdepth_connection] == IO_TYPE)
                    {
                        continue;
                    }

                    chash_connection = gatepinhash[ghash_connection].parentComponent[gdepth_connection];
                    cdepth_connection = gatepinhash[ghash_connection].parentComponentDepth[gdepth_connection];

                    lhash_connection = comphash[chash_connection].lib_type[cdepth_connection];
                    ldepth_connection = comphash[chash_connection].lib_type_depth[cdepth_connection];

                    dest_location_x = compslocation[chash_connection].x[cdepth_connection];
                    dest_location_y = compslocation[chash_connection].y[cdepth_connection];

                    if(dest_location_x > max_x)
                    {
                        max_x = dest_location_x;
                    }
                    if(dest_location_y > max_y)
                    {
                        max_y = dest_location_y;
                    }
                    if(dest_location_x < min_x)
                    {
                        min_x = dest_location_x;
                    }
                    if(dest_location_y < min_y)
                    {
                        min_y = dest_location_y;
                    }
                }
                *IO_hpwl = *IO_hpwl + fabs(max_x - min_x) + fabs(max_y - min_y);
            }
        }
    }

    *total_hpwl = *net_hpwl + *IO_hpwl;
}

void calculate_hpwl_pararel(double *net_hpwl, double *IO_hpwl, double *total_hpwl)
{
    double max_x, max_y, min_x, min_y;
    int chash, cdepth, ghash, gdepth;
    int ghash_connection, gdepth_connection, chash_connection, cdepth_connection;
    int lhash, ldepth, lhash_connection, ldepth_connection;
    int k;
    double dest_location_x, dest_location_y;
    double local_net_hpwl = 0.0;
    double local_IO_hpwl = 0.0;

    #pragma omp parallel for private(gdepth, max_x, max_y, min_x, min_y, chash, cdepth, lhash, ldepth, k, ghash_connection, gdepth_connection, chash_connection, cdepth_connection, lhash_connection, ldepth_connection, dest_location_x, dest_location_y) reduction(+:local_net_hpwl,local_IO_hpwl)
    for (ghash = 0; ghash < gatepinhash_size; ghash++)
    {
        for (gdepth = 0; gdepth < HASHDEPTH; gdepth++)
        {
            if (gatepinhash[ghash].hashpresent[gdepth] == 0)
            {
                continue;
            }

            if (gatepinhash[ghash].type[gdepth] == WIRE)
            {
                chash = gatepinhash[ghash].parentComponent[gdepth];
                cdepth = gatepinhash[ghash].parentComponentDepth[gdepth];

                max_x = compslocation[chash].x[cdepth];
                max_y = compslocation[chash].y[cdepth];
                min_x = compslocation[chash].x[cdepth];
                min_y = compslocation[chash].y[cdepth];

                for (k = 0; k < gatepinhash[ghash].connections_size[gdepth]; k++)
                {
                    ghash_connection = gatepinhash[ghash].pinConn[gdepth][k];
                    gdepth_connection = gatepinhash[ghash].pinConnDepth[gdepth][k];

                    chash_connection = gatepinhash[ghash_connection].parentComponent[gdepth_connection];
                    cdepth_connection = gatepinhash[ghash_connection].parentComponentDepth[gdepth_connection];

                    dest_location_x = compslocation[chash_connection].x[cdepth_connection];
                    dest_location_y = compslocation[chash_connection].y[cdepth_connection];

                    if (dest_location_x > max_x)
                    {
                        max_x = dest_location_x;
                    }
                    if (dest_location_y > max_y)
                    {
                        max_y = dest_location_y;
                    }
                    if (dest_location_x < min_x)
                    {
                        min_x = dest_location_x;
                    }
                    if (dest_location_y < min_y)
                    {
                        min_y = dest_location_y;
                    }
                }

                local_net_hpwl += fabs(max_x - min_x) + fabs(max_y - min_y);
            }
            else if (gatepinhash[ghash].type[gdepth] == IO_TYPE)
            {
                max_x = gatepinhash[ghash].location_x[gdepth];
                max_y = gatepinhash[ghash].location_y[gdepth];
                min_x = gatepinhash[ghash].location_x[gdepth];
                min_y = gatepinhash[ghash].location_y[gdepth];

                for (k = 0; k < gatepinhash[ghash].connections_size[gdepth]; k++)
                {
                    ghash_connection = gatepinhash[ghash].pinConn[gdepth][k];
                    gdepth_connection = gatepinhash[ghash].pinConnDepth[gdepth][k];

                    if (gatepinhash[ghash_connection].type[gdepth_connection] == IO_TYPE)
                    {
                        continue;
                    }

                    chash_connection = gatepinhash[ghash_connection].parentComponent[gdepth_connection];
                    cdepth_connection = gatepinhash[ghash_connection].parentComponentDepth[gdepth_connection];

                    dest_location_x = compslocation[chash_connection].x[cdepth_connection];
                    dest_location_y = compslocation[chash_connection].y[cdepth_connection];

                    if (dest_location_x > max_x)
                    {
                        max_x = dest_location_x;
                    }
                    if (dest_location_y > max_y)
                    {
                        max_y = dest_location_y;
                    }
                    if (dest_location_x < min_x)
                    {
                        min_x = dest_location_x;
                    }
                    if (dest_location_y < min_y)
                    {
                        min_y = dest_location_y;
                    }
                }
                local_IO_hpwl += fabs(max_x - min_x) + fabs(max_y - min_y);
            }
            else if (gatepinhash[ghash].type[gdepth] == PO)
            {
                max_x = gatepinhash[ghash].location_x[gdepth];
                max_y = gatepinhash[ghash].location_y[gdepth];
                min_x = gatepinhash[ghash].location_x[gdepth];
                min_y = gatepinhash[ghash].location_y[gdepth];

                for (k = 0; k < gatepinhash[ghash].connections_size[gdepth]; k++)
                {
                    int is_output = 0;

                    ghash_connection = gatepinhash[ghash].pinConn[gdepth][k];
                    gdepth_connection = gatepinhash[ghash].pinConnDepth[gdepth][k];

                    is_output = check_gatepin_type(ghash_connection, gdepth_connection);

                    if (is_output == -1) // it is input //
                    {
                        continue;
                    }

                    if (gatepinhash[ghash_connection].type[gdepth_connection] == IO_TYPE)
                    {
                        continue;
                    }

                    chash_connection = gatepinhash[ghash_connection].parentComponent[gdepth_connection];
                    cdepth_connection = gatepinhash[ghash_connection].parentComponentDepth[gdepth_connection];

                    dest_location_x = compslocation[chash_connection].x[cdepth_connection];
                    dest_location_y = compslocation[chash_connection].y[cdepth_connection];
                    if (dest_location_x > max_x)
                    {
                        max_x = dest_location_x;
                    }
                    if (dest_location_y > max_y)
                    {
                        max_y = dest_location_y;
                    }
                    if (dest_location_x < min_x)
                    {
                        min_x = dest_location_x;
                    }
                    if (dest_location_y < min_y)
                    {
                        min_y = dest_location_y;
                    }
                }
                local_IO_hpwl += fabs(max_x - min_x) + fabs(max_y - min_y);
            }
        }
    }

    #pragma omp critical
    {
        *net_hpwl += local_net_hpwl;
        *IO_hpwl += local_IO_hpwl;
    }

    *total_hpwl = *net_hpwl + *IO_hpwl;
}


// void create_array_A()
// {
//     int i;
//     int j;
//     int k;

//     for(i = 0; i < gatepinhash_size; i++)
//     {
//         for(j = 0; j < HASHDEPTH; j++)
//         {
//             if(gatepinhash[i].hashpresent[j] == 0)
//             {
//                 continue;
//             }

//             for(k = 0; k < gatepinhash[i].connections_size[j]; k++)
//             {
//                 [i][gatepinhash[i].pinConn[j][k]] = 
//             }
//         }
//     }
// }

// // Function to calculate the degree matrix from an adjacency matrix
// void calculate_degree_matrix(int IO_size) 
// {
//     // Iterate over each row of the adjacency matrix
//     for (int i = 0; i < comphash_size - 1; i++) 
//     {
//         int degree = 0;
//         // Sum the entries in the row to calculate the degree
//         for (int j = 0; j < comphash_size - 1; j++) 
//         {
//             degree += gsl_spmatrix_get(array_A, i, j);
//         }

//         for (int j = 0; j < IOs_size; j++) 
//         {
//             degree += gsl_spmatrix_get(array_IO, i, j);
//         }
//         // Set the corresponding diagonal element in the degree matrix
//         // degree_matrix[i * comphash_size - 1 + i] = degree;
//         gsl_spmatrix_set(array_diag, i, i, degree);
//     }
// }

// // Function to perform element-wise subtraction between two sparse matrices
// void spmatrix_sub() 
// {
//     // Iterate over non-zero elements of matrix A
//     for (size_t i = 0; i < comphash_size - 1; i++) 
//     {
//         for (size_t j = 0; j < comphash_size - 1; j++) 
//         {
//             double valA = gsl_spmatrix_get(array_diag, i, j);
//             double valB = gsl_spmatrix_get(array_A, i, j);
//             gsl_spmatrix_set(laplacian_matrix, i, j, valA - valB); // Subtract corresponding elements //
//         }
//     }
// }

// void create_array_A()
// {
//     int i;
//     int j;
//     int k;
//     int ghash_connection;
//     int gdepth_connection;
//     int chash;
//     int cdepth;
//     int chash_connection;
//     int cdepth_connection;
//     int value = 0;

//     array_A = gsl_spmatrix_alloc(comphash_size - 1, comphash_size - 1);
//     gsl_spmatrix_set_zero(array_A);

//     for(i = 0; i < comphash_size - 1; i++)
//     {
//         for(j = 0; j < HASHDEPTH; j++)
//         {
//             if(comphash[i].hashpresent[j] == 0)
//             {
//                 continue;
//             }

//             compslocation[i].value[j] = value;
//             printf("component %s has value: %d\n", comphash[i].name[j], value);
//             value++;
//         }
//     }

//     value = 0;

//     for(i = 0; i < gatepinhash_size; i++)
//     {
//         for(j = 0; j < HASHDEPTH; j++)
//         {
//             if(gatepinhash[i].hashpresent[j] == 0)
//             {
//                 continue;
//             }

//             if(gatepinhash[i].type[j] == IO_TYPE || gatepinhash[i].type[j] == PO)
//             {
//                 gatepinhash[i].value[j] = value;
//                 printf("PI %s has value: %d\n", gatepinhash[i].name[j], value);
//                 value++;
//             }

//         }
//     }

//     array_IO = gsl_spmatrix_alloc(comphash_size - 1, value);
//     gsl_spmatrix_set_zero(array_IO);

//     IOs_size = value - 1;

//     for(i = 0; i < gatepinhash_size; i++)
//     {
//         for(j = 0; j < HASHDEPTH; j++)
//         {
//             if(gatepinhash[i].hashpresent[j] == 0)
//             {
//                 continue;
//             }

//             if(gatepinhash[i].type[j] == WIRE)
//             {
//                 for(k = 0; k < gatepinhash[i].connections_size[j]; k++)
//                 {   
//                     chash = gatepinhash[i].parentComponent[j];
//                     cdepth = gatepinhash[i].parentComponentDepth[j];

//                     ghash_connection = gatepinhash[i].pinConn[j][k];
//                     gdepth_connection = gatepinhash[i].pinConnDepth[j][k];

//                     chash_connection = gatepinhash[ghash_connection].parentComponent[gdepth_connection];
//                     cdepth_connection = gatepinhash[ghash_connection].parentComponentDepth[gdepth_connection];

//                     gsl_spmatrix_set(array_A, compslocation[chash].value[cdepth], compslocation[chash_connection].value[cdepth_connection], 1);

//                     gsl_spmatrix_set(array_A, compslocation[chash_connection].value[cdepth_connection], compslocation[chash].value[cdepth], 1);
//                 }

//             }
//             else if(gatepinhash[i].type[j] == IO_TYPE || gatepinhash[i].type[j] == PO)
//             {
//                 for(k = 0; k < gatepinhash[i].connections_size[j]; k++)
//                 {
//                     ghash_connection = gatepinhash[i].pinConn[j][k];
//                     gdepth_connection = gatepinhash[i].pinConnDepth[j][k];

//                     chash_connection = gatepinhash[ghash_connection].parentComponent[gdepth_connection];
//                     cdepth_connection = gatepinhash[ghash_connection].parentComponentDepth[gdepth_connection];

//                     gsl_spmatrix_set(array_IO, compslocation[chash_connection].value[cdepth_connection], gatepinhash[i].value[j], 1);

//                 }
//             }
//         }
//     }

//     // gsl_spmatrix_set(array_A, 0, 1, 1);
//     // gsl_spmatrix_set(array_A, 1, 0, 1);

//     // Print the sparse matrix
//     printf("Sparse Matrix A:\n");
//     gsl_spmatrix_fprintf(stdout, array_A, "%g");

//     printf("\n\nSparse Matrix IO:\n");
//     gsl_spmatrix_fprintf(stdout, array_IO, "%g");

//     array_diag = gsl_spmatrix_alloc(comphash_size - 1, comphash_size - 1);
//     gsl_spmatrix_set_zero(array_diag);

//     calculate_degree_matrix(comphash_size - 1);

//     printf("\n\nSparse Matrix diag:\n");
//     gsl_spmatrix_fprintf(stdout, array_diag, "%g");

//     laplacian_matrix = gsl_spmatrix_alloc(comphash_size - 1, comphash_size - 1);
//     gsl_spmatrix_set_zero(laplacian_matrix);

//     spmatrix_sub();
//     printf("\n\nSparse Matrix laplacian:\n");
//     gsl_spmatrix_fprintf(stdout, laplacian_matrix, "%g");
// }

// void create_pin_vectors()
// {
//     int i;
//     int j;
//     int ghash;
//     int gdepth;
//     double value_x = 0.0;
//     double temp_x = 0.0;
//     double value_y = 0.0;
//     double temp_y = 0.0;

//     io_locationx = gsl_spmatrix_alloc(comphash_size - 1, 1);
//     gsl_spmatrix_set_zero(io_locationx);

//     for(i = 0; i < comphash_size - 1; i++)
//     {
//         value_x = 0.0;
//         temp_x = 0.0;

//         for(j = 0; j < IOs_size; j++)
//         {

//             // value_x = value_x + gsl_spmatrix_get(array_IO, i, j);
//             temp_x = gsl_spmatrix_get(array_IO, i, j);

//             get_gatepin_from_value(j, &ghash, &gdepth);
//             if(gdepth == -1)
//             {
//                 printf("Error: gatepin not found\n");
//                 exit(1);
//             }
//             // printf("Gatepin is %s\n", gatepinhash[ghash].name[gdepth]);
            
//             temp_x = temp_x * gatepinhash[ghash].location_x[gdepth];

//             value_x = value_x + temp_x;
//         }

//         // multiply by location of IO //
//         if(value_x != 0)
//         {
//             value_x = -value_x;
//         }

//         gsl_spmatrix_set(io_locationx, i, 0, value_x);
//     }

//     printf("\n\nSparse Matrix io_locationx:\n");
//     gsl_spmatrix_fprintf(stdout, io_locationx, "%g");

//     io_locationy = gsl_spmatrix_alloc(comphash_size - 1, 1);
//     gsl_spmatrix_set_zero(io_locationy);

//     for(i = 0; i < comphash_size - 1; i++)
//     {
//         value_y = 0.0;
//         temp_y = 0.0;

//         for(j = 0; j < IOs_size; j++)
//         {

//             // value_x = value_x + gsl_spmatrix_get(array_IO, i, j);
//             temp_y = gsl_spmatrix_get(array_IO, i, j);

//             get_gatepin_from_value(j, &ghash, &gdepth);
//             if(gdepth == -1)
//             {
//                 printf("Error: gatepin not found\n");
//                 exit(1);
//             }
//             // printf("Gatepin is %s\n", gatepinhash[ghash].name[gdepth]);
            
//             temp_y = temp_y * gatepinhash[ghash].location_y[gdepth];

//             value_y = value_y + temp_y;
//         }

//         // multiply by location of IO //
//         if(value_y != 0)
//         {
//             value_y = -value_y;
//         }

//         gsl_spmatrix_set(io_locationx, i, 0, value_y);
//     }

//     printf("\n\nSparse Matrix io_locationy:\n");
//     gsl_spmatrix_fprintf(stdout, io_locationy, "%g");
// }

// void solve_linear_system()
// {
//     int chash;
//     int cdepth; 

//     gsl_vector *x = gsl_vector_alloc(comphash_size - 1);
//     gsl_vector *y = gsl_vector_alloc(comphash_size - 1);

//     gsl_splinalg_itersolve *solver = gsl_splinalg_itersolve_alloc(gsl_splinalg_itersolve_gmres, comphash_size - 1, 0);
//     gsl_spmatrix *A_csc = gsl_spmatrix_alloc_nzmax(comphash_size - 1, comphash_size - 1, comphash_size - 1, GSL_SPMATRIX_CSC);
//     A_csc = gsl_spmatrix_compcol(laplacian_matrix);

//     // Convert sparse vector b to a dense vector
//     gsl_vector *b_dense = gsl_vector_alloc(comphash_size - 1);
//     for (size_t i = 0; i < comphash_size - 1; ++i) 
//     {
//         gsl_vector_set(b_dense, i, gsl_spmatrix_get(io_locationx, i, 0));
//     }

//     gsl_vector_scale(b_dense, -1.0);

//     int status;
//     double tol = 1e-7;
//     size_t iter = 0, max_iter = 1000;

//     do 
//     {
//         status = gsl_splinalg_itersolve_iterate(A_csc, b_dense, tol, x, solver);
//         iter++;
//     }
//     while (status == GSL_CONTINUE && iter < max_iter);

//     if (status == GSL_SUCCESS) 
//     {
//         printf("Converged after %zu iterations.\n", iter);
//         for (size_t i = 0; i < comphash_size - 1; i++) 
//         {
//             printf("x_%zu = %g\n", i, gsl_vector_get(x, i));
//         }
//     } 
//     else 
//     {
//         printf("Failed to converge.\n");
//     }

//     for(int i = 0; i < comphash_size - 2; i++)
//     {
//         get_component_from_value(i, &chash, &cdepth);
//         if(cdepth == -1)
//         {
//             printf("Error: component not found\n");
//             exit(1);
//         }

//         compslocation[chash].x[cdepth] = gsl_vector_get(x, i);

//     }
    
//     do 
//     {
//         status = gsl_splinalg_itersolve_iterate(A_csc, b_dense, tol, y, solver);
//         iter++;
//     }
//     while (status == GSL_CONTINUE && iter < max_iter);

//     if (status == GSL_SUCCESS) 
//     {
//         printf("Converged after %zu iterations.\n", iter);
//         for (size_t i = 0; i < comphash_size - 1; i++) 
//         {
//             printf("y_%zu = %g\n", i, gsl_vector_get(y, i));
//         }
//     } 
//     else 
//     {
//         printf("Failed to converge.\n");
//     }

//     for(int i = 0; i < comphash_size - 2; i++)
//     {
//         get_component_from_value(i, &chash, &cdepth);
//         if(cdepth == -1)
//         {
//             printf("Error: component not found\n");
//             exit(1);
//         }

//         compslocation[chash].y[cdepth] = gsl_vector_get(y, i);
//     }

// }


void create_laplacian_matrix()
{
    int i;
    int j;
    int k;
    int chash;
    int cdepth;
    int ghash_connection;
    int gdepth_connection;
    int chash_connection;
    int cdepth_connection;
    int value = 0;
    double current_matrix_value = 0;

    gsl_vector *x = gsl_vector_alloc(comphash_size - 1);
    gsl_vector *y = gsl_vector_alloc(comphash_size - 1);
    gsl_vector_set_zero(x);
    gsl_vector_set_zero(y);

    gsl_splinalg_itersolve *solver = gsl_splinalg_itersolve_alloc(gsl_splinalg_itersolve_gmres, comphash_size - 1, 0);

    laplacian_matrix = gsl_spmatrix_alloc(comphash_size - 1, comphash_size - 1);
    gsl_spmatrix_set_zero(laplacian_matrix);

    for(i = 0; i < comphash_size; i++)
    {
        for(j = 0; j < HASHDEPTH; j++)
        {
            if(comphash[i].hashpresent[j] == 0)
            {
                continue;
            }

            compslocation[i].value[j] = value;
            printf("component %s has value: %d\n", comphash[i].name[j], value);
            value++;
        }
    }

    value = 0;

    for(i = 0; i < gatepinhash_size; i++)
    {
        for(j = 0; j < HASHDEPTH; j++)
        {
            if(gatepinhash[i].hashpresent[j] == 0)
            {
                continue;
            }

            if(gatepinhash[i].type[j] == IO_TYPE || gatepinhash[i].type[j] == PO)
            {
                continue;
            }
            
            chash = gatepinhash[i].parentComponent[j];
            cdepth = gatepinhash[i].parentComponentDepth[j];

            for(k = 0; k < gatepinhash[i].connections_size[j]; k++)
            {
                ghash_connection = gatepinhash[i].pinConn[j][k];
                gdepth_connection = gatepinhash[i].pinConnDepth[j][k];

                chash_connection = gatepinhash[ghash_connection].parentComponent[gdepth_connection];
                cdepth_connection = gatepinhash[ghash_connection].parentComponentDepth[gdepth_connection];

                gsl_spmatrix_set(laplacian_matrix, compslocation[chash].value[cdepth], compslocation[chash_connection].value[cdepth_connection], -1);

                gsl_spmatrix_set(laplacian_matrix, compslocation[chash_connection].value[cdepth_connection], compslocation[chash].value[cdepth], -1);

                current_matrix_value = gsl_spmatrix_get(laplacian_matrix, compslocation[chash].value[cdepth], compslocation[chash].value[cdepth]);

                gsl_spmatrix_set(laplacian_matrix, compslocation[chash].value[cdepth], compslocation[chash].value[cdepth], current_matrix_value + 1);

                current_matrix_value = gsl_spmatrix_get(laplacian_matrix, compslocation[chash_connection].value[cdepth_connection], compslocation[chash_connection].value[cdepth_connection]);

                gsl_spmatrix_set(laplacian_matrix, compslocation[chash_connection].value[cdepth_connection], compslocation[chash_connection].value[cdepth_connection], current_matrix_value + 1);
            }

        }
    }

    io_locationx = gsl_vector_alloc(comphash_size - 1);
    io_locationy = gsl_vector_alloc(comphash_size - 1);
    gsl_vector_set_zero(io_locationx);
    gsl_vector_set_zero(io_locationy);

    for(i = 0; i < gatepinhash_size; i++)
    {
        for(j = 0; j < HASHDEPTH; j++)
        {
            if(gatepinhash[i].hashpresent[j] == 0)
            {
                continue;
            }

            if(gatepinhash[i].type[j] == IO_TYPE || gatepinhash[i].type[j] == PO)
            {
                for(k = 0; k < gatepinhash[i].connections_size[j]; k++)
                {
                    ghash_connection = gatepinhash[i].pinConn[j][k];
                    gdepth_connection = gatepinhash[i].pinConnDepth[j][k];

                    chash_connection = gatepinhash[ghash_connection].parentComponent[gdepth_connection];
                    cdepth_connection = gatepinhash[ghash_connection].parentComponentDepth[gdepth_connection];

                    current_matrix_value = gsl_spmatrix_get(laplacian_matrix, compslocation[chash_connection].value[cdepth_connection], compslocation[chash_connection].value[cdepth_connection]);

                    gsl_spmatrix_set(laplacian_matrix, compslocation[chash_connection].value[cdepth_connection], compslocation[chash_connection].value[cdepth_connection], current_matrix_value + 1);

                    // create also array io_locationx //
                    current_matrix_value = gsl_vector_get(io_locationx, compslocation[chash_connection].value[cdepth_connection]);

                    gsl_vector_set(io_locationx, compslocation[chash_connection].value[cdepth_connection], (gatepinhash[i].location_x[j] + current_matrix_value) );

                    // create also array io_locationy //
                    current_matrix_value = gsl_vector_get(io_locationy, compslocation[chash_connection].value[cdepth_connection]);

                    gsl_vector_set(io_locationy, compslocation[chash_connection].value[cdepth_connection], (gatepinhash[i].location_y[j] + current_matrix_value) );
                }   
            }

        }
    }

    printf("\n\nSparse Matrix laplacian:\n");
    gsl_spmatrix_fprintf(stdout, laplacian_matrix, "%g");

    printf("\n\nvector io_locationx:\n");
    gsl_vector_fprintf(stdout, io_locationx, "%g");

    printf("\n\nvector io_locationy:\n");
    gsl_vector_fprintf(stdout, io_locationy, "%g");

    // gsl_spmatrix *laplacian_matrix_csr = gsl_spmatrix_alloc_nzmax(comphash_size - 1, comphash_size - 1, comphash_size - 1, GSL_SPMATRIX_CSR);

    // laplacian_matrix_csr = gsl_spmatrix_compcol(laplacian_matrix);

    // int status;
    // double tol = 1e-7;
    // size_t iter = 0, max_iter = 10000;

    // do 
    // {
    //     status = gsl_splinalg_itersolve_iterate(laplacian_matrix, io_locationx, tol, x, solver);
    //     iter++;

    //     for(i = 0; i < comphash_size - 2; i++)
    //     {
    //         get_component_from_value(i, &chash, &cdepth);
    //         if(cdepth == -1)
    //         {
    //             printf("Error: component not found\n");
    //             exit(1);
    //         }

    //         compslocation[chash].x[cdepth] = gsl_vector_get(x, i);
    //         // compslocation[chash].y[cdepth] = gsl_vector_get(y, i);
    //     }
    // }
    // while (status == GSL_CONTINUE && iter < max_iter);

    // if (status == GSL_SUCCESS) 
    // {
    //     printf("Converged after %zu iterations.\n", iter);
    //     for (size_t i = 0; i < comphash_size - 1; i++) 
    //     {
    //         printf("x_%zu = %g\n", i, gsl_vector_get(x, i));
    //     }
    // } 
    // else 
    // {
    //     printf("x solution Failed to converge. and status is %d, and iter is %zu \n", status, iter);

    //     return;
    // }

    // tol = 1e-7;
    // iter = 0;
    // max_iter = 10000;

    // do 
    // {
    //     status = gsl_splinalg_itersolve_iterate(laplacian_matrix, io_locationy, tol, y, solver);
    //     iter++;
    // }
    // while (status == GSL_CONTINUE && iter < max_iter);

    // if (status == GSL_SUCCESS) 
    // {
    //     printf("Converged after %zu iterations.\n", iter);
    //     for (size_t i = 0; i < comphash_size - 1; i++) 
    //     {
    //         printf("y_%zu = %g\n", i, gsl_vector_get(x, i));
    //     }
    // } 
    // else 
    // {
    //     printf("y solution Failed to converge.\n");

    //     return; 
    // }

    // for(i = 0; i < comphash_size - 2; i++)
    // {
    //     get_component_from_value(i, &chash, &cdepth);
    //     if(cdepth == -1)
    //     {
    //         printf("Error: component not found\n");
    //         exit(1);
    //     }

    //     compslocation[chash].x[cdepth] = gsl_vector_get(x, i);
    //     compslocation[chash].y[cdepth] = gsl_vector_get(y, i);
    // }

    run_parallel_iterations();

    gsl_splinalg_itersolve_free(solver);
    gsl_vector_free(x);
    gsl_vector_free(y);
    gsl_spmatrix_free(laplacian_matrix);
    // gsl_spmatrix_free(laplacian_matrix_csr);
    gsl_vector_free(io_locationx);
    gsl_vector_free(io_locationy);
}


void solve_array_parallel(gsl_spmatrix *matrix, gsl_vector *input, gsl_vector *output) 
{
    gsl_splinalg_itersolve *solver = gsl_splinalg_itersolve_alloc(gsl_splinalg_itersolve_gmres, comphash_size - 1, 0);
    double tol = 1e-7;
    int status;
    size_t iter = 0, max_iter = 10000;

    do
     {
        status = gsl_splinalg_itersolve_iterate(matrix, input, tol, output, solver);
        iter++;
    } 
    while (status == GSL_CONTINUE && iter < max_iter);

    gsl_splinalg_itersolve_free(solver);
}

void run_parallel_iterations() 
{
    int status_x, status_y;
    int i;
    size_t comphash_size_minus_one = comphash_size - 1;
    int chash;
    int cdepth; 

    gsl_vector *x = gsl_vector_alloc(comphash_size - 1);
    gsl_vector *y = gsl_vector_alloc(comphash_size - 1);
    gsl_vector_set_zero(x);
    gsl_vector_set_zero(y);

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            solve_array_parallel(laplacian_matrix, io_locationx, x);
        }

        #pragma omp section
        {
            solve_array_parallel(laplacian_matrix, io_locationy, y);
        }
    }


    for(i = 0; i < comphash_size - 2; i++)
    {
        get_component_from_value(i, &chash, &cdepth);
        if(cdepth == -1)
        {
            printf("Error: component not found\n");
            exit(1);
        }

        compslocation[chash].x[cdepth] = gsl_vector_get(x, i);
        compslocation[chash].y[cdepth] = gsl_vector_get(y, i);
    }
}