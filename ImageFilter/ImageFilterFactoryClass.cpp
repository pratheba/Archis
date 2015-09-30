//
//  ImageFilterFactoryClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 9/26/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "ImageFilterFactoryClass.h"

ImageFilterFactoryClass::ImageFilterFactoryClass():imageFilterClass(nullptr) {
}

ImageFilterFactoryClass::~ImageFilterFactoryClass() {
    if (imageFilterClass != nullptr) {
        delete imageFilterClass;
        imageFilterClass = nullptr;
    }
}

ImageFilterClass* ImageFilterFactoryClass::GetImageFilterClass(const Array2D<Rgba>& inputImage_, const int& windowRadius,FILTERTYPE filterType, const double& sigma) {
    
    ImageFilterClass::ImageMetaData imageMetaData(static_cast<int>(inputImage_.width()), static_cast<int>(inputImage_.height()), windowRadius, sigma);
    
    switch (filterType) {
        case GAUSSIAN:
            imageFilterClass = new GaussianFilterClass();
            imageFilterClass->SetImageMetaData(imageMetaData);
            break;
        case SOBEL:
            imageFilterClass = new SobelFilterClass();
            imageFilterClass->SetImageMetaData(imageMetaData);
            
        default:
            break;
    }
    
    return imageFilterClass;
}