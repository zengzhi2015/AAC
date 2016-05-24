#ifndef BACKGROUNDSUBTRACTION_C_CODEBOOK_AAC_3_1_FRAMEWORK_H_
#define BACKGROUNDSUBTRACTION_C_CODEBOOK_AAC_3_1_FRAMEWORK_H_

#include "AACSEG.h"

class cFramework{
public:
  String imageFolder; String imageNumRule;
  char imageName[200]; String imagePath; // "in%06d.jpg"
  int H_Image_down = 120;
  int W_Image_down = 160;
  Mat mShow;
public:
  cFramework();
  ~cFramework();
public:
  void BackgroundSubtraction();
};

#endif
