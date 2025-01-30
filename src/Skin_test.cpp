#include <glm/gtc/matrix_transform.hpp>
#include "Skin.h"
#include "Vertex.h"
#include <iostream>
#include <iomanip> // For formatted output

#include "Skin.h"
#include "Skeleton.h"
#include "glm/gtx/string_cast.hpp"
#include <iostream>

Skin::Skin(Skeleton* skel)
{
    skeleton = skel;
}

Skin::~Skin()
{
}

bool Skin::Load(const char* filename)
{
    Tokenizer* token = new Tokenizer();
    token->Open(filename);

    '''
    positions [numverts] {
        [x] [y] [z]
        ...
    }
    '''

    // Load number of positions
    token->FindToken("positions");
    vertexNum = token->GetFloat();

    // Load positions
    token->FindToken("{");
    float px, py, pz;
    for (int i = 0; i < vertexNum; i++) {
        Vertex* vertex = new Vertex();
        px = token->GetFloat();
        py = token->GetFloat();
        pz = token->GetFloat();
        vertex->setPositions(px, py, pz);
        vertices.push_back(vertex);
        bindingPositions.push_back({ px, py, pz });
        shaderPositions.push_back({ px, py, pz });
    }

    // Load normals
    token->FindToken("{");
    float nx, ny, nz;
    for (int i = 0; i < vertexNum; i++) {
        nx = token->GetFloat();
        ny = token->GetFloat();
        nz = token->GetFloat();
        vertices[i]->setNormals(nx, ny, nz);
        bindingNormals.push_back({ nx, ny, nz });
        shaderNormals.push_back({ nx, ny, nz });
    }

    // Load weights
    token->FindToken("{");
    int attachmentNum, JointID;
    float weight;
    for (int i = 0; i < vertexNum; i++) {
        attachmentNum = token->GetFloat();
        for (int j = 0; j < attachmentNum; j++) {
            JointID = token->GetFloat();
            weight = token->GetFloat();
            vertices[i]->weights.push_back(weight);
            vertices[i]->joints.push_back(skeleton->joints[JointID]);
        }
    }

    // Load indices/triangles
    token->FindToken("triangles");
    int triangleNum = token->GetFloat();
    token->FindToken("{");
    int v0, v1, v2;
    for (int i = 0; i < triangleNum; i++) {
        v0 = token->GetFloat();
        v1 = token->GetFloat();
        v2 = token->GetFloat();
        shaderIndices.push_back(v0);
        shaderIndices.push_back(v1);
        shaderIndices.push_back(v2);
    }

    // Load binding matrices
    token->FindToken("bindings");
    int bindingNum = token->GetFloat();
    token->FindToken("{");
    float ax, ay, az, bx, by, bz, cx, cy, cz, dx, dy, dz;
    for (int i = 0; i < bindingNum; i++) {
        token->FindToken("{");
        ax = token->GetFloat();
        ay = token->GetFloat();
        az = token->GetFloat();
        bx = token->GetFloat();
        by = token->GetFloat();
        bz = token->GetFloat();
        cx = token->GetFloat();
        cy = token->GetFloat();
        cz = token->GetFloat();
        dx = token->GetFloat();
        dy = token->GetFloat();
        dz = token->GetFloat();
        skeleton->joints[i]->inverseB = glm::inverse(
            glm::mat4(
                ax, ay, az, 0.0f,
                bx, by, bz, 0.0f, 
                cx, cy, cz, 0.0f, 
                dx, dy, dz, 1.0f
            )
        );
    }

    token->Close();
    return true;
}

void Skin::Update()
{
    // Compute new vertex positions and normals based on the skeleton
    Vertex* curV;
    glm::vec3 curPosition;
    glm::vec3 curNormal;
    glm::mat4 M;
    glm::vec4 transformedPosition;
    glm::vec4 transformedNormal;

    for (int i = 0; i < vertexNum; i++) {
        curV = vertices[i];
        curPosition = curV->position;
        curNormal = curV->normal;
        transformedPosition = glm::vec4(0.0f);
        transformedNormal = glm::vec4(0.0f);

        for (int j = 0; j < curV->joints.size(); j++) {
            M = curV->weights[j] * curV->joints[j]->W * curV->joints[j]->inverseB;
            transformedPosition += M * glm::vec4(curPosition, 1.0f);
            transformedNormal += M * glm::vec4(curNormal, 0.0f);
        }

        shaderPositions[i] = glm::vec3(transformedPosition);
        transformedNormal = glm::normalize(transformedNormal);
        shaderNormals[i] = glm::vec3(transformedNormal);
    }
}

