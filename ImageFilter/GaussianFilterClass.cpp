//
//  GaussianFilterClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 9/26/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "GaussianFilterClass.h"

GaussianFilterClass::GaussianFilterClass() {
}

GaussianFilterClass::~GaussianFilterClass() {
}

void GaussianFilterClass::ProcessImage(const Array2D<Rgba>& inputImage_,Array2D<Rgba>& outputImage) {
    ApplyGaussianFilter(inputImage_,outputImage);
}

void GaussianFilterClass::Initialize(const Array2D<Rgba>& inputImage_) {
    
    if (metaData == nullptr) {
        metaData = new ImageMetaData();
        
        metaData->Width = static_cast<int>(inputImage_.width());
        metaData->Height = static_cast<int>(inputImage_.height());
        metaData->radius = 2;
        metaData->sigma = 1.4;
    }
}

void GaussianFilterClass::ApplyGaussianFilter(const Array2D<Rgba>& inputImage_, Array2D<Rgba>& outputImage) {
    Initialize(inputImage_);
    SeparableGaussianSmoothImage(inputImage_,outputImage);
}

Eigen::MatrixXd GaussianFilterClass::GetGaussianFilterMatrix(const int& windowRadiusSize) {
    
    Eigen::MatrixXd gaussianFilterMatrix = Eigen::MatrixXd(2*windowRadiusSize+1, 2*windowRadiusSize+1);
    
    double sigma = 1.4;
    int numerator = 0;
    int denominator = 2.0 * sigma * sigma;
    double sum = 0.0;
    
    for (int row = -windowRadiusSize; row <= windowRadiusSize; ++row) {
        for (int col = -windowRadiusSize; col <= windowRadiusSize; ++col) {
            numerator = row*row + col*col;
            gaussianFilterMatrix(row+windowRadiusSize,col+windowRadiusSize) = (std::exp(-(numerator)/denominator))/(M_PI * denominator);
            sum += gaussianFilterMatrix(row+windowRadiusSize, col+windowRadiusSize);
        }
    }
    
    for(int row = 0; row < (2*windowRadiusSize+1); ++row)
        for(int col = 0; col < (2*windowRadiusSize+1); ++col)
            gaussianFilterMatrix(row,col) /= sum;
    
    return gaussianFilterMatrix;
}

void GaussianFilterClass::InitializeSeperableGaussianKernel(GaussianKernel& gaussianKernel) {
    gaussianKernel.KernelSize   =       2 * metaData->radius + 1;
    gaussianKernel.Kernel       =       new double[gaussianKernel.KernelSize];
}

void GaussianFilterClass::LoadvaluesOfSeperablekernelComponents(GaussianKernel& gaussianKernel)
{
    double kernelConstant = (double)1 / (double)(sqrtf(2 * M_PI) * (float)gaussianKernel.variance);
    
    for(int pos = -metaData->radius; pos <= metaData->radius; ++pos) {
        int value                               =   (int)powf(float(pos), (float)2);
        double expValue                         =   (double)(-1)*((value)/(2*(powf((float)gaussianKernel.variance,(float)2))));
        gaussianKernel.Kernel[(pos + metaData->radius)]   =   kernelConstant * exp(expValue);
    }
}

void GaussianFilterClass::NormalizeSeperableKernel(GaussianKernel& kernel) {
    double epsilon = 0.000001;
    double magnitude = epsilon;
    
    for(int pos = 0; pos < kernel.KernelSize ; pos++)
        magnitude +=   kernel.Kernel[pos];
    
    for(int pos = 0; pos < kernel.KernelSize ; pos++)
        kernel.Kernel[pos] /= magnitude;
}

void GaussianFilterClass::GetSeperableGaussianKernel(GaussianKernel& gaussianKernel) {
    InitializeSeperableGaussianKernel(gaussianKernel);
    LoadvaluesOfSeperablekernelComponents(gaussianKernel);
    NormalizeSeperableKernel(gaussianKernel);
}

void GaussianFilterClass::ApplyVerticalFilterToPixel(const Array2D<Rgba>& inputImage_, Array2D<Rgba>& outputImage,const GaussianKernel& kernel) {
    
    int edgePixel;
    int radius = metaData->radius;
    Rgba pixel(0,0,0);
    Rgba sum(0,0,0);
    
    for(int rowPixel = 0; rowPixel < metaData->Height; rowPixel++) { // Y
        for(int colPixel = 0; colPixel < metaData->Width; colPixel++) { // X
            sum = Rgba(0, 0, 0);
            for(int row = -radius; row <= radius; ++row) {
                
                if((rowPixel+row) < 0 || (rowPixel+row)> (metaData->Height-1)) {
                    edgePixel = GetPixelPositionForEdges(rowPixel+row,metaData->Height-1);
                    pixel = inputImage_[edgePixel][colPixel];
                }
                else
                    pixel = inputImage_[rowPixel+row][colPixel];
                
                double weight = kernel.Kernel[row+radius];
                sum.r += pixel.r * weight;
                sum.g += pixel.g * weight;
                sum.b += pixel.b * weight;
            }
            outputImage[rowPixel][colPixel] = sum;
        }
    }
}

