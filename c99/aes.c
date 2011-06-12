/**
 * Implemenation of the AES encryption algorithm
 *
 * TODO: only supports AES-128; add support for 192 and 256 bit block lengths.
 *
 * Copyright (c) 2011 Justin Ethier
 *
 */
#include "aes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const unsigned char sbox[] = 
   // 0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
   {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76, //0
    0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0, //1
    0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15, //2
    0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75, //3
    0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84, //4
    0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf, //5
    0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8, //6
    0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2, //7
    0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73, //8
    0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb, //9
    0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79, //A
    0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08, //B
    0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a, //C
    0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e, //D
    0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf, //E
    0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16}; //F
 
const unsigned char sbox_inverse[] = 
   {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
    0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
    0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
    0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
    0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
    0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
    0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
    0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
    0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
    0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
    0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
    0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
    0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
    0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
    0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
    0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d};

/* Values used for key schedule */
const unsigned char rcon[] = 
   {0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
    0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
    0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 
    0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 
    0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 
    0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 
    0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 
    0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 
    0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 
    0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 
    0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 
    0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 
    0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 
    0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 
    0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 
    0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb};

/**
Encrypt data using given key

@param  unsigned char *     plain_text  Plaintext message to encrypt
@param  unsigned char *     key         Encryption key
@param  int                 size        Size of the encryption key, in bytes
@return unsigned char *     Dynamically allocated array of encrypted data
 */
unsigned char *encrypt(unsigned char *plain_text, unsigned char *key, int size){
    unsigned char *keys = (unsigned char *) malloc(size * 11),
                  *cipher_text = (unsigned char *) malloc(size); // TODO: may be cleaner to just pass in cipher/plain array, and destructively modify it
    int round = 0;

    memcpy(keys, key, size); // First key is the input one...
    memcpy(cipher_text, plain_text, size);

    key_schedule(keys, size); // Create round keys
    add_round_key(cipher_text, size, keys, round);
//print_block(key, 16);
//print_block(cipher_text, 16);
    for (round = 1; round < 11; round++){
        sub_bytes(cipher_text, size);
        shift_rows(cipher_text, size);
        if (round < 10) { mix_columns(cipher_text, size); }
        add_round_key(cipher_text, size, keys, round);
    }      
    free(keys);
    return cipher_text;
}

unsigned char *decrypt(unsigned char *cipher_text, unsigned char *key, int size){
    unsigned char *keys = (unsigned char *) malloc(size * 11),
                  *plain_text = (unsigned char *) malloc(size); // TODO: may be cleaner to just pass in cipher/plain array, and destructively modify it
    int round = 10;

    memcpy(keys, key, size); // First key is the input one...
    memcpy(plain_text, cipher_text, size);

    key_schedule(keys, size); // Create round keys
    add_round_key(plain_text, size, keys, round);

    for (round = 9; round >= 0; round--){
        inv_shift_rows(plain_text, size);
        inv_sub_bytes(plain_text, size);
        add_round_key(plain_text, size, keys, round);
        if (round > 0) {
            inv_mix_columns(plain_text, size); 
        }
    }      
    free(keys);
    return plain_text;
}
/**
 * Print a block of bytes
 * @param block Array of bytes
 * @param size  Size of array, in bytes
 */ 
void print_block(unsigned char block[], int size) {
  for (int i = 0; i < size; i++){
    printf("%02x ", block[i]); 
    if ((i % 4) == 3){ printf("\n"); }
  }
}

/**
 * A non-linear substitution step where each byte is replaced with another according to a lookup table.
 */
void do_sub_bytes(unsigned char block[], int size, int inverse){
  for (int i = 0; i < size; i++){
    if(inverse)
      block[i] = sbox_inverse[block[i]];
    else
      block[i] = sbox[block[i]];
  }
}

void sub_bytes(unsigned char block[], int size){
  do_sub_bytes(block, size, 0);
}

void inv_sub_bytes(unsigned char block[], int size){
  do_sub_bytes(block, size, 1);
}


/**
 * A transposition step where each row of the state is shifted cyclically a certain number of steps.
 * 
 */
void shift_rows(unsigned char block[], int size){
    int col_size = size / 4;
    int row_size = size / col_size;
    unsigned char temp;

    for (int col = 1; col < col_size; col++){
      for (int i = 0; i < col; i++){
      for (int row = 1; row < row_size; row++){
        int ci = col * col_size; // col index
        int ni = (row + 1) % row_size ; // next index
        temp = block[ci + row];
        block[ci + row] = block[ci + ni];
        block[ci + ni] = temp;
      }
      }
    }
}

void inv_shift_rows(unsigned char block[], int size){
    int col_size = size / 4;
    int row_size = size / col_size;
    unsigned char temp;

    for (int col = 1; col < col_size; col++){
      for (int i = 0; i < col; i++){
      for (int row = row_size-1; row > 0; row--){
        int ci = col * col_size; // col index
        int ni = (row - 1) % row_size ; 
        temp = block[ci + row];
        block[ci + row] = block[ci + ni];
        block[ci + ni] = temp;
      }
      }
    }
}

 /** 
  * A mixing operation which operates on the columns of the state, combining the four bytes in each column
  * see: http://en.wikipedia.org/wiki/Rijndael_mix_columns
  */
