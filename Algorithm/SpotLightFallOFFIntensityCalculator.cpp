//
//  SpotLightFallOFFIntensityCalculator.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 8/27/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "SpotLightFallOFFIntensityCalculator.h"
#define BLOCKSIZE 5
#define WINDOWSIZE 3

SpotLightFallOFFIntensityCalculator::SpotLightFallOFFIntensityCalculator():utilityClass(new UtilityClass()) {
    
}

SpotLightFallOFFIntensityCalculator::~SpotLightFallOFFIntensityCalculator() {
    
}

void SpotLightFallOFFIntensityCalculator::Initialize(const Array2D<Rgba>& inputImage_, const Point2D<int>& corePoint_) {
    
}

void SpotLightFallOFFIntensityCalculator::GetLightFallOffPointsfromCorePoints(const Array2D<Rgba> inputImage_, const Point2D<int> corePoint_) {
    
     GetLightFallOffPointsfromCorePoints_UsingBlocksOfPixels(inputImage_, corePoint_);
}

void SpotLightFallOFFIntensityCalculator::GetLightFallOffPointsfromCorePoints_UsingBlocksOfPixels(const Array2D<Rgba>& inputImage_, const Point2D<int>& corePoint_) {
    
    int imageWidth = (int)inputImage_.width();
    int imageHeight = (int)inputImage_.height();
    //Initialize(inputImage_, corePoint_);
    
    Point2D<int> currPoint = corePoint_;
    Point2D<int> corePoint = corePoint_;
    int block = 1;
    double currBlockIntensity = 0;
    double prevBlockIntensity = 0;
    double currPixelWindowIntensity = 0;
    
    
    // Loop through 0 to 360
    for (int degree = 0; degree < 360; ++degree) {
        currPoint   =   corePoint_;
        block       =   1;
        currBlockIntensity = 0;
        
        std::cout << "*********** degree = " << degree << std::endl;
        while ((currPoint.x >= 0) && (currPoint.x < imageWidth) && (currPoint.y >= 0) && (currPoint.y < imageHeight)) {
            prevBlockIntensity = currBlockIntensity;
            currBlockIntensity = 0;
            
            for (int radius = block; radius <=(block+BLOCKSIZE); ++radius) {
                
                currPoint = const_cast<Point2D<int>&>(corePoint_) + Point2D<int>(radius * cos(degree), radius * sin(degree));
                //currPoint = corePoint + Point2D<double>(radius*cos(degree), radius*sin(degree));
                
                if ((currPoint.x < 0) || (currPoint.x > imageWidth) || (currPoint.y < 0) || (currPoint.y > imageHeight)) {
                    break;
                }
                currPixelWindowIntensity = 0;
                utilityClass->GetAveragePixelIntensityAroundaPoint(inputImage_, currPoint, WINDOWSIZE);
                currBlockIntensity += currPixelWindowIntensity;
                //currBlockIntensity += inputImage->at<uchar>(currPoint.y,currPoint.x);
            }
            currBlockIntensity /= BLOCKSIZE;
            
            double intensityDifference = prevBlockIntensity - currBlockIntensity;
            std::cout << "intensity difference  = " << intensityDifference << std::endl;
            //if ((intensityDifference > 2 && intensityDifference < 5) && (block != 1)) {
            
            /*
            if ((intensityDifference >= 5 && intensityDifference <= 8) && (block != 1)) {
                if ((currPoint.x >= 0) && (currPoint.x < imageWidth) && (currPoint.y >= 0) && (currPoint.y < imageHeight)) {
                    cv::circle(refImage, currPoint, 2, cv::Scalar(0,0,255), -1);
                }
            }
            
            if ((intensityDifference >= 10 && intensityDifference <= 15) && (block != 1)) {
                if ((currPoint.x >= 0) && (currPoint.x < imageWidth) && (currPoint.y >= 0) && (currPoint.y < imageHeight)) {
                    cv::circle(refImage, currPoint, 2, cv::Scalar(0,255,255), -1);
                }
            }
             */
            
            block += BLOCKSIZE;
        }
    }
    return;
}

void SpotLightFallOFFIntensityCalculator::GetLightFallOffPointsfromCorePoints_UsingConeSector(const Array2D<Rgba>& inputImage_, const Point2D<int>& corePoint_) {
    
    }

void SpotLightFallOFFIntensityCalculator::GetLightFallOffPointsfromCorePoints_UsingTableMapOfSectorsAndSegment(const Array2D<Rgba>& inputImage_, const Point2D<int>&corePoint_) {
    
    // Create a Table with Degree and radius
//    std::vector<std::vector<Point2D<int>>> PixelInSector(360);
//    int numOfRows = (int)inputImage_.height();
//    int numOfCols = (int)inputImage_.width();
//    
//    for (int row=0; row < numOfRows; ++row) {
//        for (int col = 0; col < numOfCols; ++col) {
//            // Which pixel belongs to which sector and which segment
//            // Get distance from corePoint_;
//            
//            Point2D<double> currPixel = Point2D<double>((row - corePoint_.x),(col - corePoint_.y));
//            
//            double distance = std::sqrt(std::pow(currPixel.x, 2) + std::pow(currPixel.y, 2));
//            double theta = rad2deg(atan2(-currPixel.x, currPixel.y));
//            
//            PixelInSector[theta].push_back(PixelInSector[theta].end(),ty)
//        }
//    }

}

void SpotLightFallOFFIntensityCalculator::GetLightFallOffPointsfromCorePoints_UsingGradientEstimation(const Array2D<Rgba>& inputImage_, const Point2D<int>& corePoint_) {
    
}
