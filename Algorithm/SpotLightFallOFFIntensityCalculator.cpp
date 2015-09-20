//
//  SpotLightFallOFFIntensityCalculator.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 8/27/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "SpotLightFallOFFIntensityCalculator.h"
#include <numeric>

#define BLOCKSIZE 5
#define WINDOWSIZE 3
#define EPSILON 0.0001

SpotLightFallOFFIntensityCalculator::SpotLightFallOFFIntensityCalculator():utilityClass(new UtilityClass()) {
    
}

SpotLightFallOFFIntensityCalculator::~SpotLightFallOFFIntensityCalculator() {
    
}

void SpotLightFallOFFIntensityCalculator::Initialize(const Array2D<Rgba>& inputImage_, const Point2D<int>& corePoint_) {
    
}

int SpotLightFallOFFIntensityCalculator::GetNumberOfRadiusSegments(const Array2D<Rgba>& inputImage_, const Point2D<int>& corePoint_) {
    
    //Get Max distance from core point to all points in image
    int max1 = std::sqrt(std::pow(corePoint_.x, 2) + std::pow(corePoint_.y, 2));
    int max2 = std::sqrt(std::pow(corePoint_.x - inputImage_.width(), 2) + std::pow(corePoint_.y - inputImage_.height(), 2));
    int max3 = std::sqrt(std::pow(corePoint_.x - inputImage_.width(), 2) + std::pow(corePoint_.y, 2));
    int max4 = std::sqrt(std::pow(corePoint_.x, 2) + std::pow(corePoint_.y - inputImage_.height(), 2));
    
    int max = std::max(std::max(std::max(max1, max2), max3), max4);
    return (int)(max/BLOCKSIZE);
}

