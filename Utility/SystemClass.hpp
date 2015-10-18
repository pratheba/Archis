//
//  SystemClass.hpp
//  Archis
//
//  Created by Pratheba Selvaraju on 10/17/15.
//  Copyright © 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef SystemClass_hpp
#define SystemClass_hpp

#include <iostream>
#include "../Image/ImageSystemClass.h"
#include "../Scene/CameraSystemClass.h"
#include "../Scene/LightSystemClass.h"
#include "../Scene/GeometrySystemClass.h"

class SystemClass {
public:
    SystemClass();
    ~SystemClass();
    
    SystemClass& operator=(const SystemClass& systemClass) {
        return *this;
    }
    
    ImageSystemClass& _imageSystemClass;
    CameraSystemClass& _cameraSystemClass;
    LightSystemClass& _lightSystemClass;
    GeometrySystemClass& _geometrySystemClass;
    
private:
//    
//    ImageSystemClass& _imageSystemClass;
//    CameraSystemClass& _cameraSystemClass;
//    LightSystemClass& _lightSystemClass;
//    GeometrySystemClass& _geometrySystemClass;
    
};


#endif /* SystemClass_hpp */
