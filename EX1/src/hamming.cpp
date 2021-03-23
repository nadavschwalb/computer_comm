#define UNCODED_LEN 11
#define ENCODED_LEN 15
#define BYTE 8

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <time.h>

template<typename T>
void decoded_to_bits(T* file_block,char* bits, int nbits);
void encoded_to_bits(int* file_block,char* bits, int nbits);
void parity_encoder(char* inbits, char* outbits);
void parity_decoder(char* inbits, char* outbits);
void reverseArray(char* arr, int arr_len);
void print_arr(char* arr,int arr_len);
int merge_hamming(char* outbits,int nbits);
void hamming_encoder(char* bits,int* encoded);
void hamming_decoder(char* bits,int* decoded);


int main(int argc, char** argv){

  //global variables
  int iResult;
  char readbuf [UNCODED_LEN];
  char bits[UNCODED_LEN*BYTE];
  int recvbuf[UNCODED_LEN];
  int encoded[BYTE];
  char inbits[UNCODED_LEN];
  char outbits[ENCODED_LEN];
  char encoded_bits[ENCODED_LEN*BYTE];


  FILE *fp = fopen("../Dtelem.jpg", "r" );
  fseek(fp,0,SEEK_END);
  printf("file length in bytes: %ld\n",ftell(fp));
  iResult = fseek(fp,0,SEEK_SET);
  if(iResult != 0){
    printf("seek error: %S\n",strerror(iResult));
  }


  int read_count;
  
  while(true){

      read_count = fread(readbuf,sizeof(char),UNCODED_LEN,fp);
      if (read_count!=UNCODED_LEN){
        printf("\n");
          if (0!=feof(fp)){
              printf("end of file reached\n");
              fclose(fp);
              return 0;
          }
          int errnum=ferror(fp);
          printf("Error in reading file: %s\n",strerror(errnum));
          fclose(fp);
          return 1;
      }
      decoded_to_bits(readbuf,bits,UNCODED_LEN);
      printf("uncoded bits: ");
      print_arr(bits,UNCODED_LEN*BYTE);
      hamming_encoder(bits,encoded);
      encoded_to_bits(encoded,encoded_bits,ENCODED_LEN);
      printf("encoded bits: ");
      print_arr(encoded_bits,ENCODED_LEN*BYTE);
      hamming_decoder(encoded_bits,recvbuf);
      decoded_to_bits(recvbuf,bits,UNCODED_LEN);
      printf("decoded bits: ");
      print_arr(bits,UNCODED_LEN*BYTE);
      

  }
  return 0;
}


//---------------------functions-------------------------

template<typename T>
void decoded_to_bits(T* file_block,char* bits, int nbits){
  int bit_index =0;
  for(int i=0;i<nbits;i++){
    for(int j=BYTE-1;j>=0;j--){ //get bits from byte
      *(bits+bit_index) = ((T)file_block[i] &(1 << j)) >> j;
      bit_index++;
    }
  }
}

void encoded_to_bits(int* file_block,char* bits, int nbits){
  int bit_index =0;
  for(int i=0;i<nbits;i++){
    for(int j=2*BYTE-2;j>=0;j--){ //get bits from byte
      *(bits+bit_index) = (file_block[i] &(1 << j)) >> j;
      bit_index++;
    }
  }
}


void parity_encoder(char* inbits, char* outbits){
  //parity bit 1: 2,4,6,8,10,12,14
  //parity bit 2: 2,5,6,9,10,13,14
  //parity bit 4: 4,5,6,11,12,13,14
  //parity bit 8: 8-14

  //reverse inbits
  reverseArray(inbits,UNCODED_LEN);
  //fill outbits with msg
  outbits[2] = inbits[0]; outbits[4] = inbits[1]; outbits[5] = inbits[2]; 
  outbits[6] = inbits[3]; outbits[8] = inbits[4]; outbits[9] = inbits[5];
  outbits[10] = inbits[6]; outbits[11] = inbits[7]; outbits[12] = inbits[8];
  outbits[13] = inbits[9]; outbits[14] = inbits[10];

  //calculate parities
  outbits[0] = outbits[2]^outbits[4]^outbits[6]^outbits[8]^outbits[10]^outbits[12]^outbits[14];
  outbits[1] = outbits[2]^outbits[5]^outbits[6]^outbits[9]^outbits[10]^outbits[13]^outbits[14];
  outbits[3] = outbits[4]^outbits[5]^outbits[6]^outbits[11]^outbits[12]^outbits[13]^outbits[14];
  outbits[7] = outbits[8]^outbits[9]^outbits[10]^outbits[11]^outbits[12]^outbits[13]^outbits[14];

  //reverse output array
  reverseArray(outbits,ENCODED_LEN);
}

