//
//  ImageSystemClass.h
//  Archis
//
//  Created by Pratheba Selvaraju on 8/26/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef __Archis__ImageSystemClass__
#define __Archis__ImageSystemClass__

#include "Image2DClass.h"
#include "UtilityClass.h"

class ImageSystemClass {
public:
    static ImageSystemClass& GetInstance();
    static ImageSystemClass& GetInstance(const std::string& inputImageFileName);
    static void ReleaseInstance();
    
    const Image2DClass& GetCurrentImage()const;
    void const SetCurrentImage(const Image2DClass& currentImage);
    
    const long GetCurrentImageWidth() const;
    const long GetCurrentImageHeight() const;
    
    
    
    VecOf2dDoublePoints GetCentroidsOfHighIntensityPixelsinCurrentImage();
    
private:
    ImageSystemClass();
    ImageSystemClass(const std::string& inputImageFileName);
    ImageSystemClass(const ImageSystemClass &imageSystemClass);
    ~ImageSystemClass();
    
    
    static ImageSystemClass* _imageSystem;
    static Image2DClass* _currentImage;
    
    //std::vector<Image2DClass*> _images;
    
};

#endif /* defined(__Archis__ImageSystemClass__) */
