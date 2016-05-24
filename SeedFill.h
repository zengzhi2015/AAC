#ifndef BACKGROUNDSUBTRACTION_C_CODEBOOK_AAC_3_1_SEEDFILL_H_
#define BACKGROUNDSUBTRACTION_C_CODEBOOK_AAC_3_1_SEEDFILL_H_

#include "CommonLibs.h"

class sSeedFill {
public:
  Mat mBinImg;
  Mat mLablePos;
  Mat mLableNeg;
  Mat mFiltedImg;
public:
  map<int,int> mAreaPosMap;
  map<int,int> mAreaNegMap;
public:
  int threshPos;
  int threshNeg;
public:
  sSeedFill();
  ~sSeedFill();
private:
  void fCore(Mat &binImg, Mat &labelImg, map<int,int> &AreaMap);
public:
  void fLabelPos();
  void fCalThresh();
  void fFilterPos();
  void fLabelNeg();
  void fFilterNeg();
public:
  void fAutoPost(Mat &bin, Mat &out);
};

#endif
