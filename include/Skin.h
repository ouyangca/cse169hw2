// #pragma once

// #include <vector>
// #include "Skeleton.h"
// #include "Vertex.h"
// #include "Tokenizer.h"
// #include "glm/glm.hpp"

// class Skin {
// public:
//     Skin(Skeleton* skel);
//     ~Skin();

//     bool Load(const char* filename);
//     void Update();
//     void Draw(const glm::mat4& viewProjMtx, GLuint shader); 
//     void SanityCheck();


// private:
//     Skeleton* skeleton; // Reference to the skeleton
//     int vertexNum; // Number of vertices

//     std::vector<Vertex*> vertices; // List of vertices
//     std::vector<glm::vec3> bindingPositions; // Initial vertex positions
//     std::vector<glm::vec3> bindingNormals; // Initial normals

//     std::vector<glm::vec3> shaderPositions; // CPU-stored vertex positions
//     std::vector<glm::vec3> shaderNormals; // CPU-stored vertex normals
//     std::vector<unsigned int> shaderIndices; // CPU-stored triangle indices
    
// };


#pragma once
#include "Vertex.h"
#include "Tokenizer.h"
#include <vector>
#include "Skeleton.h"

class Skin
{
public:
	int vertexNum;
	std::vector<Vertex*> vertices;
	// The skeleton associated with this skin; used for linking joints
	Skeleton* skeleton; 

	// shader-related
	GLuint VAO, VBO_positions, VBO_normals, EBO;
	std::vector<glm::vec3> bindingPositions;
	std::vector<glm::vec3> bindingNormals;
	std::vector<glm::vec3> shaderPositions;
	std::vector<glm::vec3> shaderNormals;
	std::vector<unsigned int> shaderIndices;



	Skin(Skeleton* skel);
	~Skin();

	void BindBuffer();
	bool Load(const char* filename = "assets/wasp.skin");
	void Update();
	void Draw(bool isDrawOriginalSkin, const glm::mat4& viewProjMtx, GLuint shader);

};