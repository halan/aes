#include "aes.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

// compare 2 arrays for equality
int bytencmp (const unsigned char * const s1, const unsigned char * const s2, const size_t num)
{
    const unsigned char * const us1 = (const unsigned char *) s1;
    const unsigned char * const us2 = (const unsigned char *) s2;
 
    for(size_t i = (size_t)0; i < num; ++i)
    {
        if(us1[i] < us2[i])
           return -1;
        else if(us1[i] > us2[i])
           return 1;
    }
 
    return 0;
}

/**
 * Test cases for multipyling in GF(2 ^ 8)
 */
void test_mul_in_gf(){
  unsigned char p = mul_in_gf(0x53, 0xCA);
  printf("%d\n", p);
  assert(p == 1);

  p = mul_in_gf(0x57, 0x83);
  printf("%d\n", p);
  assert(p == 193);
}

/**
 *
 */
void test_shift_rows(){
  char msg[] = "0123456789ABCDE";
  print_block(msg, 16);

printf("\n");
  shift_rows(msg, 16);
  print_block(msg, 16);

  printf("\n");
  inv_shift_rows(msg, 16);
  print_block(msg, 16);

// TODO: cannot use strcmp because it stops on 0!
// TODO: assert inv is equal to original
  assert( strcmp(msg, "0123456789ABCDE") == 0 );
}

void test_mix_columns(){
  unsigned char block[] = {0x87, 0x6e, 0x46, 0xa6,
                           0xf2, 0x4c, 0xe7, 0x8c,
                           0x4d, 0x90, 0x4a, 0xd8,
                           0x97, 0xec, 0xc3, 0x95, 0x00},
                origblock[17];
  unsigned char expected[] = {0x47, 0x37, 0x94, 0xed,
                            0x40, 0xd4, 0xe4, 0xa5,
                            0xa3, 0x70, 0x3a, 0xa6,
                            0x4c, 0x9f, 0x42, 0xbc, 0x00};
  memcpy(origblock, block, 17);
  mix_columns(block, 16);
  print_block(block, 16);
  printf("\n");
  print_block(expected, 16);
  printf("\n");
  assert( bytencmp(block, expected, 16) == 0 );

  inv_mix_columns(block, 16);
  assert( bytencmp(block, origblock, 16) == 0);
  print_block(block, 16);
  printf("\n");
}

void print_keys(unsigned char block[], int size) {
  for (int i = 0; i < size; i++){
    printf("%02x ", block[i]); 
    if ((i % 16) == 15){ printf("\n"); }
  }
}

void test_key_schedule_128bitkey(){
  unsigned char keys[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  unsigned char expected[] =   
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
      0x62, 0x63, 0x63, 0x63, 0x62, 0x63, 0x63, 0x63, 0x62, 0x63, 0x63, 0x63, 0x62, 0x63, 0x63, 0x63, 
      0x9b, 0x98, 0x98, 0xc9, 0xf9, 0xfb, 0xfb, 0xaa, 0x9b, 0x98, 0x98, 0xc9, 0xf9, 0xfb, 0xfb, 0xaa, 
      0x90, 0x97, 0x34, 0x50, 0x69, 0x6c, 0xcf, 0xfa, 0xf2, 0xf4, 0x57, 0x33, 0x0b, 0x0f, 0xac, 0x99, 
      0xee, 0x06, 0xda, 0x7b, 0x87, 0x6a, 0x15, 0x81, 0x75, 0x9e, 0x42, 0xb2, 0x7e, 0x91, 0xee, 0x2b, 
      0x7f, 0x2e, 0x2b, 0x88, 0xf8, 0x44, 0x3e, 0x09, 0x8d, 0xda, 0x7c, 0xbb, 0xf3, 0x4b, 0x92, 0x90, 
      0xec, 0x61, 0x4b, 0x85, 0x14, 0x25, 0x75, 0x8c, 0x99, 0xff, 0x09, 0x37, 0x6a, 0xb4, 0x9b, 0xa7, 
      0x21, 0x75, 0x17, 0x87, 0x35, 0x50, 0x62, 0x0b, 0xac, 0xaf, 0x6b, 0x3c, 0xc6, 0x1b, 0xf0, 0x9b, 
      0x0e, 0xf9, 0x03, 0x33, 0x3b, 0xa9, 0x61, 0x38, 0x97, 0x06, 0x0a, 0x04, 0x51, 0x1d, 0xfa, 0x9f, 
      0xb1, 0xd4, 0xd8, 0xe2, 0x8a, 0x7d, 0xb9, 0xda, 0x1d, 0x7b, 0xb3, 0xde, 0x4c, 0x66, 0x49, 0x41, 
      0xb4, 0xef, 0x5b, 0xcb, 0x3e, 0x92, 0xe2, 0x11, 0x23, 0xe9, 0x51, 0xcf, 0x6f, 0x8f, 0x18, 0x8e};
  key_schedule(keys, 16);
  print_keys(keys, 176);
  
  printf("\n");
  print_keys(expected, 176);

  assert( bytencmp(keys, expected, 176) == 0 );
}

/**
 * Run all test cases
 */
int main(){
  test_shift_rows();
  test_mul_in_gf();
  test_mix_columns();
  test_key_schedule_128bitkey();

  printf("\nAll tests passed!\n");
  return 0;
}
