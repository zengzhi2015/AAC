/*
 * CodeBook.h
 *
 *  Created on: 2015年12月15日
 *      Author: ZENG
 */

#ifndef BACKGROUNDSUBTRACTION_C_CODEBOOK_AAC3_0_CODEBOOK_H_
#define BACKGROUNDSUBTRACTION_C_CODEBOOK_AAC3_0_CODEBOOK_H_

#include "CodeWord.h"

class cCodeBook {
private:
  list<cCodeWord> mCodeBook;
private:
  double mTn = 25;
public: // temporary variable
  int mNofP = 0;
  double mR2_AVR;
  double mL2_AVR;
  double mCst;
  list<cCodeWord>::iterator mMatched;
  bool mIsMatched;
private:
  void fCalculateAVR();
  void fFilter();
public:
  cCodeBook();
  ~cCodeBook();
public:
  bool fCheckBook(double b, double g, double r);
  double fCalculatePsiF();
  void fUpdateBook(double b, double g, double r);
};



#endif /* BACKGROUNDSUBTRACTION_C_CODEBOOK_AAC3_0_CODEBOOK_H_ */
