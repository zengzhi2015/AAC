#include "CodeWord.h"

cCodeWord::cCodeWord() {
  ;
}

cCodeWord::~cCodeWord() {
  ;
}

void cCodeWord::fInitialize(double b, double g, double r, double r2_init, double l2_inite) {
  mCb = b;
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
  mCb += mCPb * 0.1;
  mCg += mCPg * 0.1;
  mCr += mCPr * 0.1;
  double temp_L = sqrt(mL2);
  if(mU >= 0) {
    mDb = mDb*temp_L + (mCPb - mDb*temp_L) * 0.1;
    mDg = mDg*temp_L + (mCPg - mDg*temp_L) * 0.1;
    mDr = mDr*temp_L + (mCPr - mDr*temp_L) * 0.1;
  }
  else {
    mDb = mDb*temp_L - (mCPb - mDb*temp_L) * 0.1;
    mDg = mDg*temp_L - (mCPg - mDg*temp_L) * 0.1;
    mDr = mDr*temp_L - (mCPr - mDr*temp_L) * 0.1;
  }
  double tLD = sqrt(mDb*mDb + mDg*mDg + mDr*mDr);
  mDb /= tLD;
  mDg /= tLD;
  mDr /= tLD;
  mR2 += (9*mV2 - mR2) * 0.1; mR2 = MAX(125,MIN(1600,mR2));
  mL2 += (9*mU2 - mL2) * 0.1; mL2 = MAX(125,MIN(10000,mL2));
  mN += 1;
}
