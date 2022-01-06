#include <math.h>
#include <stdio.h>
#include <assert.h>


double distance(int d, const double *x, const double *y) {
  double dist = 0 ; 
  for (int i = 0; i < d; i++){
    double  temp = pow((x[i]-y[i]),2);
    dist = temp + dist ; 
  }
  return sqrt(dist); 
}

int insert_if_closer(int k, int d, const double *points, int *closest, const double *query, int candidate) {

  //Forloop to compare dist. to every new point to existing point in closests
  for (int i = 0; i < k; i++){
    double dist_point_query = distance(d,&points[candidate*d],query);//Dist. between point and query
    double dist_point_candidate = distance(d,&points[closest[i]*d], query);// Dist. between every element in closests to new point 

    // If existing value in closests is -1 then update 
    if (closest[i] == -1){
      closest[i] = candidate ;
      return 1 ;                 
    }

    //If new value needs to be inserted
    else if (dist_point_query < dist_point_candidate){

      //Shifts all elements backwards
      for(int j=k-1;j>=i;j--){
        closest[j]=closest[j-1];
      }
      //Updates closest
      closest[i] = candidate;
      return 1; 
    }
    //If it is bigger || equal to existing value do: Nothing
    else if (dist_point_query >= closest[i]){ }
    
  }
  return 0; 
}















