#pragma once
#include <vector>
#include "Keyframe.h"
#include "Tokenizer.h"


class Channel {
public:
    // Mode extrap_in;
    // Mode extrap_out;
    char extrap_in[256], extrap_out[256];
	int numKeys;
	std::vector<Keyframe*> keyframes;

	Channel();
	~Channel();

	bool Load(Tokenizer* tokenizer);
	void Precompute();
	float Evaluate(float time);


private:
    // Linear interpolation between two keyframes
    float interpolate(const Keyframe& k1, const Keyframe& k2, float time);
	int ComputeCycleNum(float curTime, float keyTime, float duration);

};