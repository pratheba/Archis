//
//  Image2DClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 8/26/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "Image2DClass.h"


Image2DClass::Image2DClass() {
    imagePixels.resizeErase(0, 0);
}

Image2DClass::Image2DClass(const std::string& inputFileName) {

    try {
        RgbaInputFile file(inputFileName.c_str());
        Imath_2_2::Box2i dataWindow = file.dataWindow();
        int width  = dataWindow.max.x - dataWindow.min.x + 1; // col
        int height = dataWindow.max.y - dataWindow.min.y + 1; // row
        imagePixels.resizeErase(height, width);
        file.setFrameBuffer(&imagePixels[0][0] - dataWindow.min.x - dataWindow.min.y  * width, 1, width);
        file.readPixels(dataWindow.min.y, dataWindow.max.y);
    }
    catch (const std::exception &exc)
    {
        std::cerr << exc.what() << std::endl;
    }
}

const Array2D<Rgba>& Image2DClass::GetImage2DArrayPixels()const {
    return imagePixels;
}




