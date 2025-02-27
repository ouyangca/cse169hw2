

#include "Animation.h"
#include "Tokenizer.h" 
#include <vector>
#include <iostream>


// Constructor
Animation::Animation()  {
}

// Destructor
Animation::~Animation() {
}

// Load animation data from a file
bool Animation::Load(const char* animfile) {
    Tokenizer* tokenizer = new Tokenizer();
    if (!tokenizer->Open(animfile)) {
        delete tokenizer;
        return false; // Failed to open file
    }

    // tokenizer->FindToken("animation");

    // Set time range
    tokenizer->FindToken("range");
    time_start = tokenizer->GetFloat();
    time_end = tokenizer->GetFloat();

    // Get number of channels
    tokenizer->FindToken("numchannels");
    numChannels = tokenizer->GetFloat();

    // tokenizer->FindToken("channels");

    // Load each channel
    for (int i = 0; i < numChannels; i++) {
        Channel* new_channel = new Channel();
        if (new_channel->Load(tokenizer)) {
            channels.push_back(new_channel);
        } else {
            delete new_channel; // Clean up if loading fails
            std::cout << "ERROR loading animation file." << std::endl;
            return false;
        }
    }

    tokenizer->Close();
    Precompute();
    delete tokenizer;
    return true;
}

// Precompute data for all channels
void Animation::Precompute() {
    for (int i = 0; i < numChannels; i++)
		channels[i]->Precompute();
}

// Evaluate all channels at a given time and store the results in `poses`
void Animation::Evaluate(float time, std::vector<float>& poses) {
    poses.clear(); // Clear any existing data in the poses vector
    for (int i = 0; i < numChannels; i++)
		poses[i] = channels[i]->Evaluate(time);
}



// #include "Animation.h"

// #include <vector>

// class Animation {
// public:
//     std::vector<Channel> channels;

//     // Add a channel to the animation
//     void addChannel(const Channel& channel) {
//         channels.push_back(channel);
//     }

//     void Load(const char* filename){
//         Tokenizer* tokenizer = new Tokenizer();
//         tokenizer->Open(filename);

//         tokenizer->FindToken("animation");

//         // set time range
//         tokenizer->FindToken("range");
//         float time_start, time_end;
//         time_start = tokenizer->GetFloat();
//         time_end = tokenizer->GetFloat();


//         // # of channels
//         tokenizer->FindToken("numchannels");
//         float numChannels = tokenizer->GetFloat();

//         tokenizer->FindToken("channels");

//         // load each channel
//         for (int i = 0; i < numChannels; i++) {
//             Channel channel = new Channel();
//             channel.Load(tokenizer);
//             channels.push_back(channel);

//         }

//         tokenizer->Close();
//     }
    

//     // Evaluate all channels at a given time
//     std::vector<float> evaluate(float time) {
//         std::vector<float> values;
//         for (auto& channel : channels) {
//             values.push_back(channel.evaluate(time));
//         }
//         return values;
//     }
// };
