#include "Framework.h"

cFramework::cFramework() {
  ;
}

cFramework::~cFramework() {
  ;
}

void cFramework::BackgroundSubtraction() {
  cAACSEG CBSEG;
  int H = H_Image_down;
  int W = W_Image_down;
  mShow.create(H,W*2,CV_8UC3);
  Mat binBGR(H,W,CV_8UC3,Scalar(0,0,0));

  for(int frameNumber = 0; frameNumber <= 50000; frameNumber++) {
    // read image
    sprintf(imageName, imageNumRule.c_str(), frameNumber);
    imagePath = imageFolder + (String)imageName;
    Mat img_input = imread(imagePath, IMREAD_COLOR);

    if (img_input.empty()) {
      continue;
    }
    else {
      resize(img_input,img_input,Size(W_Image_down,H_Image_down));
    }
    // segmentation
    Mat img_mask(img_input.size(),CV_8UC1,Scalar(0));
    CBSEG.fProcess(img_input, img_mask);

    cvtColor(img_mask, binBGR, CV_GRAY2BGR);
    img_input.copyTo(mShow(Rect(0,0,W,H)));
    binBGR.copyTo(mShow(Rect(W,0,W,H)));
    imshow("Segmentation",mShow);

    waitKey(10);
  }

  cvDestroyAllWindows();
}
