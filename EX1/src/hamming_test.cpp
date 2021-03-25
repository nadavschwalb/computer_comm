#include "hamming.hpp"
using namespace Hamming;
int main(int argc, char** argv){

  //global variables
  int iResult;
  char readbuf [UNCODED_LEN];
  char recvbuf [UNCODED_LEN];
  char encodedbuf[ENCODED_LEN];
  char bits[UNCODED_LEN*BYTE_LEN];
  int encoded[BYTE_LEN];
  char encoded_c[ENCODED_LEN];
  char inbits[UNCODED_LEN]={1,1,0,0,1,0,0,1,1,0,1};
  char outbits[ENCODED_LEN];
  char encoded_bits[ENCODED_LEN*BYTE_LEN];
  char decoded_bits[UNCODED_LEN*BYTE_LEN];
  char encoded_bits_recv[ENCODED_LEN*BYTE_LEN];
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
      printf("uncoded: ");
      print_arr(readbuf,UNCODED_LEN);
      encode(readbuf,encodedbuf);
      printf("encoded: ");
      print_arr(encodedbuf,ENCODED_LEN);
      decode(encodedbuf,recvbuf);
      printf("decoded: ");
      print_arr(recvbuf,UNCODED_LEN);

  }
  return 0;
}