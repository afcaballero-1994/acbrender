#include"Texture.hpp"

#pragma clang diagnostic push 
#pragma clang diagnostic ignored "-Wsign-conversion"
#pragma clang diagnostic ignored "-Wcast-align"
#pragma clang diagnostic ignored "-Wimplicit-int-conversion"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#pragma clang diagnostic pop

Texture::Texture(char const *filename, const std::string& type) {
  glCreateTextures(GL_TEXTURE_2D, 1, &ID);
  glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER );
  glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER );
  glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int w, h, n;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *pixels = stbi_load(filename, &w, &h, &n, 0);

  if (pixels != NULL) {
    glTextureStorage2D(ID, 1, GL_RGB8, w, h);
    glTextureSubImage2D(ID, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    glGenerateTextureMipmap(ID);
    stbi_image_free(pixels);
    
  } else {
      std::cerr << "error loading texture" << std::endl;
      exit(1);
  }
}

void Texture::bind(unsigned int loc) {
  glBindTextureUnit(loc, ID);
}
