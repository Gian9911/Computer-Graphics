#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include "Libraries/include/stb/stb_image.h"
#include "Libraries/include/stb/stb_image_write.h"
#include <cmath>
#include <iostream>
#include <vector>

struct Color {
  unsigned char r=0, g=0, b=0;
};
struct Gray {
  unsigned char gray;
};
enum type_padding { zero, pixel_replication, pixel_mirroring };

class ImageProcessing {
private:
  std::vector<Gray> rgbToGray(unsigned char *diffuse_data, int width, int height);
  std::vector<Gray> apply_padding_to_image(std::vector<Gray> img, const int kernel_size, type_padding padding,int height,int width);
public:
  ImageProcessing();
  int compute_normal_map(std::string diffusePath, double strength,type_padding padding=zero);
};

#endif