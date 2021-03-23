#pragma once
#define UNCODED_LEN 11
#define ENCODED_LEN 15
#define BYTE_LEN 8

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>

namespace Hamming
{
  void parity_encoder(char* inbits, char* outbits);
  void parity_decoder(char* inbits, char* outbits);
  void reverseArray(char* arr, int arr_len);
  void print_arr(char* arr,int arr_len);
  int merge_hamming(char* outbits,int nbits);
  void hamming_encoder(char* bits,int* encoded);
  void hamming_decoder(char* bits,int* decoded);

  template<typename T>
  void get_bits(T* block, char* bits, int nbits, int nelem){
    int bit_index =0;
    for(int i=0;i<nelem;i++){
      for(int j=nbits-1;j>=0;j--){ //get bits from byte
        *(bits+bit_index) = (block[i] &(1 << j)) >> j;
        bit_index++;
      }
    }
  }
} // namespace Hamming