void mix_columns(unsigned char block[], int size){  
  int row_size = 4; // #@block_size / (8 * 4) 
  for (int col = 0; col < row_size; col++){
    unsigned char ci = row_size * col; // column index
    unsigned char mul2[4];
    unsigned char high_bit;
    for (int m = 0; m < 4; m++){
        mul2[m] = block[ci + m];
        high_bit = mul2[m] & 0x80;
        mul2[m] <<= 1;
        if (high_bit == 0x80){ mul2[m] ^= 0x1b; }
    }

    unsigned char r[4];
    r[0] = mul2[0] ^ block[3 + ci] ^ block[2 + ci] ^ mul2[1] ^ block[1 + ci];
    r[1] = mul2[1] ^ block[0 + ci] ^ block[3 + ci] ^ mul2[2] ^ block[2 + ci];
    r[2] = mul2[2] ^ block[1 + ci] ^ block[0 + ci] ^ mul2[3] ^ block[3 + ci];
    r[3] = mul2[3] ^ block[2 + ci] ^ block[1 + ci] ^ mul2[0] ^ block[0 + ci];

    for (int i = 0; i < 4; i++){
      block[ci + i] = r[i];  
    }
  }
}


/**
 *
 */
void inv_mix_columns(unsigned char block[], int size){
  int row_size = 4; //@block_size / (8 * 4) 
  for (int col = 0; col < row_size; col++){
    unsigned char ci = row_size * col;
    unsigned char r[4];
    r[0] = mul_in_gf(block[ci + 0], 0xe) ^
           mul_in_gf(block[ci + 1], 0xb) ^
           mul_in_gf(block[ci + 2], 0xd) ^
           mul_in_gf(block[ci + 3], 0x9);
    r[1] = mul_in_gf(block[ci + 0], 0x9) ^
           mul_in_gf(block[ci + 1], 0xe) ^
           mul_in_gf(block[ci + 2], 0xb) ^
           mul_in_gf(block[ci + 3], 0xd);
    r[2] = mul_in_gf(block[ci + 0], 0xd) ^
           mul_in_gf(block[ci + 1], 0x9) ^
           mul_in_gf(block[ci + 2], 0xe) ^
           mul_in_gf(block[ci + 3], 0xb);
    r[3] = mul_in_gf(block[ci + 0], 0xb) ^
           mul_in_gf(block[ci + 1], 0xd) ^
           mul_in_gf(block[ci + 2], 0x9) ^
           mul_in_gf(block[ci + 3], 0xe);

    for(int i = 0; i < 4; i++){
      block[ci + i] = r[i];  
    }
  }
}


/**
 * Multiply numbers in GF(2 ^ 8)
 *
 * @param  a
 * @param  b
 * @return Product of a and b
 */
unsigned char mul_in_gf(unsigned char a, unsigned char b){
  unsigned char p = 0, hbit; // High bit
  for (int i = 0; i < 8; i++){
    hbit = a & 0x80;

    if ((b >> i) & 0x01 == 0x01){
       p ^= a; // Polynomial addition in GF
    }
    
    a <<= 1;
    if (hbit == 0x80){ // High bit set
      a ^= 0x1b;
    }
  }
  return p;
}

/**
 * Each byte of the state is combined with the round key; 
 * each round key is derived from the cipher key using a key schedule.
 *
 * Inputs: data block array, key schedule array, round number
 */
void add_round_key(unsigned char block[], int size, unsigned char key_schedule[], int round){
  for (int i = 0; i < size; i++){
    block[i] = (block[i] ^ key_schedule[round * 16 + i]);  
  }  
}

/**
 * 
 * Input: AES Key (an array of bytes)
 *   actually, this array needs to be large enough to hold b bytes of expanded
 *   key, including the first n bytes which are the AES key.
 * Output: Array of Keys, one for each round. Output is to the same array
 *
 */ 
void key_schedule(unsigned char *keys, int size){
// TODO: generalize this to support bit lengths other than 128
//       EG: b has a value of 176 for 128-bit keys, 208 for 192-bit keys, and 240 for 256-bit keys 
//       see: http://en.wikipedia.org/wiki/Rijndael_key_schedule

    int i = 1; //, k=size;
    for (int bKey = 1; bKey < 11; bKey++){
      unsigned char t[4] = {keys[bKey*size - 4], keys[bKey*size - 3], keys[bKey*size - 2], keys[bKey*size - 1]};
      key_schedule_core(t, i);
      i += 1;
      for(int idx = 0; idx < 4; idx++){
        keys[bKey*(size) + idx] = (t[idx] ^ keys[bKey*size - 16 + idx]);
      }

      for (int c = 1; c < 4; c++){
        int index = 4 * c;
        unsigned char t[4] = {keys[bKey*size - 4 + index],
                              keys[bKey*size - 3 + index], 
                              keys[bKey*size - 2 + index], 
                              keys[bKey*size - 1 + index]};
        for (int cc = 0; cc < 4; cc++){
          keys[bKey*(size) + cc + index] = (t[cc] ^ keys[bKey*size - 16 + cc + index]);
        }
      }
    }
}

/**
 * Core component of the key schedule
 *
 * @param word   32-bit word (array of 4 bytes)
 * @param i     iteration number
 *
 */
void key_schedule_core(unsigned char word[], int i) {
  // Copy the output over to the input. 
  // Rotate operation to rotate the output eight bits to the left 
  unsigned char tmp = word[0];
  int idx;
  for(idx = 1; idx < 4; idx++){
    word[idx-1] = word[idx];
  }
  word[3] = tmp;

  // Apply Rijndael's S-box on all four individual bytes in the output word 
  for (idx = 0; idx < 4; idx++){
    word[idx] = sbox[ word[idx] ];
  }

  // On just the first (leftmost) byte of the output word, exclusive or
  // the byte with 2 to the power of (i-1). 
  // In other words, perform the rcon operation with i as the input, 
  // and exclusive or the rcon output with the first byte of the output word 
  word[0] ^= rcon[i];
}

