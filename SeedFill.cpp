#include "SeedFill.h"

sSeedFill::sSeedFill() {
  ;
}


sSeedFill::~sSeedFill() {
  ;
}

void sSeedFill::fCore(Mat &binImg, Mat &labelImg, map<int,int> &AreaMap) {
  const int H = binImg.rows;
  const int W = binImg.cols;
  const int max = H*W;

  labelImg.create(binImg.size(),CV_32SC1);
  binImg.convertTo(labelImg, CV_32SC1, -1);
  AreaMap.clear();

  int label = 1;
  int *p_labelImg;
  int sum;
  for (int i = 0; i < H; i++) {
    for (int j = 0; j < W; j++) {
      p_labelImg = (int*)labelImg.data + i*W + j;
      if (*p_labelImg == -255) {
        stack<pair<int,int>> neighborPixels;
        neighborPixels.push(pair<int,int>(i,j));
        sum = 0;
        while(!neighborPixels.empty()) {
          pair<int,int> curPixel = neighborPixels.top();
          int curY = curPixel.first;
          int curX = curPixel.second;
          *((int*)labelImg.data + curY*W + curX) = label;
          sum += 1;
          neighborPixels.pop() ;
          if(curX==0) {
            if(curY==0) {
              if(*((int*)labelImg.data + curY*W + curX+1) == -255) {
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)labelImg.data + (curY+1)*W + curX) == -255) {
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
            if(curY==H-1) {
              if(*((int*)labelImg.data + curY*W + curX+1) == -255) {
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)labelImg.data + (curY-1)*W + curX) == -255) {
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
            }
            if((curY<H-1)&(curY>0)) {
              if(*((int*)labelImg.data + curY*W + curX+1) == -255) {
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)labelImg.data + (curY-1)*W + curX) == -255) {
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
              if(*((int*)labelImg.data + (curY+1)*W + curX) == -255) {
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
          }
          if(curX==W-1) {
            if(curY==0) {
              if(*((int*)labelImg.data + curY*W + curX-1) == -255) {
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)labelImg.data + (curY+1)*W + curX) == -255) {
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
            if(curY==H-1) {
              if(*((int*)labelImg.data + curY*W + curX-1) == -255) {
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)labelImg.data + (curY-1)*W + curX) == -255) {
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
            }
            if((curY<H-1)&(curY>0)) {
              if(*((int*)labelImg.data + curY*W + curX-1) == -255) {
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)labelImg.data + (curY-1)*W + curX) == -255) {
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
              if(*((int*)labelImg.data + (curY+1)*W + curX) == -255) {
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
          }
          if((curX<W-1)&(curX>0)) {
            if(curY==0) {
              if(*((int*)labelImg.data + curY*W + curX-1) == -255) {
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)labelImg.data + curY*W + curX+1) == -255) {
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)labelImg.data + (curY+1)*W + curX) == -255) {
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
            if(curY==H-1) {
              if(*((int*)labelImg.data + curY*W + curX-1) == -255) {
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)labelImg.data + curY*W + curX+1) == -255) {
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)labelImg.data + (curY-1)*W + curX) == -255) {
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
            }
            if((curY<H-1)&(curY>0)) {
              if(*((int*)labelImg.data + curY*W + curX-1) == -255) {
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)labelImg.data + curY*W + curX+1) == -255) {
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)labelImg.data + (curY-1)*W + curX) == -255) {
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
              if(*((int*)labelImg.data + (curY+1)*W + curX) == -255) {
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
          }
        }
        AreaMap[label] = sum;
        label += 1;
      }
    }
  }
}

void sSeedFill::fLabelPos() {
  Mat tempImg;
  tempImg = mBinImg > 0;
  fCore(tempImg, mLablePos, mAreaPosMap);
}

void sSeedFill::fLabelNeg() {
  Mat tempImg;
  tempImg = mFiltedImg == 0;
  fCore(tempImg, mLableNeg, mAreaNegMap);
}

void sSeedFill::fCalThresh() {
  vector<int> V;
  for(auto elem: mAreaPosMap) {
    V.push_back(elem.second);
  }
  sort(V.begin(),V.end());
  int P = round((double)V.size()*0.995);
  P = MAX(MIN(V.size()-1,P),V.size()-10);
  if(V.empty()) {
    threshPos = -1;
    threshNeg = -1;
  }
  else {
    threshPos = MAX(MIN(V[P],mBinImg.cols*mBinImg.rows/300),mBinImg.cols*mBinImg.rows/1000);
    threshNeg = threshPos*2;
  }
}

void sSeedFill::fFilterPos() {
  mBinImg.copyTo(mFiltedImg);
  int *p_labelPos = (int*)mLablePos.data;
  uchar *p_FiltedImg = mFiltedImg.data;

  int max = mFiltedImg.cols * mFiltedImg.rows;
  for(int i = 0; i < max; i++) {
    if(*p_FiltedImg > 0) {
      if(mAreaPosMap[*p_labelPos] <= threshPos) {
        *p_FiltedImg = 0;
      }
    }

    p_labelPos += 1;
    p_FiltedImg += 1;
  }
}

void sSeedFill::fFilterNeg() {
  int *p_labelNeg = (int*)mLableNeg.data;;
  uchar *p_FiltedImg = mFiltedImg.data;
  int max = mFiltedImg.cols * mFiltedImg.rows;
  for(int i = 0; i < max; i++) {
    if(*p_FiltedImg == 0) {
      if(mAreaNegMap[*p_labelNeg] < threshNeg) {
        *p_FiltedImg = 255;
      }
    }
    p_labelNeg += 1;
    p_FiltedImg += 1;
  }
}

void sSeedFill::fAutoPost(Mat &bin, Mat &out) {
  bin.copyTo(mBinImg);
  fLabelPos();
  fCalThresh();
  fFilterPos();
  fLabelNeg();
  fFilterNeg();
  mFiltedImg.copyTo(out);
}