void GaussianFilterClass::ApplyHorizontalFilterToPixel(const Array2D<Rgba>& inputImage_, Array2D<Rgba>& outputImage,const GaussianKernel& kernel) {
    int edgePixel;
    int radius = metaData->radius;
    Rgba pixel(0,0,0);
    Rgba sum(0,0,0);
    
    for(int rowPixel = 0; rowPixel < metaData->Height; rowPixel++) { // Y
        for(int colPixel = 0; colPixel < metaData->Width; colPixel++) { // X
            sum = Rgba(0, 0, 0);
            for(int col = -radius; col <= radius; ++col) {
                
                if((colPixel+col) < 0 || (colPixel+col)> (metaData->Width-1)) {
                    edgePixel = GetPixelPositionForEdges(colPixel+col,metaData->Width-1);
                    pixel = inputImage_[rowPixel][edgePixel];
                }
                else
                    pixel = inputImage_[rowPixel][colPixel+col];
                
                double weight = kernel.Kernel[col+radius];
                sum.r += pixel.r * weight;
                sum.g += pixel.g * weight;
                sum.b += pixel.b * weight;
                
            }
            outputImage[rowPixel][colPixel] = sum;
        }
    }
}

void GaussianFilterClass::ConvolveImagewithSeperableGaussianKernel(const Array2D<Rgba>& inputImage_, Array2D<Rgba>& outputImage, GaussianKernel& kernel) {
    
    Array2D<Rgba> tempImage(metaData->Height, metaData->Width);
    for (int row = 0; row < (metaData->Height); ++row) {
        for (int col = 0; col < (metaData->Width); ++col) {
            tempImage[row][col] = Rgba(0, 0, 0);
        }
    }
    
    ApplyHorizontalFilterToPixel(inputImage_, tempImage,kernel);
    ApplyVerticalFilterToPixel(tempImage,outputImage, kernel);
}

// 2 1-D Convoluted Gaussian Filter
void GaussianFilterClass::SeparableGaussianSmoothImage(const Array2D<Rgba>& inputImage_, Array2D<Rgba>& outputImage)
{
    GaussianKernel gaussianKernel(0,metaData->sigma,NULL);
    GetSeperableGaussianKernel(gaussianKernel);
    ConvolveImagewithSeperableGaussianKernel(inputImage_,outputImage, gaussianKernel);
}

void GaussianFilterClass::InitializeGaussianKernel(GaussianKernel& gaussianKernel) {
    gaussianKernel.KernelSize   =       2 * metaData->radius + 1;
    gaussianKernel.Kernel       =       new double[gaussianKernel.KernelSize * gaussianKernel.KernelSize];
}

void GaussianFilterClass::LoadvaluesOfkernelComponents(GaussianKernel& gaussianKernel)
{
    double kernelConstant = (double)1 / (double)(2 * M_PI * powf((float)gaussianKernel.variance,(float)2));
    
    for(int row = -metaData->radius; row <= metaData->radius; ++row) {
        for(int col = -metaData->radius; col <= metaData->radius; ++col) {
            int Xvalue      =   (int)powf(float(row ), (float)2);
            int Yvalue      =   (int)powf(float(col ), (float)2);
            double expValue    =   (double)(-1)*((Xvalue + Yvalue)/(2*(powf((float)gaussianKernel.variance,(float)2))));
            
            gaussianKernel.Kernel[(row + metaData->radius)*gaussianKernel.KernelSize + col + metaData->radius] = kernelConstant * exp(expValue);
        }
    }
}

void GaussianFilterClass::NormalizeKernel(GaussianKernel& kernel) {
    double epsilon = 0.000001;
    double magnitude = epsilon;
    for(int pos = 0; pos < kernel.KernelSize * kernel.KernelSize ; pos++) {
        magnitude +=   kernel.Kernel[pos];
    }
    
    for(int pos = 0; pos < kernel.KernelSize * kernel.KernelSize ; pos++)
        kernel.Kernel[pos] /= magnitude;
}

void GaussianFilterClass::GetGaussianKernel(GaussianKernel& gaussianKernel) {
    InitializeGaussianKernel( gaussianKernel);
    LoadvaluesOfkernelComponents(gaussianKernel);
    NormalizeKernel(gaussianKernel);
}

// 2-D Gaussian Filter
void GaussianFilterClass::ConvolveImagewithGaussianKernel(const Array2D<Rgba>& inputImage_, Array2D<Rgba>& outputImage,const GaussianKernel& kernel) {
    
    int radius = metaData->radius;
    Rgba pixel(0,0,0);
    Rgba sum(0,0,0);
    
    for(int rowPixel = 0; rowPixel < metaData->Height; rowPixel++) {
        for(int colPixel = 0; colPixel < metaData->Width; colPixel++) {
            
            sum = Rgba(0,0,0);
            for(int row = -radius; row <= radius; ++row) {
                for(int col = -radius; col <= radius; ++col) {
                    
                    pixel           =   inputImage_[rowPixel + row + radius][colPixel + col + radius];
                    double  weight  =   kernel.Kernel[(row + radius)* kernel.KernelSize + (col + radius)];
                    
                    sum.r           += pixel.r * weight;
                    sum.g           += pixel.g * weight;
                    sum.b           += pixel.b * weight;
                }
            }
            outputImage[rowPixel][colPixel] = sum;
        }
    }
}

void GaussianFilterClass::GaussianBlurImage(const Array2D<Rgba>& inputImage_, Array2D<Rgba>& outputImage)
{
    GaussianKernel gaussianKernel(0,metaData->sigma,NULL);
    GetGaussianKernel(gaussianKernel);
    ConvolveImagewithGaussianKernel(inputImage_ ,outputImage,gaussianKernel);
}

