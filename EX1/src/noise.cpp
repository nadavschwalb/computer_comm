#include "noise.hpp"

NoiseyChannel::NoiseyChannel(int seed, int n):gen(seed),distrib(0,pow(2,16)/n){
  unsigned int prob = pow(2,16)/n;
  printf("noise generator constructed with probability 1:%d\n",prob);
}

int NoiseyChannel::roll(){
  int result = distrib(gen);
  return result;
}

void NoiseyChannel::rand_flip_bit(char* msg,int msg_len){
  int result;
  for(int i=0;i<msg_len;i++){
    result = roll();
    if(result<8){
      flip(&msg[i],result);
    }
  }
}