void SpotLightFallOFFIntensityCalculator::GetLightFallOffPointsfromCorePoints(const Array2D<Rgba>& inputImage_, const Point2D<int> corePoint_) {
    
    GetLightFallOffPointsfromCorePoints_UsingTableMapOfSectorsAndSegment(inputImage_, corePoint_);
     //GetLightFallOffPointsfromCorePoints_UsingBlocksOfPixels(inputImage_, corePoint_);
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

int Vector_acc(int lhs, const Rgba& rhs)
{
    return lhs + ((rhs.r + rhs.g + rhs.b)/3);
}

int Vector_value(int lhs, const Rgba& rhs)
{
    if ((rhs.r == 0) && (rhs.g == 0) && (rhs.b == 0)) {
        return lhs + 1;
    }
    return lhs;
}


struct value_equal {
    Rgba comp_to;
    
    value_equal(Rgba in_value):comp_to(in_value){}
    
    bool operator()(Rgba& input) {
        if ((input.r == comp_to.r) && (input.g == comp_to.g) && (input.b == comp_to.b)) {
            return true;
        }
        return false;
    }
};

void SpotLightFallOFFIntensityCalculator::InitializeSectorsOfImage(const Array2D<Rgba>& inputImage_, const Point2D<int>&corePoint_) {
    int maxNumberOfRadiusSegment =  GetNumberOfRadiusSegments(inputImage_, corePoint_);
    
    Sector = SectorsInImage(360, SegmentInEachSector(maxNumberOfRadiusSegment,std::vector<Point2D<int>>()));
    //SegmentInEachSector Segment(maxNumberOfRadiusSegment,std::vector<Point2D<int>>());
    
    SectorIntensity = IntensityInImageSectors(360, IntensitySegmentInEachSector(maxNumberOfRadiusSegment, std::vector<Rgba>()));
    //IntensitySegmentInEachSector SegmentIntensity(maxNumberOfRadiusSegment, std::vector<Rgba>());
    
}

void SpotLightFallOFFIntensityCalculator::GetLightFallOffPointsfromCorePoints_UsingTableMapOfSectorsAndSegment(const Array2D<Rgba>& inputImage_, const Point2D<int>&corePoint_) {
    
    // Create a Table with Degree and radius
    //  PixelInSegment[degree][radius] -> vector of pixels.
    
    int maxNumberOfRadiusSegment =  GetNumberOfRadiusSegments(inputImage_, corePoint_);
    InitializeSectorsOfImage(inputImage_, corePoint_);
    
    int numOfRows = (int)inputImage_.height();
    int numOfCols = (int)inputImage_.width();
    
    for (int row=0; row < numOfRows; ++row) {
        for (int col = 0; col < numOfCols; ++col) {
            // Which pixel belongs to which sector and which segment
            // Get distance from corePoint_;
            
            if (row == corePoint_.y && col == corePoint_.x) {
                continue;
            }
 
            Point2D<int> currPixel = Point2D<int>((row - corePoint_.y),(col - corePoint_.x));
            
            double distance = std::sqrt(std::pow(currPixel.x, 2) + std::pow(currPixel.y, 2));
            int theta = (int)rad2deg(atan2(-currPixel.y, currPixel.x));
            int block = (distance - 1) / (BLOCKSIZE );
            
            if (theta < 0) {
                theta = 360 + theta;
            }
            Sector[theta][block].push_back(Point2D<int>(row,col));
            SectorIntensity[theta][block].push_back(inputImage_[row][col]);
        }
    }
    
    UtilityClass* util = new UtilityClass();
    Rgba* outputPixels = util->GetImagePixelsToWrite(numOfCols, numOfRows);
    
    std::vector<double>diffInIntensity = std::vector<double>(360,0);
    
    for (int index = 0; index < 360; ++index) {
        for (int block = 1; block < (maxNumberOfRadiusSegment); ++block) {
            
            int numberOfZeroIntensityVectorBlock1 = 0, numberOfZeroIntensityVectorBlock2 = 0;
  
            if (Sector[index][block].size() > 0) {

                numberOfZeroIntensityVectorBlock1 = (int)std::count_if(SectorIntensity[index][block].begin(), SectorIntensity[index][block].end(), value_equal(Rgba(0, 0, 0)));
                
                std::cout << index << "-" << block << " = " << numberOfZeroIntensityVectorBlock1 << " total = " << SectorIntensity[index][block].size() << "  normalized = " << (double)((double)numberOfZeroIntensityVectorBlock1/ (double)SectorIntensity[index][block].size()) << " -- " << Sector[index][block][0].y << "," << Sector[index][block][0].x <<  std::endl;
            }
            
            else if (Sector[index][block-1].size() > 0) {
                numberOfZeroIntensityVectorBlock2 = (int)std::count_if(SectorIntensity[index][block-1].begin(), SectorIntensity[index][block-1].end(), value_equal(Rgba(0, 0, 0)));
                
                 std::cout << index << "-" << block-1 << " = " << numberOfZeroIntensityVectorBlock2 << " total = " << SectorIntensity[index][block-1].size() << "  normalized = " << (double)((double)numberOfZeroIntensityVectorBlock2/ (double)SectorIntensity[index][block-1].size()) << " -- " << Sector[index][block-1][0].y << "," << Sector[index][block-1][0].x <<  std::endl;
            }
            
            if (std::abs((int)Sector[index][block].size() - (int)Sector[index][block-1].size()) > 0) {
  
                // Sum of all pixel intensities in each block.
                int avgSumOfElementsInFirstBlock   =   (std::accumulate(SectorIntensity[index][block].begin(),SectorIntensity[index][block].end(),0, Vector_acc))/((int)SectorIntensity[index][block].size() + EPSILON);
                int avgSumOfElementsInSecondBlock  =   std::accumulate(SectorIntensity[index][block-1].begin(),SectorIntensity[index][block-1].end(),0, Vector_acc)/((int)SectorIntensity[index][block-1].size() + EPSILON);
                
                int averagePixelIntensityDifference = std::abs( avgSumOfElementsInFirstBlock - avgSumOfElementsInSecondBlock);
                
                if (Sector[index][block].size() > 0) {
                std::cout << index << "::" << block << "::" << Sector[index][block][0].y << ","<< Sector[index][block][0].x << " = " << averagePixelIntensityDifference << std::endl;
                }
                else {
                    std::cout << index << "::" << block-1 << "::" << Sector[index][block-1][0].y << ","<< Sector[index][block-1][0].x << " = " << averagePixelIntensityDifference << std::endl;
                }
                
                if (averagePixelIntensityDifference > diffInIntensity[index]) {
                    diffInIntensity[index] = averagePixelIntensityDifference;
                    std::cout << "Overall difference in intensity = " ;
                    if (Sector[index][block].size() > 0) {
                    
                    std::cout << index << "::" << block << "::" << Sector[index][block][0].y << ","<< Sector[index][block][0].x << " = " << diffInIntensity[index] << std::endl;
                    }
                    else {
                    std::cout << index << "::" << block << "::" << Sector[index][block-1][0].y << ","<< Sector[index][block-1][0].x << " = " << diffInIntensity[index] << std::endl;
                    }
                }
            }
        }
    }
    
    
    for (int index = 0; index <360; ++index) {
        for (int pixelIndex = 0; pixelIndex < Sector[index][5].size(); ++pixelIndex) {
             int i = Sector[index][5][pixelIndex].y * numOfCols + Sector[index][5][pixelIndex].x;
             outputPixels[i].r = 1.0;
            std::cout << Sector[index][5][pixelIndex].y << "," << Sector[index][5][pixelIndex].x << "   ";
        }
        std::cout << std::endl;
    }
    util->WriteImage2DArrayPixels("output-angle90.exr", outputPixels, numOfCols, numOfRows);
    
    delete[] outputPixels;
    delete util;
    
}

void SpotLightFallOFFIntensityCalculator::GetGradientDifferenceForEachSectorSegments(const int& maxNumberOfRadiusSegment) {
    
    std::vector<double>diffInIntensity = std::vector<double>(360,0);
    
    for (int index = 0; index < 360; ++index) {
        for (int block = 1; block < (maxNumberOfRadiusSegment); ++block) {
            
            //int numberOfZeroIntensityVectorBlock1 = 0, numberOfZeroIntensityVectorBlock2 = 0;
            
            int count1 = 0;
            for (int i = 0; i < SectorIntensity[index][block].size(); ++i) {
                if ((SectorIntensity[index][block][i].r == 0) && (SectorIntensity[index][block][i].g == 0) && (SectorIntensity[index][block][i].b == 0)) {
                    count1++;
                }
            }
            
            int count2 = 0;
            for (int i = 0; i < SectorIntensity[index][block-1].size(); ++i) {
                if ((SectorIntensity[index][block-1][i].r == 0) && (SectorIntensity[index][block-1][i].g == 0) && (SectorIntensity[index][block-1][i].b == 0)) {
                    count2++;
                }
            }
            
            if (Sector[index][block].size() > 0) {
                std::cout << index << "-" << block << " = " << count1 << " total = " << SectorIntensity[index][block].size() << "  normalized = " << (double)((double)count1/ (double)SectorIntensity[index][block].size()) << " -- " << Sector[index][block][0].y << "," << Sector[index][block][0].x <<  std::endl;
            }
            
            if (Sector[index][block-1].size() > 0) {
                
                std::cout << index << "-" << block-1 << " = " << count2 << "  .total = " << SectorIntensity[index][block-1].size() << " normalized = " << (double)((double)count2/ (double)SectorIntensity[index][block-1].size()) << " -- " << Sector[index][block-1][0].y << "," << Sector[index][block-1][0].x << std::endl;
            }
            
            if (std::abs((int)Sector[index][block].size() - (int)Sector[index][block-1].size()) > 0) {
                
                // Sum of all pixel intensities in each block.
                int avgSumOfElementsInFirstBlock   =   (std::accumulate(SectorIntensity[index][block].begin(),SectorIntensity[index][block].end(),0, Vector_acc))/((int)SectorIntensity[index][block].size() + EPSILON);
                int avgSumOfElementsInSecondBlock  =   std::accumulate(SectorIntensity[index][block-1].begin(),SectorIntensity[index][block-1].end(),0, Vector_acc)/((int)SectorIntensity[index][block-1].size() + EPSILON);
                
                int averagePixelIntensityDifference = std::abs( avgSumOfElementsInFirstBlock - avgSumOfElementsInSecondBlock);
                
                if (Sector[index][block].size() > 0) {
                    std::cout << index << "::" << block << "::" << Sector[index][block][0].y << ","<< Sector[index][block][0].x << " = " << averagePixelIntensityDifference << std::endl;
                }
                else {
                    std::cout << index << "::" << block-1 << "::" << Sector[index][block-1][0].y << ","<< Sector[index][block-1][0].x << " = " << averagePixelIntensityDifference << std::endl;
                }
                
                if (averagePixelIntensityDifference > diffInIntensity[index]) {
                    diffInIntensity[index] = averagePixelIntensityDifference;
                    std::cout << "Overall difference in intensity = " ;
                    if (Sector[index][block].size() > 0) {
                        
                        std::cout << index << "::" << block << "::" << Sector[index][block][0].y << ","<< Sector[index][block][0].x << " = " << diffInIntensity[index] << std::endl;
                    }
                    else {
                        std::cout << index << "::" << block << "::" << Sector[index][block-1][0].y << ","<< Sector[index][block-1][0].x << " = " << diffInIntensity[index] << std::endl;
                    }
                }
            }
        }
    }
}

void SpotLightFallOFFIntensityCalculator::GetLightFallOffPointsfromCorePoints_UsingAngleEstimationFromLightPosition(const Array2D<Rgba>& inputImage_, const Point2D<int>& corePoint_) {
    
}

void SpotLightFallOFFIntensityCalculator::GetLightFallOffPointsfromCorePoints_UsingGradientEstimation(const Array2D<Rgba>& inputImage_, const Point2D<int>& corePoint_) {
    
}
