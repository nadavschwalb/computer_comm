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


void file_to_bits(char* file_block,char* bits);
void parity_solver(char* inbits, char* outbits);
void reverseArray(char* arr, int arr_len);
void print_arr(char* arr,int arr_len);
int merge_hamming(char* outbits);
void hamming_encoder(char* bits,int* encoded);

int main(int argc, char** argv){
  int iResult;
  char readbuf [UNCODED_LEN];
  char bits[UNCODED_LEN*BYTE];
  int encoded[BYTE];
  char inbits[UNCODED_LEN];
  char outbits[ENCODED_LEN];
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
      file_to_bits(readbuf,bits);
      hamming_encoder(bits,encoded);


  }
  return 0;
}

void file_to_bits(char* file_block,char* bits){
  int bit_index =0;
  for(int i=0;i<UNCODED_LEN;i++){
    for(int j=BYTE-1;j>=0;j--){ //get bits from byte
      *(bits+bit_index) = (file_block[i] &(1 << j)) >> j;
      bit_index++;
    }
  }
}

void parity_solver(char* inbits, char* outbits){
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

int merge_hamming(char* outbits){
  reverseArray(outbits,ENCODED_LEN);
  int result = 0;
  for(int i=0;i<ENCODED_LEN;i++){
    result += outbits[i]<<i;
  }
  return result;
}

void hamming_encoder(char* bits,int* encoded){
  char inbits[UNCODED_LEN];
  char outbits[ENCODED_LEN];
  for(int i=0;i<BYTE;i++){
    //create sub array of 11 bits
    for(int j=0;j<UNCODED_LEN;j++){
      inbits[j] = bits[i*UNCODED_LEN +j];
    }
    //encode inbits into outbits array
    parity_solver(inbits,outbits);
    //merge encoded bits into int array
    encoded[i] = merge_hamming(outbits);
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



