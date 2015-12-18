/*
 * CodeWord.cpp
 *
 *  Created on: 2015年12月15日
 *      Author: ZENG
 */

#include "CodeWord.h"

cCodeWord::cCodeWord() {
  ;
}

cCodeWord::~cCodeWord() {
  ;
}

void cCodeWord::fInitialize(double b, double g, double r, double r2_init, double l2_inite) {
  mCb = b; // RGB
  mCg = g;
  mCr = r;
  double tLP = sqrt(b*b+g*g+r*r);
  mDb = mCb/tLP;
  mDg = mCg/tLP;
  mDr = mCr/tLP;
  mR2 = r2_init; mR2 = MAX(125,MIN(400,mR2));
  mL2 = l2_inite; mL2 = MAX(125,MIN(400,mL2));
  mN = 1;
}

bool cCodeWord::fCheckMatch(double b, double g, double r) {
  mCPb = b - mCb;
  mCPg = g - mCg;
  mCPr = r - mCr;
  mLCP2 = mCPb*mCPb + mCPg*mCPg + mCPr*mCPr;
  mU = mCPb*mDb + mCPg*mDg + mCPr*mDr;
  mU2 = mU*mU;
  mV2 = mLCP2 - mU2;

  if(mU2 <= mL2 && mV2 <= mR2) {
    return true;
  }
  else {
    return false;
  }
}

void cCodeWord::fUpdate() {
  mCb += mCPb * mArpha;
  mCg += mCPg * mArpha;
  mCr += mCPr * mArpha;
  if(mU2*4>mL2||mV2*4>mR2) {
    if(mU >= 0) {
      mDb += (mCPb - mDb) * mArpha;
      mDg += (mCPg - mDg) * mArpha;
      mDr += (mCPr - mDr) * mArpha;
    }
    else {
      mDb -= (mCPb + mDb) * mArpha;
      mDg -= (mCPg + mDg) * mArpha;
      mDr -= (mCPr + mDr) * mArpha;
    }
  }
  double tLD = sqrt(mDb*mDb + mDg*mDg + mDr*mDr);
  mDb /= tLD;
  mDg /= tLD;
  mDr /= tLD;
  mR2 += (9*mV2 - mR2) * mArpha; mR2 = MAX(125,MIN(400,mR2));
  mL2 += (9*mU2 - mL2) * mArpha; mL2 = MAX(125,MIN(400,mL2));
  mN += 1;
}


