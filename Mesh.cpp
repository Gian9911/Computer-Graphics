#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, std::vector<Texture> &textures) {
  Mesh::vertices = vertices;
  Mesh::indices = indices;
  Mesh::textures = textures;
}

void Mesh::Draw(GLFWwindow *window, Shader &shader, Camera &camera) {
  VAO.Bind();
  VBO VBO(vertices);
  EBO EBO(indices);
  VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void *)0);                    // pos
  VAO.LinkAttrib(VBO, 1, 2, GL_FLOAT, sizeof(Vertex), (void *)(3 * sizeof(float)));  // tex
  VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void *)(5 * sizeof(float)));  // norm
  VAO.LinkAttrib(VBO, 3, 3, GL_FLOAT, sizeof(Vertex), (void *)(8 * sizeof(float)));  // tangent
  VAO.LinkAttrib(VBO, 4, 3, GL_FLOAT, sizeof(Vertex), (void *)(11 * sizeof(float))); // bitangent

  VAO.Unbind();
  VBO.Unbind();
  EBO.Unbind();
  shader.Activate();
  VAO.Bind();

  // Keep track of how many of each type of textures we have
  unsigned int numDiffuse = 0;
  unsigned int numSpecular = 0;
  unsigned int numNormal = 0;
  for (unsigned int i = 0; i < textures.size(); i++) {
    std::string num;
    std::string type = textures[i].type;
    if (type == "diffuse")
      num = std::to_string(numDiffuse++);
    else if (type == "specular")
      num = std::to_string(numSpecular++);
    else if (type == "normal")
      num = std::to_string(numNormal++);
    textures[i].texUnit(shader, (type + num).c_str(), i);
    textures[i].Bind();
  }
  glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
  camera.Matrix(shader, "camMatrix");

  glm::mat4 rot = glm::mat4(1.0f);
  glm::mat4 scalingMatrix = glm::mat4(1.0f);
  glm::mat4 translationMatrix = glm::mat4(1.0f);

  rot = glm::rotate(rot, glm::radians(angleOfRotation), directionRot);
  scalingMatrix = glm::scale(scalingMatrix, glm::vec3(scale));
  translationMatrix = glm::translate(translationMatrix, (position));
  rotationMatrix = rot * rotationMatrix;

  int modelLoc = glGetUniformLocation(shader.ID, "rotation");
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(rotationMatrix));
  int scaleLoc = glGetUniformLocation(shader.ID, "scalingMatrix");
  glUniformMatrix4fv(scaleLoc, 1, GL_FALSE, glm::value_ptr(scalingMatrix));
  int tranLoc = glGetUniformLocation(shader.ID, "translationMatrix");
  glUniformMatrix4fv(tranLoc, 1, GL_FALSE, glm::value_ptr(translationMatrix));
  GLuint glight = glGetUniformLocation(shader.ID, "lightType");
  glUniform1i(glight, lightType);
  GLuint gnormal = glGetUniformLocation(shader.ID, "normal_enabled");
  glUniform1i(gnormal, normal_enabled);

  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Mesh::DrawAssistant(GLFWwindow *window, Shader &shader, Camera &camera) {
  if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS && !pressed_P) {
    pressed_P = true;
    lightType++;
    lightType = lightType % 3;
  }
  if (pressed_P && glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) {
    pressed_P = false;
  }
  if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && !pressed_N) {
    pressed_N = true;
    normal_enabled++;
    normal_enabled = normal_enabled % 2;
  }
  if (pressed_N && glfwGetKey(window, GLFW_KEY_N) == GLFW_RELEASE) {
    pressed_N = false;
  }
  if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && scale < 1.6) {
    scale += 0.001f;
  }
  if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS && scale > 0.5) {
    scale -= 0.001f;
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    position += glm::vec3(0.0f, -1.0f / sensibility, 0.0f);
  }
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    position += glm::vec3(0.0f, 1.0f / sensibility, 0.0f);
  }
  if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
    position += glm::vec3(-1.0f / sensibility, 0.0f, 0.0f);
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
    position += glm::vec3(1.0f / sensibility, 0.0f, 0.0f);
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
    position += glm::vec3(0.0f, 0.0f, 1.0f / sensibility);
  }
  if (glfwGetKey(window, GLFW_KEY_RIGHT_ALT) == GLFW_PRESS) {
    position += glm::vec3(0.0f, 0.0f, -1.0f / sensibility);
  }

  if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
    glm::vec3 newDir = position + (glm::cross(camera.Orientation, camera.Up) / sensibility);
    // std::cout << "ORI: " << camera.Orientation.x << "," << camera.Orientation.y << "," << camera.Orientation.z << std::endl;
    // std::cout << "VET: " << (-camera.Position + position).x << "," << (-camera.Position + position).y << "," << (-camera.Position + position).z << std::endl;
    // std::cout << "Angle: " << abs(glm::angle(glm::normalize(camera.Orientation), glm::normalize(-camera.Position + newDir))) << std::endl;
    bool valid_move = (abs(glm::angle(glm::normalize(camera.Orientation), glm::normalize(-camera.Position + position)) <= glm::radians(35.0f)));
    if (valid_move)
      position += glm::cross(camera.Orientation, camera.Up) / sensibility; // asse x della camera
  }

  if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
    glm::vec3 newPos = position + glm::vec3(0, 1, 0) / sensibility;
    glm::vec3 newOrient = (position - camera.Position);
    if (camera.CheckValidMove(newOrient)) {
      position = newPos;
      camera.Orientation = newOrient;
    }
  }

  if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
    if (camera.CheckValidMove((position - camera.Position)))
      camera.Orientation = (position - camera.Position);
  }
  if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
    position += glm::normalize(camera.Position - position) / sensibility;
  }

  angleOfRotation = 0.0f;
  if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
    angleOfRotation = 0.1f;
    glm::vec3 cameraDirection = glm::normalize(camera.Position - position);
    directionRot = glm::normalize(glm::cross(camera.Up, cameraDirection)); // asse x della camera
  }
  if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
    angleOfRotation = 0.1f;
    directionRot = glm::vec3(0.0f, 1.0f, 0.0f);
  }
  if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
    angleOfRotation = 0.1f;
    directionRot = glm::normalize(camera.Position - position);
  }
  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
    angleOfRotation = 0.1f;
    directionRot = glm::vec3(1.0f, 0.0f, 0.0f);
  }

  if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
    angleOfRotation = 0.1f;
    directionRot = glm::vec3(0.0f, 0.0f, 1.0f);
  }
  Draw(window, shader, camera);
}