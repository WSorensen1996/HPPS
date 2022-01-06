#include <stdlib.h>
#include "numbers.h"

int main() {

  //Testing 'bits8_from_int':
  int bits8_from_int_0 = -45 ; 
  int bits8_from_int_1 = 0   ; 
  int bits8_from_int_2 = 123  ;  
  int bits8_from_int_3 = 310 ; 
  printf("\n \nTesting if:  bits8_from_int(%3d) == ",bits8_from_int_0); 
  bits8_print(bits8_from_int(bits8_from_int_0));
  printf("\nTesting if:  bits8_from_int(%3d) == ",bits8_from_int_1);  
  bits8_print(bits8_from_int(bits8_from_int_1));
  printf("\nTesting if:  bits8_from_int(%3d) == ",bits8_from_int_2);  
  bits8_print(bits8_from_int(bits8_from_int_2));
  printf("\nTesting if:  bits8_from_int(%3d) == ",bits8_from_int_3);  
  bits8_print(bits8_from_int(bits8_from_int_3));
  printf("\n\n");//Spacing

  //Testing 'bits8_to_int':
  int bits8_to_int_0 = -10;  
  int bits8_to_int_1 = 0;
  int bits8_to_int_2 = 32;   
  int bits8_to_int_3 = 310;  
  printf("\nTesting if:  bits8_to_int(bits8_from_int(%3d)) == %3d", bits8_to_int_0, (bits8_to_int(bits8_from_int(bits8_to_int_0))));
  printf("\nTesting if:  bits8_to_int(bits8_from_int(%3d)) == %3d", bits8_to_int_1, (bits8_to_int(bits8_from_int(bits8_to_int_1))));
  printf("\nTesting if:  bits8_to_int(bits8_from_int(%3d)) == %3d", bits8_to_int_2, (bits8_to_int(bits8_from_int(bits8_to_int_2))));
  printf("\nTesting if:  bits8_to_int(bits8_from_int(%3d)) == %3d\n \n", bits8_to_int_3, (bits8_to_int(bits8_from_int(bits8_to_int_3))));

  //Testing 'bits8_add':
  int bits8_add_0 = -50; 
  int bits8_add_1 = 0; 
  int bits8_add_2 = 90; 
  int bits8_add_3 = 240; 
  printf("\nTesting if:  bits8_add(%3d,%3d) == %3d", bits8_add_0,bits8_add_1,bits8_to_int(bits8_add(bits8_from_int(bits8_add_0),bits8_from_int(bits8_add_1))));
  printf("\nTesting if:  bits8_add(%3d,%3d) == %3d", bits8_add_1,bits8_add_1,bits8_to_int(bits8_add(bits8_from_int(bits8_add_1),bits8_from_int(bits8_add_1))));
  printf("\nTesting if:  bits8_add(%3d,%3d) == %3d", bits8_add_0,bits8_add_2,bits8_to_int(bits8_add(bits8_from_int(bits8_add_0),bits8_from_int(bits8_add_2))));
  printf("\nTesting if:  bits8_add(%3d,%3d) == %3d\n \n", bits8_add_2,bits8_add_3,bits8_to_int(bits8_add(bits8_from_int(bits8_add_2),bits8_from_int(bits8_add_3))));

  //Testing 'bits8_negate':
  int bits8_negate_0 = -10; 
  int bits8_negate_1 = 0; 
  int bits8_negate_2 = 20; 
  int bits8_negate_3 = 320; 
  printf("\nTesting if: bits8_negate(");
  bits8_print(bits8_from_int(bits8_negate_0));
  printf(") == ");
  bits8_print(bits8_negate(bits8_from_int(bits8_negate_0)));

  printf("\nTesting if: bits8_negate(");
  bits8_print(bits8_from_int(bits8_negate_1));
  printf(") == ");
  bits8_print(bits8_negate(bits8_from_int(bits8_negate_1)));

  printf("\nTesting if: bits8_negate(");
  bits8_print(bits8_from_int(bits8_negate_2));
  printf(") == ");
  bits8_print(bits8_negate(bits8_from_int(bits8_negate_2)));

  printf("\nTesting if: bits8_negate(");
  bits8_print(bits8_from_int(bits8_negate_3));
  printf(") == ");
  bits8_print(bits8_negate(bits8_from_int(bits8_negate_3)));
  printf("\n\n");//Spacing

  //Testing 'bits8_mul':
  int bits8_mul_0 = -10; 
  int bits8_mul_1 = 5; 
  int bits8_mul_2 = 5; 
  int bits8_mul_3 = 30;  
  printf("\nTesting if: bits8_mul(%3d, %3d) == %3d",bits8_mul_0,bits8_mul_1, bits8_to_int(bits8_mul(bits8_from_int(bits8_mul_0),bits8_from_int(bits8_mul_1))));
  printf("\nTesting if: bits8_mul(%3d, %3d) == %3d",bits8_mul_1,bits8_mul_2, bits8_to_int(bits8_mul(bits8_from_int(bits8_mul_1),bits8_from_int(bits8_mul_2))));
  printf("\nTesting if: bits8_mul(%3d, %3d) == %3d",bits8_mul_2,bits8_mul_3, bits8_to_int(bits8_mul(bits8_from_int(bits8_mul_2),bits8_from_int(bits8_mul_3))));
  printf("\nTesting if: bits8_mul(%3d, %3d) == %3d \n \n",bits8_mul_3,bits8_mul_0, bits8_to_int(bits8_mul(bits8_from_int(bits8_mul_3),bits8_from_int(bits8_mul_0))));

  //Testing subtraction method: 
  printf("Subtracting by the following method: \nBits_add(bit_from_int(20),negate(bit_from_int(5))) = ");
  bits8_print(bits8_add(bits8_from_int(20), bits8_negate(bits8_from_int(5))));
  printf("\nConverted to int equals = %d \n\n", bits8_to_int(bits8_add(bits8_from_int(20), bits8_negate(bits8_from_int(5))) ));

  return 0; 
}
