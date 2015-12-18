/*
 * AACSEG.h
 *
 *  Created on: 2015年12月15日
 *      Author: ZENG
 */

#ifndef BACKGROUNDSUBTRACTION_C_CODEBOOK_AAC3_0_AACSEG_H_
#define BACKGROUNDSUBTRACTION_C_CODEBOOK_AAC3_0_AACSEG_H_

#include "CodeBook.h"

class cAACSEG {
private:
  Mat mImage;
  Mat mRaw;
  Mat mResult;
  cCodeBook *mModel;
private:
  int mH;
  int mW;
  int mMAX;
  bool mInitialized = false;
  int mFrame = 0;
  int mTraining = 250; // Modify the number of training frames if nassasary
  RNG rng;
public:
  cAACSEG();
  ~cAACSEG();
private:
  void fInitialize(const Mat &BGR);
  void fSeedfill(Mat &Gray, int filterSize, bool removePos);
  void fPostProc(Mat &Mask);
public:
  void fProcess(const Mat &BGR, Mat &BIN);
};



#endif /* BACKGROUNDSUBTRACTION_C_CODEBOOK_AAC3_0_AACSEG_H_ */
