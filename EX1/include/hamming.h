#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>

#define UNCODED_LEN 11
#define ENCODED_LEN 15
#define BYTE 8

namespace Hamming
{
  template<typename T>
  void get_bits(T* block,char* bits, int nbits, int nelem);
  void parity_encoder(char* inbits, char* outbits);
  int parity_decoder(char* inbits, char* outbits);
  void reverseArray(char* arr, int arr_len);
  template<typename T>
  void print_arr(T* arr,int arr_len);
  int merge_hamming(char* outbits,int nbits);
  void hamming_encoder(char* bits,int* encoded);
  void hamming_decoder(char* bits,int* decoded);
} // namespace Hamming

