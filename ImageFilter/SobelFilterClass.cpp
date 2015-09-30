//
//  SobelFilterClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 9/27/15.
//  Copyright © 2015 Pratheba Selvaraju. All rights reserved.
//

#include "SobelFilterClass.hpp"
#define MAX_BRIGHTNESS 65536


SobelFilterClass::SobelFilterClass() {
}

SobelFilterClass::~SobelFilterClass() {
}


void SobelFilterClass::InitializeSobelKernel(SobelKernel& sobelKernel) {
    sobelKernel.KernelSize    =       2*metaData->radius + 1;
    sobelKernel.XSobelKernel  =       new double*[sobelKernel.KernelSize];
    sobelKernel.YSobelKernel  =       new double*[sobelKernel.KernelSize];
    
    for (int index = 0; index < sobelKernel.KernelSize; ++index) {
        sobelKernel.XSobelKernel[index] = new double[sobelKernel.KernelSize];
        sobelKernel.YSobelKernel[index] = new double[sobelKernel.KernelSize];
    }
}

void SobelFilterClass::LoadvaluesOfkernelComponents(SobelKernel& sobelKernel)
{
    sobelKernel.XSobelKernel[0][0] = 1;
    sobelKernel.XSobelKernel[0][1] = 0;
    sobelKernel.XSobelKernel[0][2] = -1;
    
    sobelKernel.XSobelKernel[1][0] = 2;
    sobelKernel.XSobelKernel[1][1] = 0;
    sobelKernel.XSobelKernel[1][2] = -2;
    
    sobelKernel.XSobelKernel[2][0] = 1;
    sobelKernel.XSobelKernel[2][1] = 0;
    sobelKernel.XSobelKernel[2][2] = -1;
    
   
    sobelKernel.YSobelKernel[0][0] = -1;
    sobelKernel.YSobelKernel[0][1] = -2;
    sobelKernel.YSobelKernel[0][2] = -1;
    
    sobelKernel.YSobelKernel[1][0] = 0;
    sobelKernel.YSobelKernel[1][1] = 0;
    sobelKernel.YSobelKernel[1][2] = 0;
    
    sobelKernel.YSobelKernel[2][0] = 1;
    sobelKernel.YSobelKernel[2][1] = 2;
    sobelKernel.YSobelKernel[2][2] = 1;
}

void SobelFilterClass::GetSobelKernel(SobelKernel& sobelKernel) {
    
    InitializeSobelKernel(sobelKernel);
    LoadvaluesOfkernelComponents(sobelKernel);
}

inline int setPixelOrientation(double orientationInRadian ) {
    double orient = (double)((orientationInRadian / M_PI * 180.0));
    
    return ((180 + (int)orient) %(int)(180));
}

void SobelFilterClass::CalculateMagAndOrientationOfSoblePixels(const Array2D<Rgba>& inputImage_,Array2D<Rgba>& outputImage,SobelKernel& kernel,SobelImagePixel** sobelImagePixel) {
    
    double Gx, Gy, mag, orient;
    int radius                              =   metaData->radius;
    int rowEdgePixel, colEdgePixel;
    Rgba pixel(0,0,0);
    double minValue = DBL_MAX;
    double maxValue = DBL_MIN;
    double threshold = 1000;
    
    for(int rowPixel = 0; rowPixel < metaData->Height; rowPixel++) { // Y
        for(int colPixel = 0; colPixel < metaData->Width; colPixel++) { // X
            
            Gx = 0.0; Gy = 0.0;
            
            for(int row = -radius; row <= radius; row++) {
                rowEdgePixel = rowPixel + row;
                if((rowEdgePixel) < 0 || (rowEdgePixel) > (metaData->Height-radius))
                    rowEdgePixel = GetPixelPositionForEdges(rowEdgePixel,metaData->Height-radius);
                
                for(int col = -radius; col <= radius; col++) {
                    colEdgePixel = colPixel + col;
                    if((colEdgePixel) < 0 || (colEdgePixel) > (metaData->Width-radius))
                        colEdgePixel = GetPixelPositionForEdges(colEdgePixel,metaData->Width-radius);
                    
                    pixel = inputImage_[rowEdgePixel][colEdgePixel];
                    double weight = kernel.XSobelKernel[row+radius][col+radius];
                    Gx += ((double)((pixel.r + pixel.g + pixel.b)/3) * weight);
                    
                    weight = kernel.YSobelKernel[row+radius][col+radius];
                    Gy += ((double)((pixel.r + pixel.g + pixel.b)/3) * weight);
                }
            }
            
            mag = sqrt(Gx * Gx + Gy * Gy);
            orient = setPixelOrientation(atan2f(Gy, Gx));
            
            //outputImage[rowPixel][colPixel] = Rgba(mag,mag,mag);
            
            minValue = std::min(minValue, mag);
            maxValue = std::max(maxValue, mag);
            sobelImagePixel[rowPixel][colPixel] = SobelImagePixel(Gx, Gy, mag, orient);
        }
    }
    
    for(int rowPixel = 0; rowPixel < metaData->Height; rowPixel++) { // Y
        for(int colPixel = 0; colPixel < metaData->Width; colPixel++) { // X
            double value = MAX_BRIGHTNESS * (sobelImagePixel[rowPixel][colPixel].magnitude - minValue) / (maxValue - minValue);
            std::cout << rowPixel << "," << colPixel << " = " << value << std::endl;
            if (value < threshold) {
                value = 0;
            }
            
            outputImage[rowPixel][colPixel] = Rgba(value,value,value);
        }
    }
}

void SobelFilterClass::ApplySobelFilterToImage(const Array2D<Rgba>& inputImage_, Array2D<Rgba>& outputImage, SobelKernel& kernel) {
    
    
    SobelImagePixel** sobelImagePixel = new SobelImagePixel*[metaData->Height];
    for (int row = 0; row < metaData->Height; ++row)
        sobelImagePixel[row] = new SobelImagePixel[metaData->Width];
    
    CalculateMagAndOrientationOfSoblePixels(inputImage_,outputImage,kernel,sobelImagePixel);
    
}

void SobelFilterClass::ProcessImage(const Array2D<Rgba>& inputImage_,Array2D<Rgba>& outputImage) {
    SobelKernel sobelKernel(0,NULL,NULL);
    
    GetSobelKernel(sobelKernel);
    ApplySobelFilterToImage(inputImage_, outputImage, sobelKernel);
}


