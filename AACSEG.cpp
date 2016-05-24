#include "AACSEG.h"

cAACSEG::cAACSEG() {
  mModel = NULL;
}

cAACSEG::~cAACSEG() {
  delete [] mModel;
}

void cAACSEG::fInitialize(const Mat &BGR) {
  mH = BGR.rows;
  mW = BGR.cols;
  mMAX = mH*mW;
  mImage.create(BGR.size(),CV_8UC3);
  mRaw.create(BGR.size(),CV_8UC1);
  mResult.create(BGR.size(),CV_8UC1);
  mModel = new cCodeBook[mMAX];
  mCst.create(BGR.size(),CV_8UC1);
  mRMOD.create(BGR.size(),CV_8UC1);
  mInitialized = true;
}

void cAACSEG::fProcess(const Mat &BGR, Mat &BIN) {
  mFrame += 1;
  if(!mInitialized) {
    fInitialize(BGR);
  }
  BGR.copyTo(mImage);
  uchar *pImage;
  uchar *pRaw;
  uchar *pResult;
  cCodeBook *pModel;
  if(mFrame <= mTraining) { // training
    pImage = mImage.data;
    pModel = mModel;
    for (int i = 0; i < mMAX; ++i) {
      pModel->fCheckBook(*pImage,*(pImage+1),*(pImage+2));
      pModel->fUpdateBook(*pImage,*(pImage+1),*(pImage+2));

      pImage += 3;
      pModel += 1;
    }
    mRaw = mRaw*0;
    mRaw.copyTo(mResult);
  }
  else { // Seg
    pImage = mImage.data;
    pRaw = mRaw.data;
    pResult = mResult.data;
    pModel = mModel;
    for (int i = 0; i < mMAX; ++i) {
      if(pModel->fCheckBook(*pImage,*(pImage+1),*(pImage+2))) { // is matched
        *pRaw = 0;
      }
      else { // not matched
        *pRaw = 255;
      }
      pImage += 3;
      pRaw += 1;
      pModel += 1;
    }

    Mat Temp;
    mRaw.copyTo(Temp);
    sSeedFill postproc;
    postproc.fAutoPost(Temp,Temp);
    pImage = mImage.data;
    pRaw = mRaw.data;
    uchar *pTemp = Temp.data;
    pModel = mModel;
    uchar *pmCst = mCst.data;
    mCst = mCst*0;
    uchar *pRMOD = mRMOD.data;
    mRMOD = mRMOD*0;

    double rate_mod;
    for (int i = 0; i < mMAX; i++) {
      if(*pRaw == 0 && *pTemp == 0) {
        pModel->fUpdateBufffer(0,0);
        rate_mod = pModel->fCalculateRMODTN();
        rate_mod = MAX(rate_mod,0);
        *pRMOD = MIN(255,127.0 * rate_mod);
        if(rng.uniform(0.,1.) <= 0.01*rate_mod) {
          pModel->fUpdateBook(*pImage,*(pImage+1),*(pImage+2));
        }
      }
      if(*pRaw == 255 && *pTemp == 0) {
        pModel->fUpdateBufffer(1,0);
        rate_mod = pModel->fCalculateRMODFP();
        rate_mod = MAX(rate_mod,0);
        *pRMOD = MIN(255,127.0 * rate_mod);
        if(rng.uniform(0.,1.) <= pModel->fCalculatePsiF()*rate_mod) {
          pModel->fUpdateBook(*pImage,*(pImage+1),*(pImage+2));
        }
        *pmCst = MIN(255,1000*pModel->mCst);
      }
      if(*pRaw == 255 && *pTemp == 255) {
        pModel->fUpdateBufffer(1,1);
        if(rng.uniform(0.,1.) <= 0.0002) {
          pModel->fUpdateBook(*pImage,*(pImage+1),*(pImage+2));
        }
      }
      if(*pRaw == 0 && *pTemp == 255) {
        pModel->fUpdateBufffer(0,1);
      }

      pImage += 3;
      pRaw += 1;
      pTemp += 1;
      pModel += 1;
      pmCst += 1;
      pRMOD += 1;
    }

    mResult.copyTo(BIN);
    Temp.copyTo(mResult);
    fPostProc(mResult);

  }

}

void cAACSEG::fPostProc(Mat &Mask) {
  int H = Mask.rows;
  int W = Mask.cols;
  int size = W*H/500;

  medianBlur(Mask, Mask, 3);
  erode(Mask,Mask,Mat(),Point(-1,-1),1);
  fSeedfill(Mask, size, true);
  dilate(Mask,Mask,Mat(),Point(-1,-1),1);
  medianBlur(Mask, Mask, 3);

}

