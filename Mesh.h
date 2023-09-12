#ifndef MESH_CLASS_H
#define MESH_CLASS_H

#include <string>
#include<vector>

#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"
#include "shaderClass.h"


class Mesh
{
public:
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;
	// Store VAO in public so it can be used in the Draw function
	VAO VAO;
	float angleOfRotation = 0.0f;
	GLint lightType = 0;
	GLint normal_enabled = 0;
	float scale = 1.0f;
	float sensibility = 1000.0f;
	glm::vec3 directionRot = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f); // use me for update translation direction and amount
	glm::mat4 rotationMatrix = glm::mat4(1.0f);
	bool pressed_P=false;
	bool pressed_N=false;

	Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, std::vector<Texture> &textures);
	
	void Draw(GLFWwindow *window, Shader &shader, Camera &camera);
	void DrawAssistant(GLFWwindow *window, Shader &shader, Camera &camera);
};
#endif