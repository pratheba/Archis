//
//  ImageFilterFactoryClass.h
//  Archis
//
//  Created by Pratheba Selvaraju on 9/26/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef __Archis__ImageFilterFactoryClass__
#define __Archis__ImageFilterFactoryClass__

#include <iostream>
#include "GaussianFilterClass.h"

enum FILTERTYPE {
    GAUSSIAN = 0,
    SOBEL = 1,
    PREWITT = 2,
    ROBERTS = 3,
    LOG = 4,
    CANNY = 5
};

class ImageFilterFactoryClass {
public:
    ImageFilterFactoryClass();
    ~ImageFilterFactoryClass();
    
    ImageFilterClass* GetImageFilterClass(const Array2D<Rgba>& inputImage_, const int& windowRadius,FILTERTYPE filterType, const double& sigma =1.4);
    
private:
    
    ImageFilterClass* imageFilterClass;
   
};

#endif /* defined(__Archis__ImageFilterFactoryClass__) */