void Skin::Draw(bool isDrawOriginalSkin, const glm::mat4& viewProjMtx, GLuint shader)
{
    glm::mat4 modelMtx = glm::mat4(1.0f);
    glm::mat4 mvpMtx = viewProjMtx * modelMtx;
    glm::vec3 ambientColor = { 0.1f, 0.05f, 0.8f }; // Final reflected ambient color

    // Activate the shader program
    glUseProgram(shader);

    // Get the locations and send the uniforms to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "ModelMtx"), 1, GL_FALSE, glm::value_ptr(modelMtx));
    glUniformMatrix4fv(glGetUniformLocation(shader, "ModelViewProjectionMtx"), 1, GL_FALSE, glm::value_ptr(mvpMtx));
    glUniform3fv(glGetUniformLocation(shader, "AmbientColor"), 1, &ambientColor[0]);

    // Use the immediate mode drawing approach (without VBOs)
    if (isDrawOriginalSkin) {
        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < shaderIndices.size(); i += 3) {
            int v0 = shaderIndices[i];
            int v1 = shaderIndices[i + 1];
            int v2 = shaderIndices[i + 2];

            // Original positions
            glVertex3f(bindingPositions[v0].x, bindingPositions[v0].y, bindingPositions[v0].z);
            glVertex3f(bindingPositions[v1].x, bindingPositions[v1].y, bindingPositions[v1].z);
            glVertex3f(bindingPositions[v2].x, bindingPositions[v2].y, bindingPositions[v2].z);

            // Original normals
            glNormal3f(bindingNormals[v0].x, bindingNormals[v0].y, bindingNormals[v0].z);
            glNormal3f(bindingNormals[v1].x, bindingNormals[v1].y, bindingNormals[v1].z);
            glNormal3f(bindingNormals[v2].x, bindingNormals[v2].y, bindingNormals[v2].z);
        }
        glEnd();
    } else {
        glBegin(GL_TRIANGLES);
        for (size_t i = 0; i < shaderIndices.size(); i += 3) {
            int v0 = shaderIndices[i];
            int v1 = shaderIndices[i + 1];
            int v2 = shaderIndices[i + 2];

            // Shader transformed positions
            glVertex3f(shaderPositions[v0].x, shaderPositions[v0].y, shaderPositions[v0].z);
            glVertex3f(shaderPositions[v1].x, shaderPositions[v1].y, shaderPositions[v1].z);
            glVertex3f(shaderPositions[v2].x, shaderPositions[v2].y, shaderPositions[v2].z);

            // Shader transformed normals
            glNormal3f(shaderNormals[v0].x, shaderNormals[v0].y, shaderNormals[v0].z);
            glNormal3f(shaderNormals[v1].x, shaderNormals[v1].y, shaderNormals[v1].z);
            glNormal3f(shaderNormals[v2].x, shaderNormals[v2].y, shaderNormals[v2].z);
        }
        glEnd();
    }

    // Deactivate the shader program
    glUseProgram(0);
}

// void Skin::Draw( const glm::mat4& viewProjMtx, GLuint shader)
// {
//    // Prepare the model matrix and MVP matrix
//     glm::mat4 modelMtx = glm::mat4(1.0f);  // Identity model matrix
//     glm::mat4 mvpMtx = viewProjMtx * modelMtx;

//     // Use the shader program
//     glUseProgram(shader);

//     // Send the model and MVP matrices to the shader as uniform variables
//     glUniformMatrix4fv(glGetUniformLocation(shader, "ModelMtx"), 1, GL_FALSE, (float*)&modelMtx);
//     glUniformMatrix4fv(glGetUniformLocation(shader, "ModelViewProjectionMtx"), 1, GL_FALSE, (float*)&mvpMtx);

//     // Send additional uniforms (e.g., lighting) if necessary
//     glm::vec3 ambientColor = { 0.1, 0.05, 0.8 }; // Example ambient color
//     glUniform3fv(glGetUniformLocation(shader, "AmbientColor"), 1, &ambientColor[0]);

//     // Iterate over the vertices and apply skinning
//     std::vector<glm::vec3> transformedPositions;
//     std::vector<glm::vec3> transformedNormals;

//     for (int i = 0; i < vertexNum; i++) {
//         Vertex* curV = vertices[i];
//         glm::vec3 curPosition = curV->position;
//         glm::vec3 transformedPosition = glm::vec3(0.0f);

