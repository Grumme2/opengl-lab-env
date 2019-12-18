#include "config.h"
//#include "exampleapp.h"
#include "GraphicsNode.h"
#include <cstring>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
//#include <bits/stdc++.h>
#include "imgui.h"
//#include "btree.h"
#include <glm/glm.hpp>
#include <math.h> 
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
# define M_PIl          3.141592653589793238462643383279502884L
#define STRING_BUFFER_SIZE 8192

#include "Shader.h"
#include <fstream>

GraphicsNode::GraphicsNode(Mesh* mesh, Material* material, glm::mat4 transform) {
	GraphicsNode::mesh = mesh;
	GraphicsNode::material = material;
	GraphicsNode::transform = transform;
}

void GraphicsNode::Draw() {
	GraphicsNode::mesh->BindMesh();
	GraphicsNode::mesh->setupmesh();
	GraphicsNode::material->ApplyMaterial();

	int size = GraphicsNode::mesh->getindexbuffersize();
	glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, nullptr);
}