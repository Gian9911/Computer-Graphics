// #include <fstream>
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>
// #include <stb/stb_image.h>

#include "Mesh.h" // import glad
#include <GLFW/glfw3.h>
#include "imageProcessing.h"


const unsigned int width = 1900;
const unsigned int height = 1200;

Vertex vertices[] = {
    //           COORDINATES             /            TexCoord   //
    Vertex{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f)}, // Bottom side
    Vertex{glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},  // Bottom side
    Vertex{glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},   // Bottom side
    Vertex{glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(1.0f, 0.0f)},  // Bottom side

    Vertex{glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(0.0f, 0.0f)}, // up Side
    Vertex{glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},  // up Side
    Vertex{glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},   // up Side
    Vertex{glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 0.0f)},  // up Side

    Vertex{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f)}, // left side
    Vertex{glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},  // left side
    Vertex{glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},   // left side
    Vertex{glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 0.0f)},  // Left Side

    Vertex{glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},  // Right side
    Vertex{glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},   // Right side
    Vertex{glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f)},  // Right side
    Vertex{glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 1.0f)}, // Right Side

    Vertex{glm::vec3(-1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},  // Facing side
    Vertex{glm::vec3(-1.0f, -1.0f, 1.0f), glm::vec2(0.0f, 0.0f)}, // Facing side
    Vertex{glm::vec3(1.0f, -1.0f, 1.0f), glm::vec2(1.0f, 0.0f)},  // Facing side
    Vertex{glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},   // Facing Side

    Vertex{glm::vec3(-1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 0.0f)},  // -Facing side
    Vertex{glm::vec3(1.0f, 1.0f, -1.0f), glm::vec2(1.0f, 1.0f)},   // -Facing side
    Vertex{glm::vec3(-1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 0.0f)}, // -Facing side
    Vertex{glm::vec3(1.0f, -1.0f, -1.0f), glm::vec2(0.0f, 1.0f)}   // -Facing Side
};
GLuint indices[] = {2,  1,  0,              //
                    3,  2,  0,              //
                    6,  7,  4,              //
                    5,  6,  4,              //
                    9,  10, 11,             //
                    8,  9,  11,             //
                    14, 13, 12,             //
                    15, 14, 12,             //
                    16, 17, 18,             //
                    18, 19, 16,             //
                    20, 21, 23,             //
                    23, 22, 20};            //

Vertex lightVertices[] = {                  //     COORDINATES     //
    Vertex{glm::vec3(-0.1f, -0.1f, 0.1f)},  // 0 A
    Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)}, // 1 B
    Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},  // 2 C
    Vertex{glm::vec3(0.1f, -0.1f, 0.1f)},   // 3 D
    Vertex{glm::vec3(-0.1f, 0.1f, 0.1f)},   // 4 E
    Vertex{glm::vec3(-0.1f, 0.1f, -0.1f)},  // 5 F
    Vertex{glm::vec3(0.1f, 0.1f, -0.1f)},   // 6 G
    Vertex{glm::vec3(0.1f, 0.1f, 0.1f)}};   // 7 H

GLuint lightIndices[] = {2, 1, 0, 0, 3, 2, 0, 4, 7, 0, 7, 3, 3, 7, 6, 3, 6, 2, 2, 6, 5, 2, 5, 1, 1, 5, 4, 1, 4, 0, 4, 5, 6, 4, 6, 7};

