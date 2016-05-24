#ifndef BACKGROUNDSUBTRACTION_C_CODEBOOK_AAC_3_1_AACSEG_H_
#define BACKGROUNDSUBTRACTION_C_CODEBOOK_AAC_3_1_AACSEG_H_

#include "CodeBook.h"
#include "SeedFill.h"

class cAACSEG {
public:
  Mat mImage;
  Mat mRaw;
  Mat mResult;
  Mat mCst;
  Mat mRMOD;
  cCodeBook *mModel;
public:
  int mH;
  int mW;
  int mMAX;
  bool mInitialized = false;
  int mFrame = 0;
  RNG rng;
public: // Parameters
  int mTraining = 250;
public:
  cAACSEG();
  ~cAACSEG();
public:
  void fInitialize(const Mat &BGR);
  void fSeedfill(Mat &Gray, int filterSize, bool removePos);
  void fPostProc(Mat &Mask);
public:
  void fProcess(const Mat &BGR, Mat &BIN);
};



#endif