void parity_decoder(char* inbits, char* outbits){
  int errnum =0;
  char result =0;
  reverseArray(inbits,ENCODED_LEN);
  //check parities
  result = inbits[2]^inbits[4]^inbits[6]^inbits[8]^inbits[10]^inbits[12]^inbits[14];
  if(result!=inbits[0]){
    errnum+=1;
    printf("error detected: 0 bit\n");
  }
  result = inbits[2]^inbits[5]^inbits[6]^inbits[9]^inbits[10]^inbits[13]^inbits[14];
  if(result!=inbits[1]){
    errnum+=1;
    printf("error detected: 1 bit\n");
  }
  result = inbits[4]^inbits[5]^inbits[6]^inbits[11]^inbits[12]^inbits[13]^inbits[14];
  if(result!=inbits[3]){
    errnum+=1;
    printf("error detected: 3 bit\n");
  }
  result = inbits[8]^inbits[9]^inbits[10]^inbits[11]^inbits[12]^inbits[13]^inbits[14];
  if(result!=inbits[7]){
    errnum+=1;
    printf("error detected: 7 bit\n");
  }
  printf("number of errors: %d\n",errnum);
  //fill outbits with decoded message
  outbits[0] = inbits[2]; outbits[1] = inbits[4]; outbits[2] = inbits[5]; 
  outbits[3] = inbits[6]; outbits[4] = inbits[8]; outbits[5] = inbits[9];
  outbits[6] = inbits[10]; outbits[7] = inbits[11]; outbits[8] = inbits[12];
  outbits[9] = inbits[13]; outbits[10] = inbits[14];
  reverseArray(outbits,UNCODED_LEN);
}

int merge_hamming(char* outbits,int nbits){
  reverseArray(outbits,nbits);
  int result = 0;
  for(int i=0;i<nbits;i++){
    result += outbits[i]<<i;
  }
  return result;
}



void hamming_encoder(char* bits,int* encoded){
  printf("encoder in: ");
  print_arr(bits,UNCODED_LEN*BYTE);
  char inbits[UNCODED_LEN];
  char outbits[ENCODED_LEN];
  printf("encoder out: ");
  for(int i=0;i<BYTE;i++){
    //create sub array of 11 bits
    for(int j=0;j<UNCODED_LEN;j++){
      inbits[j] = bits[i*UNCODED_LEN +j];
    }
    //encode inbits into outbits array
    parity_encoder(inbits,outbits);
    print_arr(outbits,ENCODED_LEN);
    //merge encoded bits into int array
    encoded[i] = merge_hamming(outbits,ENCODED_LEN);
  }
}

void hamming_decoder(char* bits,int* decoded){
  printf("decoder in: ");
  print_arr(bits,ENCODED_LEN*BYTE);
  char inbits[ENCODED_LEN];
  char outbits[UNCODED_LEN];
  printf("decoder out: ");
  for(int i=0;i<BYTE;i++){
    //create sub array of 15 bits
    for(int j=0;j<ENCODED_LEN;j++){
      inbits[j] = bits[i*ENCODED_LEN +j];
    }
    //encode inbits into outbits array
    parity_decoder(inbits,outbits);
    print_arr(outbits,UNCODED_LEN);
    //merge encoded bits into int array
    decoded[i] = merge_hamming(outbits,UNCODED_LEN);
  }
}


void reverseArray(char* arr, int arr_len){
  int start = 0;
  int end = arr_len -1;
  while (start < end){
    int temp = arr[start]; 
    arr[start] = arr[end];
    arr[end] = temp;
    start++;
    end--;
  } 
}   

void print_arr(char* arr,int arr_len){
  for(int i=0;i<arr_len;i++){
    printf("%d",arr[i]);
  }
  printf("\n");
}



