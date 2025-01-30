#pragma once

#include <vector>
#include <string>  // Required for std::string
#include <iostream> // For std::cout, std::endl
#include "Skeleton.h"

#include "core.h"

Skeleton::Skeleton() {root = NULL;}
Skeleton::~Skeleton() {}

// ○ Reading in .skel files through load()
// ○ Building skeleton tree structure (bones + joints)
bool Skeleton::load(const char* skelFile){
    Tokenizer token;
    token.Open(skelFile);
    token.FindToken("balljoint");

    // Parse tree
    root = new Joint();
    root->Load(token);

    // Finish
    token.Close();
    return true;
}


        
// ○ Computing local/world matrices through update()
void Skeleton::update()
{
    glm::mat4 temp = glm::mat4(1.0f);
	root->Update(temp);
}

// ○ Rendering everything through draw()

void Skeleton::draw(const glm::mat4& viewProjMtx, GLuint shader)
{
	root->Draw(viewProjMtx, shader);
}