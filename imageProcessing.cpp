#include "imageProcessing.h"

std::vector<Gray> ImageProcessing::rgbToGray(unsigned char *diffuse_data, int width, int height) {
  std::vector<Color> diffuse_map(width * height);
  for (int i = 0; i < width * height; i++) {
    diffuse_map[i].r = diffuse_data[i * 3];
    diffuse_map[i].g = diffuse_data[i * 3 + 1];
    diffuse_map[i].b = diffuse_data[i * 3 + 2];
  }
  std::vector<Gray> gray_img(width * height);
  int index_gray = 0;
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      int index = j + width * i;
      // 0.299 ∙ Red + 0.587 ∙ Green + 0.114 ∙ Blue
      gray_img[index].gray = 0.299f * diffuse_map[index].r + 0.587f * diffuse_map[index].g + 0.114f * diffuse_map[index].b;
    }
  }
  return gray_img;
}
// Function to compute the normal map
ImageProcessing::ImageProcessing(){};

std::vector<Gray> ImageProcessing::apply_padding_to_image(std::vector<Gray> img, const int kernel_size, type_padding padding, int height, int width) {
  const int shift = kernel_size / 2;
  const int new_width = width + kernel_size - 1;
  const int new_height = height + kernel_size - 1;
  std::vector<Gray> padded_img_matrix(new_width * new_height);
  const int OLD_WIDTH = width;
  const int OLD_HEIGHT = height;
  switch (padding) {
  case zero:
    for (int i = 0; i < new_height; i++) {
      for (int j = 0; j < new_width; j++) {
        padded_img_matrix[i * new_width + j].gray = 0;
      }
    }
    for (int i = 0; i < OLD_HEIGHT; i++)
      for (int j = 0; j < OLD_WIDTH; j++)
        padded_img_matrix[(i + shift) * new_width + j + shift].gray = img[i * OLD_WIDTH + j].gray;
    break;
  case pixel_mirroring:
  case pixel_replication:
    for (int i = 0; i < OLD_HEIGHT; i++)
      for (int j = 0; j < OLD_WIDTH; j++)
        padded_img_matrix[(i + shift) * new_width + j + shift].gray = img[i * OLD_WIDTH + j].gray;
    for (int i = 0; i < shift; i++)
      for (int j = 0; j < OLD_WIDTH; j++) {
        int shift_padding_row = padding == pixel_mirroring ? shift - i : 0;
        padded_img_matrix[i * new_width + j + shift].gray = img[(shift_padding_row)*OLD_WIDTH + j].gray;                                         //[0 * OLD_WIDTH + j]
        padded_img_matrix[(new_height - 1 - i) * new_width + j + shift].gray = img[(OLD_HEIGHT - 1 - (shift_padding_row)) * OLD_WIDTH + j].gray; //[(OLD_HEIGHT - 1 - 0) * OLD_WIDTH + j]
      }
    for (int i = 0; i < OLD_HEIGHT; i++)
      for (int j = 0; j < shift; j++) {
        int shift_padding_col = padding == pixel_mirroring ? shift - j : 0;
        padded_img_matrix[(i + shift) * new_width + j].gray = img[i * OLD_WIDTH + (shift_padding_col)].gray;                                 // [i * OLD_WIDTH + 0]
        padded_img_matrix[(i + shift) * new_width + new_width - 1 - j].gray = img[i * OLD_WIDTH + OLD_WIDTH - 1 - (shift_padding_col)].gray; //[i * OLD_WIDTH + OLD_WIDTH - 1 - 0]
      }
    // copy corners top-left,top-right,bottom-left,bottom-right
    if (padding == pixel_mirroring)
      for (int i = 0; i < shift; i++)
        for (int j = 0; j < shift; j++) {
          int shift_padding_row = shift - i;
          int shift_padding_col = shift - j;
          padded_img_matrix[i * new_width + j].gray = img[(shift_padding_row)*OLD_WIDTH + (shift_padding_col)].gray;                                       // [0 * OLD_WIDTH + 0]
          padded_img_matrix[i * new_width + new_width - 1 - j].gray = img[(shift_padding_row)*OLD_WIDTH + OLD_WIDTH - (shift_padding_col)-1].gray;         // [0 * OLD_WIDTH + OLD_WIDTH - j - 0]
          padded_img_matrix[(new_height - 1 - i) * new_width + j].gray = img[(OLD_HEIGHT - (shift_padding_row)-1) * OLD_WIDTH + (shift_padding_col)].gray; // [(OLD_HEIGHT - 0 - 1) * OLD_WIDTH + 0]
          padded_img_matrix[(new_height - 1 - i) * new_width + (new_width - 1 - j)].gray =
              img[(OLD_HEIGHT - (shift_padding_row)-1) * OLD_WIDTH + (OLD_WIDTH - (shift_padding_col)-1)].gray; // [(OLD_HEIGHT - 0 - 1) * OLD_WIDTH + (OLD_WIDTH - 0 - 1)]
        }
    else
      for (int i = 0; i < shift; i++)
        for (int j = 0; j < shift; j++) {
          padded_img_matrix[i * new_width + j].gray = img[0].gray;                                               // [0 * OLD_WIDTH + 0]
          padded_img_matrix[i * new_width + new_width - 1 - j].gray = img[OLD_WIDTH - 1].gray;                   // [0 * OLD_WIDTH + OLD_WIDTH - j - 0]
          padded_img_matrix[(new_height - 1 - i) * new_width + j].gray = img[(OLD_HEIGHT - 1) * OLD_WIDTH].gray; // [(OLD_HEIGHT - 0 - 1) * OLD_WIDTH + 0]
          padded_img_matrix[(new_height - 1 - i) * new_width + (new_width - 1 - j)].gray =
              img[(OLD_HEIGHT - 1) * OLD_WIDTH + (OLD_WIDTH - 1)].gray; // [(OLD_HEIGHT - 0 - 1) * OLD_WIDTH + (OLD_WIDTH - 0 - 1)]
        }
    break;
  }
  return padded_img_matrix;
}
/* Create normal map in textures/normal_map*/
int ImageProcessing::compute_normal_map(std::string diffusePath, double strength, type_padding padding) {
  int width, height, channels;
  unsigned char *diffuse_data = stbi_load(diffusePath.c_str(), &width, &height, &channels, 3);
  std::vector<Color> normal_map(width * height);
  std::vector<Gray> map = apply_padding_to_image(rgbToGray(diffuse_data, width, height), 3, padding, height, width);
  // map(width+2,height+2)  normal_map(width,height)
  for (int y = 1; y < height + 1; y++) {
    for (int x = 1; x < width + 1; x++) {
      // Calculate gradients in the x and y directions
      int gx = (map[(y - 1) * (width + 2) + (x + 1)].gray + 2 * map[y * (width + 2) + (x + 1)].gray + map[(y + 1) * (width + 2) + (x + 1)].gray) -
               (map[(y - 1) * (width + 2) + (x - 1)].gray + 2 * map[y * (width + 2) + (x - 1)].gray + map[(y + 1) * (width + 2) + (x - 1)].gray);

      int gy = (map[(y + 1) * (width + 2) + (x - 1)].gray + 2 * map[(y + 1) * (width + 2) + x].gray + map[(y + 1) * (width + 2) + (x + 1)].gray) -
               (map[(y - 1) * (width + 2) + (x - 1)].gray + 2 * map[(y - 1) * (width + 2) + x].gray + map[(y - 1) * (width + 2) + (x + 1)].gray);

      // Calculate the normal vector
      double length = std::sqrt(gx * gx + gy * gy + 255 * 255); // 255 is the depth range (maximum intensity value) [gx,gy,255]
      double nx = -gx * strength / length;
      double ny = -gy * strength / length; 
      double nz = 1.0;

      // Normalize the normal vector
      double norm = std::sqrt(nx * nx + ny * ny + nz * nz);
      nx /= norm;
      ny /= norm;
      nz /= norm;

      // Convert the normal vector to RGB values
      normal_map[(y - 1) * width + (x - 1)].r = static_cast<unsigned char>((nx + 1.0) * 0.5 * 255);
      normal_map[(y - 1) * width + (x - 1)].g = static_cast<unsigned char>((ny + 1.0) * 0.5 * 255);
      normal_map[(y - 1) * width + (x - 1)].b = static_cast<unsigned char>((nz + 1.0) * 0.5 * 255);
    }
  }
  if (!stbi_write_png("textures/normal_map.png", width, height, 3, normal_map.data(), width * 3)) {
    std::cout << "Failed to write the image." << std::endl;
    return 1;
  } else
    return 0;
}