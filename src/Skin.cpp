#include "Skin.h"
#include "glm/gtx/string_cast.hpp"
#include <iostream>

Skin::Skin(Skeleton* skel)
{
    if (skel){
        skeleton = skel;
    } 
}

Skin::~Skin()
{
}


bool Skin::Load(const char* filename)
{
    Tokenizer* tknizer = new Tokenizer();
    tknizer->Open(filename);

    tknizer->FindToken("positions");
    vertexNum = tknizer->GetFloat();

    // Set positions
    tknizer->FindToken("{");
    float px, py, pz;
    for (int i = 0; i < vertexNum; i++) {
        Vertex* vertex = new Vertex();
        px = tknizer->GetFloat();
        py = tknizer->GetFloat();
        pz = tknizer->GetFloat();
        vertex->setPositions(px, py, pz);
        vertices.push_back(vertex);
        bindingPositions.push_back({ px, py, pz });
        shaderPositions.push_back({ px, py, pz });
    }

    // Set normals
    tknizer->FindToken("{");
    float nx, ny, nz;
    for (int i = 0; i < vertexNum; i++) {
        nx = tknizer->GetFloat();
        ny = tknizer->GetFloat();
        nz = tknizer->GetFloat();
        vertices[i]->setNormals(nx, ny, nz);
        bindingNormals.push_back({ nx, ny, nz });
        shaderNormals.push_back({ nx, ny, nz });
    }

    // Set weights
    tknizer->FindToken("{");
    int attachmentNum, JointID;
    float weight;
    for (int i = 0; i < vertexNum; i++) {
        attachmentNum = tknizer->GetFloat();
        for (int j = 0; j < attachmentNum; j++) {
            JointID = tknizer->GetFloat();
            weight = tknizer->GetFloat();
            vertices[i]->weights.push_back(weight);
            vertices[i]->joints.push_back(skeleton->joints[JointID]);
        }
    }

    // Set indices/triangles
    tknizer->FindToken("triangles");
    int triangleNum = tknizer->GetFloat();
    tknizer->FindToken("{");
    int v0, v1, v2;
    for (int i = 0; i < triangleNum; i++) {
        v0 = tknizer->GetFloat();
        v1 = tknizer->GetFloat();
        v2 = tknizer->GetFloat();
        shaderIndices.push_back(v0);
        shaderIndices.push_back(v1);
        shaderIndices.push_back(v2);
    }

    // Set binding matrices (one binding matrix to one joint)
    tknizer->FindToken("bindings");
    int bindingNum = tknizer->GetFloat(); // bindingNum = jointNum
    tknizer->FindToken("{");
    float ax, ay, az, bx, by, bz, cx, cy, cz, dx, dy, dz;
    for (int i = 0; i < bindingNum; i++) {
        tknizer->FindToken("{");
        ax = tknizer->GetFloat();
        ay = tknizer->GetFloat();
        az = tknizer->GetFloat();
        bx = tknizer->GetFloat();
        by = tknizer->GetFloat();
        bz = tknizer->GetFloat();
        cx = tknizer->GetFloat();
        cy = tknizer->GetFloat();
        cz = tknizer->GetFloat();
        dx = tknizer->GetFloat();
        dy = tknizer->GetFloat();
        dz = tknizer->GetFloat();
        skeleton->joints[i]->inverseB = glm::inverse(
            glm::mat4(
                ax, ay, az, 0.0f,
                bx, by, bz, 0.0f, 
                cx, cy, cz, 0.0f, 
                dx, dy, dz, 1.0f
            )
        );
    }

    tknizer->Close();
    BindBuffer();
    return true;
}

// bool Skin::Load(const char* filename)
// {
//     Tokenizer* tknizer = new Tokenizer();
    
//     tknizer->Open(filename);

//     tknizer->FindToken("positions");
//     vertexNum = tknizer->GetFloat();

//     // Set positions
//     tknizer->FindToken("{");
//     float px, py, pz;
//     for (int i = 0; i < vertexNum; i++) {
//         Vertex* vertex = new Vertex();
//         px = tknizer->GetFloat();
//         py = tknizer->GetFloat();
//         pz = tknizer->GetFloat();
//         vertex->setPositions(px, py, pz);
//         vertex->setShaderPositions(px, py, pz);
//         vertices.push_back(vertex);
//         bindingPositions.push_back({ px, py, pz });
//         shaderPositions.push_back({ px, py, pz });
//     }