//         // Apply skinning transformations (no buffers, just CPU-side calculations)
//         for (int j = 0; j < curV->joints.size(); j++) {
//             glm::mat4 jointMatrix = curV->weights[j] * curV->joints[j]->W * curV->joints[j]->inverseB;
//             glm::vec4 transformedVec = jointMatrix * glm::vec4(curPosition, 1.0f);
//             transformedPosition += glm::vec3(transformedVec);
//         }

//         transformedPositions.push_back(transformedPosition);

//         // Similarly, apply the skinning to normals (if needed)
//         glm::vec3 curNormal = curV->normal;
//         glm::vec3 transformedNormal = glm::vec3(0.0f);

//         for (int j = 0; j < curV->joints.size(); j++) {
//             glm::mat4 jointMatrix = curV->weights[j] * curV->joints[j]->W * curV->joints[j]->inverseB;
//             glm::vec4 transformedNormalVec = jointMatrix * glm::vec4(curNormal, 0.0f);
//             transformedNormal += glm::vec3(transformedNormalVec);
//         }

//         transformedNormals.push_back(transformedNormal);
//     }

//     // Now we need to send the transformed positions and normals directly to the shader
//     GLuint positionLocation = glGetAttribLocation(shader, "Position");
//     GLuint normalLocation = glGetAttribLocation(shader, "Normal");

//     // Send transformed positions to shader
//     for (int i = 0; i < transformedPositions.size(); i++) {
//         glVertexAttrib3fv(positionLocation, &transformedPositions[i][0]);
//     }

//     // Send transformed normals to shader
//     for (int i = 0; i < transformedNormals.size(); i++) {
//         glVertexAttrib3fv(normalLocation, &transformedNormals[i][0]);
//     }


//     // Render the geometry using glDrawArrays or glDrawElements
//     // You can use the EBO (element buffer) to draw indexed geometry or use glDrawArrays if no indices
//     glDrawElements(GL_TRIANGLES, shaderIndices.size(), GL_UNSIGNED_INT, 0); // If using an index buffer
//     // glDrawArrays(GL_TRIANGLES, 0, transformedPositions.size()); // If no indices
//     // glDrawElements(GL_TRIANGLES, shaderIndices.size(), GL_UNSIGNED_INT, 0);

//     // Unbind the VAO and shader program
//     glBindVertexArray(0);
//     glUseProgram(0);

//     std::cout << "Skin drawn with shader (no GPU buffers). Vertices transformed." << std::endl;

// }


void Skin::SanityCheck() {
    std::cout << "==== Sanity Check: Skin Data ====" << std::endl;

    // Print vertex positions
    std::cout << "Vertex Positions (" << vertexNum << " vertices):" << std::endl;
    for (size_t i = 0; i < vertices.size(); i++) {
        std::cout << "Vertex " << i << ": " << glm::to_string(vertices[i]->position) << std::endl;
    }

    // Print vertex normals
    std::cout << "\nVertex Normals:" << std::endl;
    for (size_t i = 0; i < vertices.size(); i++) {
        std::cout << "Vertex " << i << ": " << glm::to_string(vertices[i]->normal) << std::endl;
    }

    // Print vertex weights and joint attachments
    std::cout << "\nVertex Weights & Joint Attachments:" << std::endl;
    for (size_t i = 0; i < vertices.size(); i++) {
        std::cout << "Vertex " << i << " - Weights: ";
        for (size_t j = 0; j < vertices[i]->weights.size(); j++) {
            std::cout << "(Joint " << vertices[i]->joints[j]->JointName << ": " 
                      << vertices[i]->weights[j] << ") ";
        }
        std::cout << std::endl;
    }

    // Print triangle indices
    std::cout << "\nTriangle Indices (" << shaderIndices.size() / 3 << " triangles):" << std::endl;
    for (size_t i = 0; i < shaderIndices.size(); i += 3) {
        std::cout << "Triangle " << i / 3 << ": (" 
                  << shaderIndices[i] << ", " 
                  << shaderIndices[i + 1] << ", " 
                  << shaderIndices[i + 2] << ")" << std::endl;
    }

    // Print binding matrices for each joint
    std::cout << "\nBinding Matrices:" << std::endl;
    for (size_t i = 0; i < skeleton->joints.size(); i++) {
        std::cout << "Joint " << i << " Inverse Bind Matrix:\n" 
                  << glm::to_string(skeleton->joints[i]->inverseB) << std::endl;
    }

    std::cout << "==================================" << std::endl;
}
