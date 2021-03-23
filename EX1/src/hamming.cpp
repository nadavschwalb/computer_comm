#include "hamming.hpp"

//---------------------functions-------------------------

template<typename T>
void get_bits(T* block,char* bits, int nbits, int nelem){
    int bit_index =0;
  for(int i=0;i<nelem;i++){
    for(int j=nbits-1;j>=0;j--){ //get bits from byte
      *(bits+bit_index) = (block[i] &(1 << j)) >> j;
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

int parity_decoder(char* inbits, char* outbits){
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
  //fill outbits with decoded message
  outbits[0] = inbits[2]; outbits[1] = inbits[4]; outbits[2] = inbits[5]; 
  outbits[3] = inbits[6]; outbits[4] = inbits[8]; outbits[5] = inbits[9];
  outbits[6] = inbits[10]; outbits[7] = inbits[11]; outbits[8] = inbits[12];
  outbits[9] = inbits[13]; outbits[10] = inbits[14];
  reverseArray(outbits,UNCODED_LEN);

  return errnum;
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
  char inbits[UNCODED_LEN];
  char outbits[ENCODED_LEN];
  for(int i=0;i<BYTE_LEN;i++){
    //create sub array of 11 bits
    for(int j=0;j<UNCODED_LEN;j++){
      inbits[j] = bits[i*UNCODED_LEN +j];
    }
    //encode inbits into outbits array
    parity_encoder(inbits,outbits);
    //merge encoded bits into int array
    encoded[i] = merge_hamming(outbits,ENCODED_LEN);
  }
}

void hamming_decoder(char* bits,int* decoded){
  char inbits[ENCODED_LEN];
  char outbits[UNCODED_LEN];
  for(int i=0;i<BYTE_LEN;i++){
    //create sub array of 15 bits
    for(int j=0;j<ENCODED_LEN;j++){
      inbits[j] = bits[i*ENCODED_LEN +j];
    }
    //encode inbits into outbits array
    parity_decoder(inbits,outbits);
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

template<typename T>
void print_arr(T* arr,int arr_len){
  for(int i=0;i<arr_len;i++){
    printf("%d",arr[i]);
  }
  printf("\n");
}
