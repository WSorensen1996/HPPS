#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>

#include "record.h"
#include "id_query.h" 

struct index_record {
    int64_t osm_id ;
    const struct record *record ;
};

struct indexed_data {
    struct index_record *irs;
    int n;
};


int compare (struct record *obj1, struct record *obj2) {
    if (obj1 -> osm_id > obj2 -> osm_id){
        return 1;}
    else if (obj1 -> osm_id < obj2 -> osm_id){
        return -1;}
    else {
        return 0;}
}


int bin_search(struct index_record *irs, int left, int right, int64_t needle){
    if (right >= 1 ){
        int middle = left + (right-left)/2; 
        if(irs[middle].osm_id == needle){
            return middle; 
        }
        else if (irs[middle].osm_id>needle){
            return bin_search(irs, left, middle - 1 , needle); 
        }
        return bin_search(irs, 1 + middle, right, needle);
    }   
    return -1; 
}


struct indexed_data* mk_indexed ( struct record* rs , int n){
  struct indexed_data* data = malloc(sizeof(struct indexed_data)); 
  data -> irs = malloc(n*sizeof(struct index_record)); 
  (data -> n) = n; 
  qsort( rs, n, sizeof(struct record), compare );
  for (int i = 0 ; i < n ; i++){
      (data -> irs[i].record) = &rs[i]; 
      (data -> irs[i].osm_id) = (rs[i].osm_id); 
  }
  return data; 
}

void free_indexed ( struct indexed_data* data ){
    free(data -> irs);
    free(data);
}

const struct record* lookup_indexed(struct indexed_data *data, int64_t needle) {
    int index_res = bin_search(data -> irs, 0, data -> n , needle); 
    if (index_res != -1 ){
        return data -> irs[index_res].record; 
    } 
    else return NULL; 
}

int main(int argc, char** argv) {
  return id_query_loop(argc, argv,(mk_index_fn)mk_indexed,(free_index_fn)free_indexed,(lookup_fn)lookup_indexed);
}


