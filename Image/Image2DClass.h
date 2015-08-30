//
//  Image2DClass.h
//  Archis
//
//  Created by Pratheba Selvaraju on 8/26/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef __Archis__Image2DClass__
#define __Archis__Image2DClass__

#include <iostream>
#include <OpenEXR/ImfRgbaFile.h>
#include <OpenEXR/ImfStringAttribute.h>
#include <OpenEXR/ImfMatrixAttribute.h>
#include <OpenEXR/ImfArray.h>

using namespace Imf_2_2;


class Image2DClass {
public:
    Image2DClass();
    Image2DClass(const std::string& inputFileName);
    
    const Array2D<Rgba>& GetImage2DArrayPixels()const;
    
private:
    Array2D<Rgba> imagePixels;
    
};

#endif /* defined(__Archis__Image2DClass__) */
