#include <stdio.h>
#include <math.h>
#include "../structs/structs.h"

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_spmatrix.h>
#include <gsl/gsl_spblas.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_splinalg.h>
#include <gsl/gsl_vector.h>

extern gsl_spmatrix *array_A;
extern gsl_spmatrix *array_IO;
extern gsl_spmatrix *array_diag;
extern gsl_spmatrix *laplacian_matrix;
extern gsl_spmatrix *io_locationx;

extern int IOs_size;

double calculate_HPWL();
double random_double(double, double);
void random_placer();
void calculate_hpwl_new(double *, double *, double *);
void create_array_A();
void calculate_degree_matrix(int IO_size);
void spmatrix_sub();
void create_pin_vectors();
void solve_linear_system();

