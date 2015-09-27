//
//  ImageFiltersUtilityClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 9/22/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "ImageFiltersUtilityClass.h"

ImageFiltersUtilityClass::ImageFiltersUtilityClass():gaussianClass(nullptr) {

}


Eigen::MatrixXd ImageFiltersUtilityClass::GetLaplacianOfGaussianFilterMatrix(const int& windowRadiusSize) {
    Eigen::MatrixXd LOGFilterMatrix = Eigen::MatrixXd(2*windowRadiusSize+1, 2*windowRadiusSize+1);
    
   /*
    double sigma = 1.4;
    double sum = 0.0;
    for (int row = -windowRadiusSize; row <= windowRadiusSize; ++row) {
        for (int col = -windowRadiusSize; col <= windowRadiusSize; ++col) {
            double value1 = (double)-1 / (double)(M_PI* pow(sigma, 4));
            double value2 = (double)-1*(row*row + col*col)/(double)(2*sigma*sigma);
            LOGFilterMatrix(row+windowRadiusSize,col+windowRadiusSize) = value1 * (1 + value2) * exp(value2);
           // std::cout << LOGFilterMatrix(row+windowRadiusSize,col+windowRadiusSize) << " ";
            sum += LOGFilterMatrix(row+windowRadiusSize,col+windowRadiusSize);
        }
        std::cout << std::endl;
    }
    */
    
//    for(int row = 0; row < (2*windowRadiusSize+1); ++row)
//        for(int col = 0; col < (2*windowRadiusSize+1); ++col)
//            LOGFilterMatrix(row,col) /= sum;
//    
//
    LOGFilterMatrix(0,0) = 0;
    LOGFilterMatrix(0,1) = 1;
    LOGFilterMatrix(0,2) = 0;
    LOGFilterMatrix(1,0) = 1;
    LOGFilterMatrix(1,1) = -4;
    LOGFilterMatrix(1,2) = 1;
    LOGFilterMatrix(2,0) = 0;
    LOGFilterMatrix(2,1) = 1;
    LOGFilterMatrix(2,2) = 0;
    
    return LOGFilterMatrix;
}

Eigen::Matrix3d ImageFiltersUtilityClass::GetSobelXFilter() {
    Eigen::Matrix3d sobelXFilter;
    
    sobelXFilter(0,0) = (double)-1/(double)8; sobelXFilter(0,1) = (double)0/(double)8; sobelXFilter(0,2) = (double)1/(double)8;
    sobelXFilter(1,0) = (double)-2/(double)8; sobelXFilter(1,1) = (double)0/(double)8; sobelXFilter(1,2) = (double)2/(double)8;
    sobelXFilter(2,0) = (double)-1/(double)8; sobelXFilter(2,1) = (double)0/(double)8; sobelXFilter(2,2) = (double)1/(double)8;
    
    return sobelXFilter;
}

Eigen::Matrix3d ImageFiltersUtilityClass::GetSobelYFilter() {
    Eigen::Matrix3d sobelYFilter;
    
    sobelYFilter(0,0) = (double)1/(double)8; sobelYFilter(0,1) = (double)2/(double)8; sobelYFilter(0,2) = (double)1/(double)8;
    sobelYFilter(1,0) = (double)0/(double)8; sobelYFilter(1,1) = (double)0/(double)8; sobelYFilter(1,2) = (double)0/(double)8;
    sobelYFilter(2,0) = (double)-1/(double)8; sobelYFilter(2,1) = (double)-2/(double)8; sobelYFilter(2,2) = (double)-1/(double)8;
    
    return sobelYFilter;
}

void ImageFiltersUtilityClass::ApplyLOGFilter(const Array2D<Rgba>& inputImage_,Array2D<Rgba>& blurImage,const int& windowRadius) {
    
    int imageHeight = (int)inputImage_.height();
    int imageWidth  = (int)inputImage_.width();
    
    Eigen::MatrixXd LOGFilter = GetLaplacianOfGaussianFilterMatrix(windowRadius+1);
    Rgba sum(0,0,0);
    
    for (int row = windowRadius; row < (imageHeight-windowRadius); ++row) {
        for (int col = windowRadius; col < (imageWidth-windowRadius); ++col) {
            sum = Rgba(0,0,0);
            
            for (int LOGrow = -windowRadius; LOGrow <= windowRadius; ++LOGrow) {
                for (int LOGcol = -windowRadius; LOGcol <= windowRadius; ++LOGcol) {
                    double LOGValue = LOGFilter(LOGrow+windowRadius ,LOGcol+windowRadius);
                    
                    sum.r += inputImage_[row+LOGrow][col+LOGcol].r * LOGValue;
                    sum.g += inputImage_[row+LOGrow][col+LOGcol].g * LOGValue;
                    sum.b += inputImage_[row+LOGrow][col+LOGcol].b * LOGValue;
                    
                }
            }
            blurImage[row][col] = sum;
        }
    }
}


void ImageFiltersUtilityClass::ApplyCannyEdgeDetectionFilter(const Array2D<Rgba>& inputImage_, Array2D<Rgba>& blurImage, const int& windowRadius) {
    
}


