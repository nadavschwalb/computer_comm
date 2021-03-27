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
  int read_msg(FILE* fp, char* msg);
  int write_msg(FILE* fp, char* msg, char* decoded_msg);
  void encode(char* readbuf,char* writebuf);
  void decode(char* encoded,char* filebuf);
  void parity_encoder(char* inbits, char* outbits);
  void parity_decoder(char* inbits, char* outbits);
  void reverseArray(char* arr, int arr_len);
  void print_arr(char* arr,int arr_len);
  void print_arr_nl(char* arr, int arr_len);
  void diff_arr(char* src, char* diff, int arr_len);
  void merge(char* inbits, char* outarr, int nbits);
  void hamming_encoder(char* bits,char* encoded);
  void hamming_decoder(char* bits,char* decoded);

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




