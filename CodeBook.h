#ifndef BACKGROUNDSUBTRACTION_C_CODEBOOK_AAC_3_1_CODEBOOK_H_
#define BACKGROUNDSUBTRACTION_C_CODEBOOK_AAC_3_1_CODEBOOK_H_

#include "CodeWord.h"

class cCodeBook {
public:
  list<cCodeWord> mCodeBook;
public: // temporary variable
  int mNofP = 0; // Number of population
  double mR2_AVR;
  double mL2_AVR;
  double mCst;
  list<cCodeWord>::iterator mMatched;
  bool mIsMatched;
public:
  bitset<64> mFN;
  bitset<64> mFP;
  void fUpdateBufffer(bool seg, bool vote);
  double fCalculateRMODTN();
  double fCalculateRMODFP();
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

#endif
