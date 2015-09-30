//
//  ImageFilterClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 9/26/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "ImageFilterClass.h"

ImageFilterClass::ImageFilterClass():metaData(new ImageMetaData()) {
}

ImageFilterClass::~ImageFilterClass() {
    if (metaData != nullptr) {
        delete metaData;
        metaData = nullptr;
    }
}

void ImageFilterClass::SetImageMetaData(const ImageMetaData& imageMetaData) {
    *metaData = imageMetaData;
}