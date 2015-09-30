//
//  CannyFilterClass.hpp
//  Archis
//
//  Created by Pratheba Selvaraju on 9/27/15.
//  Copyright © 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef CannyFilterClass_hpp
#define CannyFilterClass_hpp

#include <iostream>
#include "ImageFilterClass.h"

class CannyFilterClass : public ImageFilterClass {
public:
    CannyFilterClass();
    ~CannyFilterClass();
    
    void ProcessImage(const Array2D<Rgba>& inputImage_,Array2D<Rgba>& outputImage);

private:
    
};

#endif /* CannyFilterClass_hpp */
