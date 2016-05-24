#include "CodeBook.h"

cCodeBook::cCodeBook() {
  ;
}

cCodeBook::~cCodeBook() {
  ;
}

void cCodeBook::fCalculateAVR() {
  if(mCodeBook.empty()) { // no code word yet
    mR2_AVR = 125;
    mL2_AVR = 400;
  }
  else {
    mR2_AVR = 0;
    mL2_AVR = 0;
    for (list<cCodeWord>::iterator it = mCodeBook.begin(); it != mCodeBook.end(); it++) {
      mR2_AVR += it->mR2 * it->mN;
      mL2_AVR += it->mL2 * it->mN;
    }
    mR2_AVR /= mNofP;
    mL2_AVR /= mNofP;
  }
}

double cCodeBook::fCalculatePsiF() {
  double m = 999999;
  double t;
  for(list<cCodeWord>::iterator it = mCodeBook.begin();it != mCodeBook.end();it++) {
    t = MAX(1, it->mU2/it->mL2) * MAX(1, it->mV2/it->mR2) * MAX(1, it->mV2/it->mR2);
    if(t < m) {
      m = t;
    }
  }
  mCst = MIN((sqrt(m)-1)/2,0.1);

  return mCst;
}

void cCodeBook::fUpdateBufffer(bool seg, bool vote) {
  if(seg == 0 && vote == 0) {
    mFP = mFP << 1;
    mFN = mFN << 1;
    mFP[20] = 0;
    mFN[20] = 0;
  }
  if(seg == 1 && vote == 0) {
    mFP = mFP << 1; mFP[0] = 1;
    mFN = mFN << 1;
    mFP[20] = 0;
    mFN[20] = 0;
  }
  if(seg == 1 && vote == 1) {
    mFP = mFP << 1;
    mFN = mFN << 1;
    mFP[20] = 0;
    mFN[20] = 0;
  }
  if(seg == 0 && vote == 1) {
    mFP = mFP << 1;
    mFN = mFN << 1; mFN[0] = 1;
    mFP[20] = 0;
    mFN[20] = 0;
  }
}

double cCodeBook::fCalculateRMODTN() {
  double rate_mod;
  rate_mod = exp(-(double)mFN.count());

  return rate_mod;
}

double cCodeBook::fCalculateRMODFP() {
  double rate_mod;
  rate_mod = (double)mFP.count()/20.0;
  rate_mod = 1 + rate_mod*rate_mod*2;
  rate_mod = rate_mod*exp(-(double)mFN.count());

  return rate_mod;
}

void cCodeBook::fFilter() {
  if(!mCodeBook.empty()) {
    while(mNofP > 25) {
      int tRand = rand()%mNofP;
      int tSum = 0;
      list<cCodeWord>::iterator it = mCodeBook.begin();
      while(it != mCodeBook.end()) { // filter model
        tSum += it->mN;
        if(tSum >= tRand) {
          it->mN -= 1;
          mNofP -= 1;
          if(it->mN <= 0) {
            mCodeBook.erase(it++);
          }
          break;
        }
        else {
          it++;
        }
      }
    }
  }
}

bool cCodeBook::fCheckBook(double b, double g, double r) {
  if(mCodeBook.empty()) { // no code word yet
    mIsMatched = false;
    return false;
  }
  else {
    mIsMatched = false;
    list<cCodeWord>::iterator it = mCodeBook.begin();
    while(it != mCodeBook.end()) { // search for matching
      if(it->fCheckMatch(b, g, r)) { // matched
        mIsMatched = true;
        mMatched = it;
        return true;
      }
      else { // not matched
        it++;
      }
    }
    return false;
  }
}

void cCodeBook::fUpdateBook(double b, double g, double r) {
  if(mIsMatched == false) {
    fCalculateAVR();
    cCodeWord code;
    code.fInitialize(b,g,r,mR2_AVR,mL2_AVR);
    mCodeBook.push_back(code); mNofP += 1;
  }
  else {
    mMatched->fUpdate(); mNofP += 1;
  }
  fFilter();
}
