#include "Rig.h"
#include <iostream>

Rig::Rig()
{
	skeleton = new Skeleton();
	skin = new Skin(true, skeleton);
}

Rig::~Rig()
{
	delete skeleton;
	delete skin;
}

bool Rig::Load(const char* skelfile, const char* skinfile)
{
	bool isLoadSkel = skeleton->load(skelfile);
	bool isLoadSkin = skin->Load(skinfile);
	if (isLoadSkel && isLoadSkin) {
		std::cout << "Successfully Loaded Animation Rig!" << std::endl;
		return true;
	}
	else if (!isLoadSkel) {
		std::cout << "Failed To Load Skeleton for Animation Rig!" << std::endl;
		return false;
	}
	else if (!isLoadSkin) {
		std::cout << "Failed To Load Skin for Animation Rig!" << std::endl;
		return false;
	}
	return true;
}

void Rig::Update(glm::mat4 parentW)
{
	skeleton->Update(parentW);
	skin->Update(true);
	// std::cout << "happening" << std::endl;
}

void Rig::Draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	skin->DrawTriangle(viewProjMtx, shader);
}