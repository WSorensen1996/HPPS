#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>

#include "record.h"
#include "id_query.h"

struct naive_data {
  struct record *rs;
  int n;
};



// A pointer to a function that produces an index, when called with an
// array of records and the size of the array.
struct naive_data* mk_naive(struct record* rs, int n) {

  struct naive_data *data = malloc(n* sizeof(struct naive_data)); 
  (data -> rs) = rs; 
  (data -> n) = n; 
  return data; 
}


// Freeing an array produced by a mk_index_fn.
void free_naive(struct naive_data* data) {

  free(data);
}



// Look up an ID in an index produced by mk_index_fn.
const struct record* lookup_naive(struct naive_data *data, int64_t needle) {

  struct record *rs = (data -> rs); 
  for (int i = 0; i < (data -> n) ; i ++){
    if ((rs -> osm_id) == needle){
      return rs;
    }
    rs++; 
  }
  return NULL; 
}



// Run a query loop, using the provided functions for managing the
// index.
int main(int argc, char** argv) {
  return id_query_loop(argc, argv,(mk_index_fn)mk_naive,(free_index_fn)free_naive,(lookup_fn)lookup_naive);
}


