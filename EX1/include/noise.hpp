#pragma once
#include <random>
#include <math.h>
#include "ServerUtil.hpp"
#include "hamming.hpp"


  class NoiseyChannel
{
//member var
private:
  std::mt19937 gen;
  std::uniform_int_distribution<int> distrib;
//member func
  void flip(char* byte, int bit){
    char mask = 1<<bit;
    *byte = *byte^mask;
  }
public:
  NoiseyChannel(int seed, int n);
  void rand_flip_bit(char* msg,int msg_len);
  int roll();

  
};
  
 // namespace NoiseyChannel
