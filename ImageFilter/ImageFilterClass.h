//
//  ImageFilterClass.h
//  Archis
//
//  Created by Pratheba Selvaraju on 9/26/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef __Archis__ImageFilterClass__
#define __Archis__ImageFilterClass__

#include <iostream>
#include "UtilityClass.h"

class ImageFilterClass {
public:
    ImageFilterClass();
    virtual ~ImageFilterClass();
    
    virtual void ProcessImage(const Array2D<Rgba>& inputImage_,Array2D<Rgba>& outputImage) = 0;

    struct ImageMetaData {
        int Width;
        int Height;
        int radius;
        double sigma;
        ImageMetaData():Width(0),Height(0),radius(0),sigma(1){}
        ImageMetaData(int width_, int height_, int radius_, double sigma_):Width(width_),Height(height_),radius(radius_), sigma(sigma_){}
        ImageMetaData(const ImageMetaData& metaData):Width(metaData.Width),Height(metaData.Height),radius(metaData.radius), sigma(metaData.sigma){}
    };
    
    void SetImageMetaData(const ImageMetaData& metaData);
protected:

    int GetPixelPositionForEdges(int currentPixel, int edgesize);
    ImageMetaData* metaData;

};

#endif /* defined(__Archis__ImageFilterClass__) */
