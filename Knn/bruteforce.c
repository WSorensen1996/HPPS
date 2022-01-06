#include "bruteforce.h"
#include "util.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>


// Brute-force k-nearest-neighbours.
// 'k' is the number of neighbours to find.
// 'd' is the number of dimensions in the space.
// 'n' is the number of reference points.
// 'query' is the query point that we are finding neighbours for.
// Returns a freshly allocated 'k'-element array that contains the
// indexes of the nearest neighbours to 'query' in 'points'.  It is
// the responsibility of the caller to free this array.
int* knn(int k, int d, int n, const double *points, const double* query) {
  int *closest;
  closest = (int*) malloc(k*sizeof(int));
  int candidate = 0; 

  //Forloop to run through every candidate 
  for (int i = 0; i < (n+k); i++){

    //Inserting -1 on all empty slots 
    if (i<k) {
      closest[i] = -1; 
    }
    //If no empty slots then run 'insert_if_closer'
    else if (i >= k){
      insert_if_closer(k,d,points, closest ,query, candidate);
      candidate += 1 ; //Next candidate 
      }
    }

  return closest; 
}



