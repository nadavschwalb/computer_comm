#include "hamming.hpp"

using namespace Hamming;
//---------------------functions-------------------------

void Hamming::encode(char* readbuf,char* encodedbuf){
  char inbits[UNCODED_LEN*BYTE_LEN];
  char outbits[ENCODED_LEN*BYTE_LEN];
  get_bits<char>(readbuf,inbits,BYTE_LEN,UNCODED_LEN);
  // printf("uncoded bits: ");
  // print_arr(inbits,UNCODED_LEN*BYTE_LEN);
  hamming_encoder(inbits,outbits);
  // printf("encoded bits: ");
  // print_arr(outbits,ENCODED_LEN*BYTE_LEN);
  merge(outbits,encodedbuf,ENCODED_LEN);
}

void Hamming::decode(char* encoded,char* filebuf){
  char inbits[ENCODED_LEN*BYTE_LEN];
  char outbits[UNCODED_LEN*BYTE_LEN];
  get_bits<char>(encoded,inbits,BYTE_LEN,ENCODED_LEN);
  // printf("encoded bits: ");
  // print_arr(inbits,ENCODED_LEN*BYTE_LEN);
  hamming_decoder(inbits,outbits);
  // printf("decoded bits: ");
  // print_arr(outbits,UNCODED_LEN*BYTE_LEN);
  merge(outbits,filebuf,UNCODED_LEN);
}

void Hamming::parity_encoder(char* inbits, char* outbits){
  //parity bit 1: 2,4,6,8,10,12,14
  //parity bit 2: 2,5,6,9,10,13,14
  //parity bit 4: 4,5,6,11,12,13,14
  //parity bit 8: 8-14

  //reverse inbits
  //reverseArray(inbits,UNCODED_LEN);
  //print_arr(inbits,UNCODED_LEN);
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
  //reverseArray(outbits,ENCODED_LEN);
}

void Hamming::parity_decoder(char* inbits, char* outbits){
  int errnum =0;
  char result =0;
  //reverseArray(inbits,ENCODED_LEN);
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
  //fill outbits with decoded message
  outbits[0] = inbits[2]; outbits[1] = inbits[4]; outbits[2] = inbits[5]; 
  outbits[3] = inbits[6]; outbits[4] = inbits[8]; outbits[5] = inbits[9];
  outbits[6] = inbits[10]; outbits[7] = inbits[11]; outbits[8] = inbits[12];
  outbits[9] = inbits[13]; outbits[10] = inbits[14];
  //reverseArray(outbits,UNCODED_LEN);
}

void Hamming::merge(char* inbits, char* outarr, int nbits){
  memset(outarr,0,nbits);
  for(int i=0;i<nbits;i++){
    reverseArray(inbits+i*BYTE_LEN,BYTE_LEN);
    for(int j=0;j<BYTE_LEN;j++){
      outarr[i]+=inbits[j+i*BYTE_LEN]<<j; 
      //printf("%d",inbits[j+i*BYTE_LEN]);
    }
  }
}

void Hamming::hamming_encoder(char* bits,char* encoded){
  char inbits[UNCODED_LEN];
  char outbits[ENCODED_LEN];
  for(int i=0;i<BYTE_LEN;i++){
    //create sub array of 11 bits
    for(int j=0;j<UNCODED_LEN;j++){
      inbits[j] = bits[i*UNCODED_LEN +j];
    }
    //print_arr(inbits,UNCODED_LEN);
    //encode inbits into outbits array
    parity_encoder(inbits,outbits);
    //print_arr(outbits,ENCODED_LEN);
    std::copy(outbits,outbits+ENCODED_LEN,encoded + i*ENCODED_LEN);
  }
}

void Hamming::hamming_decoder(char* bits,char* decoded){
  char inbits[ENCODED_LEN];
  char outbits[UNCODED_LEN];
  //printf("decoder out: ");
  for(int i=0;i<BYTE_LEN;i++){
    //create sub array of 15 bits
    for(int j=0;j<ENCODED_LEN;j++){
      inbits[j] = bits[i*ENCODED_LEN +j];
    }
    //encode inbits into outbits array
    parity_decoder(inbits,outbits);
    //merge encoded bits into int array
    std::copy(outbits,outbits+UNCODED_LEN,decoded + i*UNCODED_LEN);
  }
} 

void Hamming::reverseArray(char* arr, int arr_len){
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

void Hamming::print_arr(char* arr,int arr_len){
  // for(int i=0;i<arr_len/BYTE_LEN;i++){
  //   for(int j=0;j<BYTE_LEN;j++){
  //     printf("%d",arr[j+i*BYTE_LEN]);
  //   }
  //    printf(" ");
  // }

  for(int i=0;i<arr_len;i++){
    printf("%d",arr[i]);
  }
  printf("\n");
}

void Hamming::diff_arr(char* src, char* diff, int arr_len){
  for(int i=0;i<arr_len;i++){
    if(src[i]!=diff[i]) printf("%d",src[i]-diff[i]);
    else printf(" ");
  }
  printf("\n");
}
