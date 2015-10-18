//
//  SystemClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 10/17/15.
//  Copyright © 2015 Pratheba Selvaraju. All rights reserved.
//

#include "SystemClass.hpp"

SystemClass::SystemClass():_imageSystemClass(ImageSystemClass::GetInstance()), _cameraSystemClass(CameraSystemClass::GetInstance()),_geometrySystemClass(GeometrySystemClass::GetInstance()),_lightSystemClass(LightSystemClass::GetInstance()) {
    
}

SystemClass::~SystemClass() {
    _imageSystemClass.ReleaseInstance();
    _cameraSystemClass.ReleaseInstance();
    _lightSystemClass.ReleaseInstance();
    _geometrySystemClass.ReleaseInstance();
}

