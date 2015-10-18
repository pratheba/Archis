//
//  ImageSystemClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 8/26/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "ImageSystemClass.h"
#include "HistogramOfImageClass.h"
#include "DBSCAN_Class.h"

#define CLUSTERMINPOINT 20
#define CLUSTEREPSILON 4

ImageSystemClass* ImageSystemClass::_imageSystem = nullptr;
Image2DClass* ImageSystemClass::_currentImage = nullptr;

ImageSystemClass& ImageSystemClass::GetInstance() {
    if (_imageSystem == nullptr) {
        _imageSystem = new ImageSystemClass();
    }
    return *_imageSystem;
};

ImageSystemClass& ImageSystemClass::GetInstance(const std::string& inputImageFileName) {
    if (_imageSystem == nullptr) {
        _imageSystem = new ImageSystemClass(inputImageFileName);
    }
    return *_imageSystem;
};

ImageSystemClass::ImageSystemClass() {
    if (_currentImage != nullptr) {
        delete _currentImage;
    }
    _currentImage = new Image2DClass();
}

ImageSystemClass::ImageSystemClass(const std::string& inputImageFileName) {
    if (_currentImage != nullptr) {
        delete _currentImage;
    }
    _currentImage = new Image2DClass(inputImageFileName);
}

void ImageSystemClass::ReleaseInstance() {
    if (_imageSystem != nullptr) {
        if (_currentImage != nullptr) {
            delete _currentImage;
            _currentImage = nullptr;
        }
        delete _imageSystem;
        _imageSystem = nullptr;
    }
}

ImageSystemClass::~ImageSystemClass() {
}

const Image2DClass& ImageSystemClass::GetCurrentImage() const{
    return *_currentImage;
}


const long  ImageSystemClass::GetCurrentImageWidth() const{
    if (_currentImage != nullptr) {
        return (_currentImage->GetImage2DArrayPixels()).width();
    }
    return 0;
    
}
const long  ImageSystemClass::GetCurrentImageHeight() const {
    if (_currentImage != nullptr) {
        return (_currentImage->GetImage2DArrayPixels()).height();
    }
    return 0;
}

VecOf2dDoublePoints ImageSystemClass::GetCentroidsOfHighIntensityPixelsinCurrentImage() {
    
    HistogramOfImageClass* _histogramImageClass = new HistogramOfImageClass();
    _histogramImageClass->SetNumOfBins(256);
    
    VecOf2dIntPoints pixelsOfMaxIntensity = _histogramImageClass->GetAllPixelswithinMaxIntensityRange(_currentImage->GetImage2DArrayPixels());
    
    DBSCAN_Class* dbScanClass = new DBSCAN_Class();
    dbScanClass->SetMinPoints(CLUSTERMINPOINT);
    dbScanClass->SetEpsilon(CLUSTEREPSILON);
    std::vector<VecOf2dIntPoints> clusterPoints = dbScanClass->GetClustersOfDataPoints(pixelsOfMaxIntensity);
    VecOf2dDoublePoints centroidPoints =  dbScanClass->GetCentroidOfClusteredPoints(clusterPoints);
    
    delete _histogramImageClass;
    delete dbScanClass;
    
    return centroidPoints;
}