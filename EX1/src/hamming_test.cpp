#include "hamming.hpp"
#include "ServerUtil.hpp"
#include "noise.hpp"
using namespace Hamming;
int main(int argc, char** argv){

  //global variables
  int iResult;
  char sendbuf[ENCODED_MSG_LEN] = {0};
  char recvbuf[UNCODED_MSG_LEN] ={0};
  //FILE* fp = fopen("../Dtelem.jpg", "r" );
  //FILE* Wfp = fopen("../outDtele.jpg","w");
  FILE* fp = fopen("../testFile.txt", "r" );
  FILE* Wfp = fopen("../outtestFile.txt","w");
  fseek(fp,0,SEEK_END);
  printf("file length in bytes: %ld\n",ftell(fp));
  iResult = fseek(fp,0,SEEK_SET);
  if(iResult != 0){
    printf("seek error: %S\n",strerror(iResult));
  }
  NoiseyChannel noise(5,1024);
  bool eof = false;
  while (!eof){
    iResult = Hamming::read_msg(fp,sendbuf);
    noise.rand_flip_bit(sendbuf,ENCODED_MSG_LEN);
    if(iResult==0) eof = true;
    if(iResult<0) return 1;
      printf("\n\n");
      Hamming::write_msg(Wfp,sendbuf,recvbuf);
      printf("\n");
    //print_arr_nl(recvbuf,UNCODED_MSG_LEN);
  }
  fclose(Wfp);
  return 0;
}