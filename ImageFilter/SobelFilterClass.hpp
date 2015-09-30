//
//  SobelFilterClass.hpp
//  Archis
//
//  Created by Pratheba Selvaraju on 9/27/15.
//  Copyright © 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef SobelFilterClass_hpp
#define SobelFilterClass_hpp

#include <iostream>
#include "ImageFilterClass.h"

class SobelFilterClass : public ImageFilterClass {
public:
    SobelFilterClass();
    ~SobelFilterClass();
    void ProcessImage(const Array2D<Rgba>& inputImage_,Array2D<Rgba>& outputImage);
    
private:
    
    struct SobelKernel {
        double** XSobelKernel;
        double** YSobelKernel;
        int KernelSize;
        
        SobelKernel():KernelSize(0), XSobelKernel(nullptr), YSobelKernel(nullptr){}
        SobelKernel(int kernelSize_,  double** XKernel_, double** YKernel_):
        KernelSize(kernelSize_), XSobelKernel(XKernel_), YSobelKernel(YKernel_) {}
        SobelKernel(const SobelKernel& SobelKernel_):
        KernelSize(SobelKernel_.KernelSize), XSobelKernel(SobelKernel_.XSobelKernel),YSobelKernel(SobelKernel_.YSobelKernel){}
        
        ~SobelKernel() {
            if(XSobelKernel != nullptr) {
                for(int index = 0; index < KernelSize; ++index)
                    delete[] XSobelKernel[index];
                delete XSobelKernel;
            }
            if(YSobelKernel != nullptr) {
                for(int index = 0; index < KernelSize; ++index)
                    delete[] YSobelKernel[index];
                delete YSobelKernel;
            }
        }
    };
    
    struct SobelImagePixel {
        double Gx;
        double Gy;
        double magnitude;
        double orientation;
       
        SobelImagePixel():Gx(0.0), Gy(0.0), magnitude(0.0), orientation(0){}
        SobelImagePixel(double Gx_, double Gy_, double magnitude_, double orientation_):Gx(Gx_), Gy(Gy_),magnitude(magnitude_), orientation(orientation_){}
    };
    
    void InitializeSobelKernel(SobelKernel& sobelKernel);
    void LoadvaluesOfkernelComponents(SobelKernel& sobelKernel);
    void GetSobelKernel(SobelKernel& sobelKernel);
    
    void CalculateMagAndOrientationOfSoblePixels(const Array2D<Rgba>& inputImage_,Array2D<Rgba>& outputImage,SobelKernel& kernel,SobelImagePixel** sobelImagePixel);
    void ApplySobelFilterToImage(const Array2D<Rgba>& inputImage_, Array2D<Rgba>& outputImage, SobelKernel& kernel);
    
};


#endif /* SobelFilterClass_hpp */
