#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include "util.h"






void OMPmatmul_seq(int n , const double *x ,const double *y, double *out) {
    #pragma omp parallel for
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            double acc = 0;
            for (int l=0; l<n; l++) {
                acc += x[i*n+l] * y[j*n+l] ;
            }
            out[i*n+j] = acc;
            //printf("%f\n", out[i*n+j]); 
            }
        }
    }

void matmul_seq(int n , const double *x ,const double *y, double *out) {
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            double acc = 0;
            for (int l=0; l<n; l++) {
                acc += x[i*n+l] * y[j*n+l] ;
            }
            out[i*n+j] = acc;
            //printf("%f\n", out[i*n+j]); 
            }
        }
    }



/* generate a random floating point number from min to max */
double randfrom(double min, double max) 
{
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
}





int main(){

    int max = 2000 ;


    for (int n=0; n<max; n+=50) {

        double *x = malloc(n*n*sizeof(double));
        double *y = malloc(n*n*sizeof(double));
        double *out = malloc(n*n*sizeof(double));

        for (int i=0; i<n; i++) {
            for (int j=0; j<n; j++) {
                x[i*n+j] = 2;
                y[i*n+j] = 3;
            }

        }

        double OMP_bef = seconds();
        OMPmatmul_seq(n, x, y, out); 
        double OMP_aft = seconds();
        double OMP_matmul_latency = OMP_aft - OMP_bef;

        double bef = seconds();
        matmul_seq(n, x, y, out); 
        double aft = seconds();
        double matmul_latency = aft-bef ;

        printf("for n = %d:  1T=%fs / OMP=%fs     -> Latency =  %fs \n",n,matmul_latency,OMP_matmul_latency, matmul_latency/OMP_matmul_latency);


        free(x);
        free(y);
        free(out);


    }
}

