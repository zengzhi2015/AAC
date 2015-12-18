/*
 * CodeWord.h
 *
 *  Created on: 2015年12月15日
 *      Author: ZENG
 */

#ifndef BACKGROUNDSUBTRACTION_C_CODEBOOK_AAC3_0_CODEWORD_H_
#define BACKGROUNDSUBTRACTION_C_CODEBOOK_AAC3_0_CODEWORD_H_

#include "CommonLibs.h"

class cCodeWord {
public: // code word
  double mCb;
  double mCg;
  double mCr;
  double mDb;
  double mDg;
  double mDr;
  double mR2;
  double mL2;
  double mN;
public: // temporary variables
  double mCPb;
  double mCPg;
  double mCPr;
  double mLCP;
  double mLCP2;
  double mU;
  double mU2;
  double mV2;
private:
  double mArpha = 0.1;
public:
  cCodeWord();
  ~cCodeWord();
public:
  void fInitialize(double b, double g, double r, double r2_init, double l2_init);
  bool fCheckMatch(double b, double g, double r);
  void fUpdate();
};



#endif /* BACKGROUNDSUBTRACTION_C_CODEBOOK_AAC3_0_CODEWORD_H_ */
