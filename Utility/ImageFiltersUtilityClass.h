//
//  ImageFiltersUtilityClass.h
//  Archis
//
//  Created by Pratheba Selvaraju on 9/22/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef __Archis__ImageFiltersUtilityClass__
#define __Archis__ImageFiltersUtilityClass__

#include <iostream>
#include "../ImageFilter/GaussianFilterClass.h"

class ImageFiltersUtilityClass {
    
public:
    ImageFiltersUtilityClass();
    ~ImageFiltersUtilityClass(){}
    
    
private:
    
    Eigen::MatrixXd GetLaplacianOfGaussianFilterMatrix(const int& windowRadiusSize);
    Eigen::Matrix3d GetSobelXFilter();
    Eigen::Matrix3d GetSobelYFilter();
    
    void ApplyLOGFilter(const Array2D<Rgba>& inputImage_,Array2D<Rgba>& blurImage, const int& windowRadius);
    void ApplyCannyEdgeDetectionFilter(const Array2D<Rgba>& inputImage_, Array2D<Rgba>& blurImage, const int& windowRadius);
    
    GaussianFilterClass* gaussianClass;
};

#endif /* defined(__Archis__ImageFiltersUtilityClass__) */
