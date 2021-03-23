#include "hamming.hpp"

int main(int argc, char** argv){

  //global variables
  int iResult;
  char readbuf [UNCODED_LEN];
  char bits[UNCODED_LEN*BYTE_LEN];
  int recvbuf[UNCODED_LEN];
  int encoded[BYTE_LEN];
  char inbits[UNCODED_LEN];
  char outbits[ENCODED_LEN];
  char encoded_bits[ENCODED_LEN*BYTE_LEN];


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
      get_bits(inbits,outbits,5,5);
      // decoded_to_bits(readbuf,bits,UNCODED_LEN);
      // Hamming::get_bits(readbuf,bits,BYTE_LEN,UNCODED_LEN);
      // Hamming::print_arr(bits,UNCODED_LEN*BYTE_LEN);
      // Hamming::hamming_encoder(bits,encoded);
      // // encoded_to_bits(encoded,encoded_bits,ENCODED_LEN);
      // Hamming::get_bits(encoded,encoded_bits,ENCODED_LEN,BYTE_LEN);
      // Hamming::print_arr(encoded_bits,ENCODED_LEN*BYTE_LEN);
      // Hamming::hamming_decoder(encoded_bits,recvbuf);
      // // recved_to_bits(recvbuf,bits,UNCODED_LEN);
      // Hamming::get_bits(recvbuf,bits,UNCODED_LEN,BYTE_LEN);
      // Hamming::print_arr(bits,UNCODED_LEN*BYTE_LEN);
  }
  return 0;
}