void cAACSEG::fSeedfill(Mat &Gray, int filterSize, bool removePos) {
  const int H = Gray.rows;
  const int W = Gray.cols;
  const int max = H*W;
  Mat lableImg(H,W,CV_32SC1,Scalar(0));
  Mat tempImg(H,W,CV_8UC1,Scalar(0));
  int fill_color;
  if(removePos) {
    tempImg = Gray > 0;
    tempImg.convertTo(lableImg, CV_32SC1, 0.003921);
    fill_color = 0;
  }
  else {
    tempImg = Gray == 0;
    tempImg.convertTo(lableImg, CV_32SC1, 0.003921);
    fill_color = 255;
  }
  int label = 2;
  int *p_lableImg;
  int Areas[10000];
  int sum;
  for (int i = 0; i < H; i++) {
    for (int j = 0; j < W; j++) {
      p_lableImg = (int*)lableImg.data + i*W + j;
      if (*p_lableImg == 1) {
        stack<pair<int,int>> neighborPixels;
        neighborPixels.push(pair<int,int>(i,j));
        sum = 0;
        while(!neighborPixels.empty()) {
          pair<int,int> curPixel = neighborPixels.top();
          int curY = curPixel.first;
          int curX = curPixel.second;
          *((int*)lableImg.data + curY*W + curX) = label;
          sum += 1;
          neighborPixels.pop() ;
          if(curX==0) {
            if(curY==0) {
              if(*((int*)lableImg.data + curY*W + curX+1) == 1) {
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)lableImg.data + (curY+1)*W + curX) == 1) {
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
            if(curY==H-1) {
              if(*((int*)lableImg.data + curY*W + curX+1) == 1) {
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)lableImg.data + (curY-1)*W + curX) == 1) {
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
            }
            if((curY<H-1)&(curY>0)) {
              if(*((int*)lableImg.data + curY*W + curX+1) == 1) {
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)lableImg.data + (curY-1)*W + curX) == 1) {
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
              if(*((int*)lableImg.data + (curY+1)*W + curX) == 1) {
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
          }
          if(curX==W-1) {
            if(curY==0) {
              if(*((int*)lableImg.data + curY*W + curX-1) == 1) {
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)lableImg.data + (curY+1)*W + curX) == 1) {
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
            if(curY==H-1) {
              if(*((int*)lableImg.data + curY*W + curX-1) == 1) {
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)lableImg.data + (curY-1)*W + curX) == 1) {
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
            }
            if((curY<H-1)&(curY>0)) {
              if(*((int*)lableImg.data + curY*W + curX-1) == 1) {
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)lableImg.data + (curY-1)*W + curX) == 1) {
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
              if(*((int*)lableImg.data + (curY+1)*W + curX) == 1) {
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
          }
          if((curX<W-1)&(curX>0)) {
            if(curY==0) {
              if(*((int*)lableImg.data + curY*W + curX-1) == 1) {
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)lableImg.data + curY*W + curX+1) == 1) {
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)lableImg.data + (curY+1)*W + curX) == 1) {
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
            if(curY==H-1) {
              if(*((int*)lableImg.data + curY*W + curX-1) == 1) {
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)lableImg.data + curY*W + curX+1) == 1) {
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)lableImg.data + (curY-1)*W + curX) == 1) {
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
            }
            if((curY<H-1)&(curY>0)) {
              if(*((int*)lableImg.data + curY*W + curX-1) == 1) {
                neighborPixels.push(pair<int,int>(curY, curX-1)) ;
              }
              if(*((int*)lableImg.data + curY*W + curX+1) == 1) {
                neighborPixels.push(pair<int,int>(curY, curX+1)) ;
              }
              if(*((int*)lableImg.data + (curY-1)*W + curX) == 1) {
                neighborPixels.push(pair<int,int>(curY-1, curX)) ;
              }
              if(*((int*)lableImg.data + (curY+1)*W + curX) == 1) {
                neighborPixels.push(std::pair<int,int>(curY+1, curX)) ;
              }
            }
          }
        }
        Areas[label] = sum;
        label += 1;
      }
    }
  }
  uchar *p_Gray;
  p_lableImg = (int*)lableImg.data;
  p_Gray = Gray.data;
  for(int i = 0; i < max; i++) {
    if(*p_lableImg > 0) {
      if(Areas[*p_lableImg] < filterSize) {
        *p_Gray = fill_color;
      }
    }
    p_lableImg += 1;
    p_Gray += 1;
  }
}