void compute_TangentBitangent() {
  for (int i = 0; i < sizeof(indices)/sizeof(GLuint); i += 3) { 
    float edge0x = vertices[indices[i + 1]].position.x - vertices[indices[i]].position.x;
    float edge0y = vertices[indices[i + 1]].position.y - vertices[indices[i]].position.y;
    float edge0z = vertices[indices[i + 1]].position.z - vertices[indices[i]].position.z;

    float edge1x = vertices[indices[i + 2]].position.x - vertices[indices[i]].position.x;
    float edge1y = vertices[indices[i + 2]].position.y - vertices[indices[i]].position.y;
    float edge1z = vertices[indices[i + 2]].position.z - vertices[indices[i]].position.z;

    glm::vec3 edge0 = glm::vec3(edge0x, edge0y, edge0z);
    glm::vec3 edge1 = glm::vec3(edge1x, edge1y, edge1z);
    glm::vec2 deltaUV0 = vertices[indices[i + 1]].texUV - vertices[indices[i]].texUV;
    glm::vec2 deltaUV1 = vertices[indices[i + 2]].texUV - vertices[indices[i]].texUV;

    float invDet = 1.0f / (deltaUV0.x * deltaUV1.y - deltaUV1.x * deltaUV0.y);

    glm::vec3 tangent = glm::vec3(invDet * (deltaUV1.y * edge0 - deltaUV0.y * edge1));
    vertices[indices[i]].tangent = tangent;
    vertices[indices[i + 1]].tangent = tangent;
    vertices[indices[i + 2]].tangent = tangent;
    glm::vec3 bitangent = glm::vec3(invDet * (-deltaUV1.x * edge0 + deltaUV0.x * edge1));
    vertices[indices[i]].bitangent = bitangent;
    vertices[indices[i + 1]].bitangent = bitangent;
    vertices[indices[i + 2]].bitangent = bitangent;

    glm::vec3 cross = glm::normalize(glm::cross(edge0, edge1));
    vertices[indices[i]].normal = cross;
    vertices[indices[i + 1]].normal = cross;
    vertices[indices[i + 2]].normal = cross;
  }
}

int main() {

  glfwInit();
  unsigned int sample = 8;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_SAMPLES, sample);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window = glfwCreateWindow(width, height, "CG project", NULL, NULL);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);

  gladLoadGL();
  glViewport(0, 0, width, height);
  Shader shaderProgram("default.vert", "default.frag");
  std::string diffusePath = "textures/diffuse.png";
  ImageProcessing ip;
  ip.compute_normal_map(diffusePath, 1);
  std::string normalPath = "textures/normal_map.png";
  std::string normalPath1 = "textures/normal.png";
  Shader lightShader("light.vert", "light.frag");

  glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);
  glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 2.0f);
  glm::mat4 lightModel = glm::mat4(1.0f);
  lightModel = glm::translate(lightModel, lightPos);

  Texture textures[]{Texture(diffusePath.c_str(), "diffuse", 0), Texture(normalPath.c_str(), "normal", 1)};

  shaderProgram.Activate();
  glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
  glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

  lightShader.Activate();
  glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
  glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);

  compute_TangentBitangent();
  std::vector<Vertex> v(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
  std::vector<GLuint> i(indices, indices + sizeof(indices) / sizeof(GLuint));
  std::vector<Texture> t(textures, textures + sizeof(textures) / sizeof(Texture));

  std::vector<Texture> app{};

  std::vector<Vertex> lv(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
  std::vector<GLuint> li(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_MULTISAMPLE);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT);
  glFrontFace(GL_CW);

  glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 2.0f);
  Camera camera(width, height, cameraPos);

  Mesh mesh(v, i, t);
  Mesh meshLight(lv, li, app);

  double prevTime;
  double currTime;
  double timeDiff;
  unsigned int counter = 0;

  while (!glfwWindowShouldClose(window)) {
    currTime = glfwGetTime();
    timeDiff = currTime - prevTime;
    counter++;
    if (timeDiff >= 1.0 / 30.0) {
      std::string fps = std::to_string((1.0 / timeDiff) * counter);
      std::string ms = std::to_string(timeDiff / counter * 1000);
      std::string title = "FPS: " + fps + " ms: " + ms;
      glfwSetWindowTitle(window, title.c_str());
      prevTime = currTime;
      counter = 0;
    }
    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.Inputs(window);
    camera.updateMatrix(45.0f, 0.1f, 100.0f);

    meshLight.Draw(window, lightShader, camera);
    mesh.DrawAssistant(window, shaderProgram, camera);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  shaderProgram.Delete();
  lightShader.Delete();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}