#pragma once
#include <vector>
#include "Channel.h"
#include "Tokenizer.h"


class Animation {
public:
    int numChannels;    
    float time_start, time_end;  
    std::vector<Channel*> channels;

    Animation();
    ~Animation();

    // void addChannel(const Channel& channel);
    bool Load(const char* animfile);
    void Precompute();
    // Evaluate all channels at a given time and store the results in `poses`
    void Evaluate(float time, std::vector<float>& poses);

    // // Evaluate all channels at a given time
    // std::vector<float> evaluate(float time);
};
