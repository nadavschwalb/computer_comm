#include "noise.hpp"

NoiseyChannel::NoiseyChannel(int seed, int n):gen(seed),distrib(0,pow(2,16)/n){
  unsigned int prob = pow(2,16)/n;
  printf("noise generator constructed with probability 1:%d\n",prob);
}

int NoiseyChannel::roll(){
  int result = distrib(gen);
  return result;
}

void NoiseyChannel::rand_flip_bit(char* encoded_msg){
  int result;
  char encoded_bits[ENCODED_LEN*BYTE_LEN];
  char merged_bits[ENCODED_LEN];
  for(int i=0;i<ENCODED_MSG_LEN/ENCODED_LEN;i++){
    Hamming::get_bits(encoded_msg+i*ENCODED_LEN,encoded_bits,BYTE_LEN,ENCODED_LEN);
    for(int j=0;j<ENCODED_LEN*BYTE_LEN;j++){
      if(roll()==1){
        flip(&encoded_bits[j]);
        printf("fliped byte %d\n",i*ENCODED_LEN*BYTE_LEN +j);
      }
    }
    Hamming::merge(encoded_bits,merged_bits,ENCODED_LEN);
    std::copy(merged_bits,merged_bits+ENCODED_LEN,encoded_msg+i*ENCODED_LEN);
  }
}

void NoiseyChannel::rand_flip_bit(char* encoded_msg, int msg_len){
  int flipped =0;
  for(int i=0;i<msg_len;i++){
    for(int j=0;j<BYTE_LEN;j++){
      if(roll()==1){
        flip(&encoded_msg[i],j);
        //printf("fliped bit %d\n",i*BYTE_LEN +j);
        flipped++;
      }
    }
  }
  printf("flipped: %d\n",flipped);
}