//     tknizer->FindToken("{");
//     float nx, ny, nz;
//     for (int i = 0; i < vertexNum; i++) {
//         nx = tknizer->GetFloat();
//         ny = tknizer->GetFloat();
//         nz = tknizer->GetFloat();
//         vertices[i]->setNormals(nx, ny, nz);
//         vertices[i]->setShaderNormals(nx, ny, nz);
//         bindingNormals.push_back({ nx, ny, nz });
//         shaderNormals.push_back({ nx, ny, nz });
//     }

//     // Set weights
//     tknizer->FindToken("{");
//     int attachmentNum, JointID;
//     float weight;
//     for (int i = 0; i < vertexNum; i++) {
//         attachmentNum = tknizer->GetFloat();
//         for (int j = 0; j < attachmentNum; j++) {
//             JointID = tknizer->GetFloat();
//             weight = tknizer->GetFloat();
//             vertices[i]->weights.push_back(weight);
//             vertices[i]->joints.push_back(skeleton->joints[JointID]);
//         }
//     }

//     std::cout << "seg" << std::endl;

//     // Set indices/triangles
//     tknizer->FindToken("triangles");
//     int triangleNum = tknizer->GetFloat();
//     tknizer->FindToken("{");
//     for (int i = 0; i < triangleNum; i++) {
//         int v0 = tknizer->GetFloat();
//         int v1 = tknizer->GetFloat();
//         int v2 = tknizer->GetFloat();
//         shaderIndices.push_back(v0);
//         shaderIndices.push_back(v1);
//         shaderIndices.push_back(v2);
//     }
//     std::cout << "seg" << std::endl;


//     // Set binding matrices (one binding matrix to one joint)
//     tknizer->FindToken("bindings");
//     int bindingNum = tknizer->GetFloat(); // bindingNum = jointNum
//     tknizer->FindToken("{");
//     float ax, ay, az, bx, by, bz, cx, cy, cz, dx, dy, dz;
//     for (int i = 0; i < bindingNum; i++) {
//         tknizer->FindToken("{");
//         ax = tknizer->GetFloat();
//         ay = tknizer->GetFloat();
//         az = tknizer->GetFloat();
//         bx = tknizer->GetFloat();
//         by = tknizer->GetFloat();
//         bz = tknizer->GetFloat();
//         cx = tknizer->GetFloat();
//         cy = tknizer->GetFloat();
//         cz = tknizer->GetFloat();
//         dx = tknizer->GetFloat();
//         dy = tknizer->GetFloat();
//         dz = tknizer->GetFloat();
//         skeleton->joints[i]->inverseB = glm::inverse(
//             glm::mat4(
//                 ax, ay, az, 0.0f,
//                 bx, by, bz, 0.0f, 
//                 cx, cy, cz, 0.0f, 
//                 dx, dy, dz, 1.0f
//             )
//         );

//     }

//     tknizer->Close();
//     return true;
// }

void Skin::Update()
{
    // Compute matrix for each joint of the current vertex;
    // Compute blended world space positions & normals for each vertex;
    // Vertex* curV;
    // glm::vec3 curPosition;
    // glm::vec3 curNormal;
    // glm::mat4 M;
    // glm::vec4 transformedPosition;
    // glm::vec4 transformedNormal;

    // for (int i = 0; i < vertexNum; i++) {
    //     curV = vertices[i];
    //     curPosition = curV->position;
    //     curNormal = curV->normal;
    //     transformedPosition = glm::vec4(0.0f);
    //     transformedNormal = glm::vec4(0.0f);

    //     for (int j = 0; j < curV->joints.size(); j++) {
    //         M = curV->weights[j] * curV->joints[j]->W * curV->joints[j]->inverseB;
    //         transformedPosition += M * glm::vec4(curPosition, 1.0f);
    //         transformedNormal += M * glm::vec4(curNormal, 0.0f);
    //     }

    //     shaderPositions[i] = glm::vec3(transformedPosition);
    //     transformedNormal = glm::normalize(transformedNormal);
    //     shaderNormals[i] = glm::vec3(transformedNormal);
    // }
}

void Skin::Draw(bool skel_found, const glm::mat4& viewProjMtx, GLuint shader)
{
    std::cout << "seg" << std::endl;
    
    triangles = new Triangle(skel_found, vertices, shaderIndices);
    std::cout << "seg" << std::endl;
    triangles->draw(viewProjMtx, shader);
}