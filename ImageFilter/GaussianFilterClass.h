//
//  GaussianFilterClass.h
//  Archis
//
//  Created by Pratheba Selvaraju on 9/26/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef __Archis__GaussianFilterClass__
#define __Archis__GaussianFilterClass__

#include <iostream>
#include "ImageFilterClass.h"


class GaussianFilterClass : public ImageFilterClass {
public:
    GaussianFilterClass();
    ~GaussianFilterClass();
    
    void ProcessImage(const Array2D<Rgba>& inputImage_,Array2D<Rgba>& outputImage);
    Eigen::MatrixXd GetGaussianFilterMatrix(const int& windowRadiusSize);
   
    
private:
    
    void Initialize(const Array2D<Rgba>& inputImage_);
    void ApplyGaussianFilter(const Array2D<Rgba>& inputImage_, Array2D<Rgba>& outputImage);

    struct GaussianKernel {
        int KernelSize;
        double variance;
        double* Kernel;
        
        GaussianKernel():KernelSize(0), variance(0), Kernel(nullptr){}
        GaussianKernel(int kernelSize_, double variance_, double* Kernel):
        KernelSize(kernelSize_), variance(variance_), Kernel(Kernel){}
        GaussianKernel(const GaussianKernel& gaussianKernel_):
        KernelSize(gaussianKernel_.KernelSize), variance(gaussianKernel_.variance), Kernel(gaussianKernel_.Kernel){}
    };
    
    void InitializeGaussianKernel(GaussianKernel& gaussianKernel);
    void LoadvaluesOfkernelComponents(GaussianKernel& gaussianKernel);
    void NormalizeKernel(GaussianKernel& kernel);
    void GetGaussianKernel(GaussianKernel& gaussianKernel);
    void ConvolveImagewithGaussianKernel(const Array2D<Rgba>& inputImage_, Array2D<Rgba>& outputImage,const GaussianKernel& kernel);
    void GaussianBlurImage(const Array2D<Rgba>& inputImage_, Array2D<Rgba>& outputImage);
    
    void InitializeSeperableGaussianKernel(GaussianKernel& gaussianKernel);
    void LoadvaluesOfSeperablekernelComponents(GaussianKernel& gaussianKernel);
    void NormalizeSeperableKernel(GaussianKernel& kernel);
    void GetSeperableGaussianKernel(GaussianKernel& gaussianKernel);
    void ApplyVerticalFilterToPixel(const Array2D<Rgba>& inputImage_, Array2D<Rgba>& outputImage,const GaussianKernel& kernel);
    void ApplyHorizontalFilterToPixel(const Array2D<Rgba>& inputImage_, Array2D<Rgba>& outputImage,const GaussianKernel& kernel);
    void ConvolveImagewithSeperableGaussianKernel(const Array2D<Rgba>& inputImage_, Array2D<Rgba>& outputImage, GaussianKernel& kernel);
    void SeparableGaussianSmoothImage(const Array2D<Rgba>& inputImage_, Array2D<Rgba>& outputImage);
    
};

#endif /* defined(__Archis__GaussianFilterClass__) */
