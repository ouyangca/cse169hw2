#include <glm/gtc/matrix_transform.hpp>
#include "Joint.h"
#include "Cube.h"
#include "DOF.h"
#include <iostream>
#include <iomanip> // For formatted output

Joint::Joint()
{
	offset = { 0.0f, 0.0f, 0.0f }; // M
	boxmin = { -0.1f, -0.1f, -0.1f };
	boxmax = { 0.1f, 0.1f, 0.1f };
	L = glm::mat4(1.0f); // local matrix
	W = glm::mat4(1.0f); // world matrix
	DOF* DOFx = new DOF();
	DOF* DOFy = new DOF();
	DOF* DOFz = new DOF();
    // DOF limits
	JointDOF.push_back(DOFx);
	JointDOF.push_back(DOFy);
	JointDOF.push_back(DOFz);
	cube = new Cube(boxmin, boxmax);
	strcpy(JointName, "");
}

Joint::~Joint()
{
}



void Joint::PrintValues() const {
    std::cout << "Joint Name: " << JointName << "\n";

    std::cout << "Offset: "
              << "x = " << offset.x << ", y = " << offset.y << ", z = " << offset.z << "\n";

    std::cout << "BoxMin: "
              << "x = " << boxmin.x << ", y = " << boxmin.y << ", z = " << boxmin.z << "\n";

    std::cout << "BoxMax: "
              << "x = " << boxmax.x << ", y = " << boxmax.y << ", z = " << boxmax.z << "\n";

    std::cout << "DOF Limits:\n";
    std::cout << "  RotX: Min = " << JointDOF[0]->GetMinLimit() << ", Max = " << JointDOF[0]->GetMaxLimit() << "\n";
    std::cout << "  RotY: Min = " << JointDOF[1]->GetMinLimit() << ", Max = " << JointDOF[1]->GetMaxLimit() << "\n";
    std::cout << "  RotZ: Min = " << JointDOF[2]->GetMinLimit() << ", Max = " << JointDOF[2]->GetMaxLimit() << "\n";

    std::cout << "Pose: "
              << "x = " << pose.x << ", y = " << pose.y << ", z = " << pose.z << "\n";

    
    std::cout << std::endl;
}


bool Joint::Load(Tokenizer &token) {
    float DOFmin, DOFmax;
    char name[256];
    token.GetToken(name);
	strcpy(JointName, name);
    token.FindToken("{");
    while(1) {
        char temp[256];
        token.GetToken(temp);
        if(strcmp(temp,"offset")==0) {
            offset.x=token.GetFloat();
            offset.y=token.GetFloat();
            offset.z=token.GetFloat();
        }
        else if (strcmp(temp, "boxmin") == 0)
		{
			boxmin.x = token.GetFloat();
			boxmin.y = token.GetFloat();
			boxmin.z = token.GetFloat();
		}
		else if (strcmp(temp, "boxmax") == 0)
		{
			boxmax.x = token.GetFloat();
			boxmax.y = token.GetFloat();
			boxmax.z = token.GetFloat();
		}
		else if (strcmp(temp, "rotxlimit") == 0)
		{
			DOFmin = token.GetFloat();
			DOFmax = token.GetFloat();
			JointDOF[0]->SetMinMax(DOFmin, DOFmax);
		}
		else if (strcmp(temp, "rotylimit") == 0)
		{
			DOFmin = token.GetFloat();
			DOFmax = token.GetFloat();
			JointDOF[1]->SetMinMax(DOFmin, DOFmax);
		}
		else if (strcmp(temp, "rotzlimit") == 0)
		{
			DOFmin = token.GetFloat();
			DOFmax = token.GetFloat();
			JointDOF[2]->SetMinMax(DOFmin, DOFmax);
		}
		else if (strcmp(temp, "pose") == 0)
		{
			pose.x = token.GetFloat();
			pose.y = token.GetFloat();
			pose.z = token.GetFloat();
			JointDOF[0]->SetValue(pose.x);
			JointDOF[1]->SetValue(pose.y);
			JointDOF[2]->SetValue(pose.z);
		}
        else if(strcmp(temp,"balljoint")==0) {
            Joint *jnt = new Joint;
            jnt->Load(token);
            AddChild(jnt);
        }
        else if(strcmp(temp,"}")==0){
            cube = new Cube(boxmin, boxmax);
            PrintValues();
            return true;
            } 
        else token.SkipLine(); // Unrecognized token
    }
}


void Joint::AddChild(Joint* child)
{
	children.push_back(child);
}

void Joint::Update(glm::mat4& parent) {
    // Compute local matrix L 
    // L(phi) = M⋅ 𝐑𝑥 (phi)
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), offset);
    glm::mat4 rotationX = glm::rotate(glm::mat4(1.0f), JointDOF[0]->GetValue(), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::mat4 rotationY = glm::rotate(glm::mat4(1.0f), JointDOF[1]->GetValue(), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 rotationZ = glm::rotate(glm::mat4(1.0f), JointDOF[2]->GetValue(), glm::vec3(0.0f, 0.0f, 1.0f));
    L = translation * rotationZ * rotationY * rotationX;

    // Compute world matrix W
    W = parent * L;

    // Recursively call Update() on children
    for (auto child : children) {
        child->Update(W);
    }
}
void Joint::Draw(const glm::mat4& viewProjMtx, GLuint shader) {
    // Set the OpenGL model matrix to the joint's world matrix
    // glPushMatrix();
    // glMultMatrixf(glm::value_ptr(W));

    // Draw the joint as a cube

    cube->draw(viewProjMtx * W, shader);

    // Recursively call Draw() on children

    for (auto* child : children) {
        child->Draw(viewProjMtx, shader);
    }

}
