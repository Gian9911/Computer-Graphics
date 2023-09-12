#include "Texture.h"

Texture::Texture(const char *image, const char *texType, GLuint slot) {
  glGenTextures(1, &ID);
  int width, height, nrComponents;
  unsigned char *data = stbi_load(image, &width, &height, &nrComponents, 0);
  // std::cout<<"W:" <<width<<"H:"<<height<<std::endl;
  if (data) {
    type = texType;
    unit = slot;
    GLenum format;
    if (nrComponents == 1)
      format = GL_RED;
    else if (nrComponents == 3)
      format = GL_RGB;
    else if (nrComponents == 4)
      format = GL_RGBA;

    glBindTexture(GL_TEXTURE_2D, ID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  } else {
    std::cout << "Texture failed to load at path: " << image << std::endl;
    stbi_image_free(data);
  }
}

void Texture::texUnit(Shader &shader, const char *uniform, GLuint unit) {
  // shader needs to be activated before changing the value of a uniform
  shader.Activate();
  GLuint texUni = glGetUniformLocation(shader.ID, uniform);
  glUniform1i(texUni, unit);
}

void Texture::Bind() {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind() { glBindTexture(GL_TEXTURE_2D, 0); }
void Texture::Delete() { glDeleteTextures(1, &ID); }