#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <stdint.h>
#include <errno.h>
#include <assert.h>
#include <math.h>

#include "record.h"
#include "coord_query.h"

struct naive_data {
  struct record *rs;
  int n;
};

struct naive_data* mk_naive(struct record* rs, int n) {
  struct naive_data *data = malloc(n* sizeof(struct naive_data)); 
  (data -> rs) = rs; 
  (data -> n) = n; 
  return data; 
}

void free_naive(struct naive_data* data) {
  free(data);
}



double distance(double x1, double y1,double x2 ,double y2) {
  double dist = pow((x2-x1),2) + pow((y2-y1),2) ; 
  return sqrt(dist); 
}


const struct record* lookup_naive(struct naive_data *data, double lon, double lat) {
  struct record closests = *(data -> rs); 
  struct record *rs = (data -> rs); 
  for (int i = 0; i < (data -> n) ; i ++){
    double new_dist = distance(lon,lat, (rs -> lon), (rs -> lat)); 
    double old_dist = distance(lon, lat, (closests.lon) , (closests.lat));
    if  (new_dist < old_dist){ 
      closests = *rs; 
    }
    rs++; 
  }
  return &closests; 

}


int main(int argc, char** argv) {
  return coord_query_loop(argc, argv, (mk_index_fn)mk_naive, (free_index_fn)free_naive, (lookup_fn)lookup_naive);

}



