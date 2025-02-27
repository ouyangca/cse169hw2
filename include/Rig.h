#pragma once

#include "Skeleton.h"
#include "Skin.h"

class Rig
{
	public:
		Skeleton* skeleton;
		Skin* skin;

		Rig();
		~Rig();

		bool Load(const char* skelfile, const char* skinfile);
		void Update(glm::mat4 parentW);
		void Draw(const glm::mat4& viewProjMtx, GLuint shader);
};