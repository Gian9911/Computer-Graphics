#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shaderClass.h"

class Camera {
public:
  glm::vec3 Position;
  glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::mat4 cameraMatrix = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);
  int axis = 0;
  // Prevents camera jumping around at first click
  bool firstClick = true;

  int width;
  int height;

  float speed = 0.005f;
  float sensitivity = 100.0f;

  Camera(int width, int height, glm::vec3 position);

  void updateMatrix(float FOVdeg, float nearPlane, float farPlane); // update cameraMatrix
  void Matrix(Shader &shader, const char *uniform);                 // send cameraMatrix
  void Inputs(GLFWwindow *window);
  bool CheckValidMove(glm::vec3 newOrientation);
};
#endif