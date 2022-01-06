#include <stdint.h>
#include<stdio.h>  
#include "bits.h"

struct bits8 {
  struct bit b0; // Least significant bit
  struct bit b1;
  struct bit b2;
  struct bit b3;
  struct bit b4;
  struct bit b5;
  struct bit b6;
  struct bit b7; // most significant bit 
};

unsigned int getbit ( unsigned int x, int i){
  return ((x >> i) & 1);
}

struct bits8 bits8_from_int(unsigned int x){
  struct bits8 bits; 
  bits.b7.v = getbit (x,7);
  bits.b6.v = getbit (x,6);
  bits.b5.v = getbit (x,5);
  bits.b4.v = getbit (x,4);
  bits.b3.v = getbit (x,3);
  bits.b2.v = getbit (x,2);
  bits.b1.v = getbit (x,1);
  bits.b0.v = getbit (x,0);
  return bits; 
}

void bits8_print(struct bits8 v){
  printf("%d%d%d%d%d%d%d%d",v.b7.v,v.b6.v,v.b5.v,v.b4.v,v.b3.v,v.b2.v,v.b1.v,v.b0.v);
}

unsigned int set_bit ( unsigned int x, int i){
  int n = ((1 << i) | x) ; 
  return n; 
}

unsigned int bits8_to_int(struct bits8 x){
  return x.b0.v | x.b1.v<<1 | x.b2.v<<2 | x.b3.v<<3 
  | x.b4.v<<4 |x.b5.v<<5 |x.b6.v<<6 |x.b7.v<<7  ; 
} 

struct bits8 bits8_add(struct bits8 x, struct bits8 y){
  struct bits8 s;  //Result saved 
  struct bits8 c;  //carry  

  //Using the carry function as given: ci = (x & y) | ((x | y) & c) 
  //Saving all the carry values in c
  c.b0 = bit_or(bit_and(x.b0 , y.b0) , bit_and(bit_or(x.b0 , y.b0 ) , bit_from_int(0))); 
  c.b1 = bit_or(bit_and(x.b1 , y.b1) , bit_and(bit_or(x.b1 , y.b1 ) , c.b0));
  c.b2 = bit_or(bit_and(x.b2 , y.b2) , bit_and(bit_or(x.b2 , y.b2 ) , c.b1));
  c.b3 = bit_or(bit_and(x.b3 , y.b3) , bit_and(bit_or(x.b3 , y.b3 ) , c.b2));
  c.b4 = bit_or(bit_and(x.b4 , y.b4) , bit_and(bit_or(x.b4 , y.b4 ) , c.b3));
  c.b5 = bit_or(bit_and(x.b5 , y.b5) , bit_and(bit_or(x.b5 , y.b5 ) , c.b4));
  c.b6 = bit_or(bit_and(x.b6 , y.b6) , bit_and(bit_or(x.b6 , y.b6 ) , c.b5));
  c.b7 = bit_or(bit_and(x.b7 , y.b7) , bit_and(bit_or(x.b7 , y.b7 ) , c.b6));

  //Using the formula for result:  si = xi  yi  ci􀀀1 
  s.b0 = bit_xor(bit_xor(x.b0 ,y.b0) ,bit_from_int(0));
  s.b1 = bit_xor(bit_xor(x.b1 ,y.b1) ,c.b0);
  s.b2 = bit_xor(bit_xor(x.b2 ,y.b2) ,c.b1);
  s.b3 = bit_xor(bit_xor(x.b3 ,y.b3) ,c.b2);
  s.b4 = bit_xor(bit_xor(x.b4 ,y.b4) ,c.b3);
  s.b5 = bit_xor(bit_xor(x.b5 ,y.b5) ,c.b4);
  s.b6 = bit_xor(bit_xor(x.b6 ,y.b6) ,c.b5);
  s.b7 = bit_xor(bit_xor(x.b7 ,y.b7) ,c.b6);
  return s; 
}

struct bits8 bits8_negate(struct bits8 x){
  struct bits8 s; 
  s.b0 = bit_not(x.b0) ; 
  s.b1 = bit_not(x.b1) ;
  s.b2 = bit_not(x.b2) ;
  s.b3 = bit_not(x.b3) ;
  s.b4 = bit_not(x.b4) ;
  s.b5 = bit_not(x.b5) ;
  s.b6 = bit_not(x.b6) ;
  s.b7 = bit_not(x.b7) ;
  s = bits8_add(s,bits8_from_int(1));
  return s; 
}

struct bits8 bits8_bit_and(struct bits8 x, struct bit y) {
  struct bits8 z; 
  z.b0 = bit_and(x.b0,y);
  z.b1 = bit_and(x.b1,y);
  z.b2 = bit_and(x.b2,y);
  z.b3 = bit_and(x.b3,y);
  z.b4 = bit_and(x.b4,y);
  z.b5 = bit_and(x.b5,y);
  z.b6 = bit_and(x.b6,y);
  z.b7 = bit_and(x.b7,y);
  return z; 
}

struct bits8 bits8_mul(struct bits8 x, struct bits8 y){
  struct bits8 z;   //
  struct bits8 s;   //Saving value 

  s = bits8_bit_and(x, y.b0);
  z = bits8_bit_and(x, y.b1);

  z = bits8_from_int(bits8_to_int(z)<<1);
  s = bits8_add(z,s);
 
  z = bits8_bit_and(x, y.b2);
  z = bits8_from_int(bits8_to_int(z)<<2);
  s = bits8_add(z,s);

  z = bits8_bit_and(x, y.b3);
  z = bits8_from_int(bits8_to_int(z)<<3);
  s = bits8_add(z,s);

  z = bits8_bit_and(x, y.b4);
  z = bits8_from_int(bits8_to_int(z)<<4);
  s = bits8_add(z,s);

  z = bits8_bit_and(x, y.b5);
  z = bits8_from_int(bits8_to_int(z)<<5);
  s = bits8_add(z,s);

  z = bits8_bit_and(x, y.b6);
  z = bits8_from_int(bits8_to_int(z)<<6);
  s = bits8_add(z,s);

  z = bits8_bit_and(x, y.b7);
  z = bits8_from_int(bits8_to_int(z)<<7);
  s = bits8_add(z,s);

  return s; 
}
