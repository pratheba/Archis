//
//  Parser.h
//  Archis
//
//  Created by Pratheba Selvaraju on 8/23/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef __Archis__Parser__
#define __Archis__Parser__

#include <iostream>
#include <vector>

#include "UtilityClass.h"
#include "../Image/ImageSystemClass.h"
#include "../Scene/CameraSystemClass.h"
#include "../Scene/LightSystemClass.h"
#include "../Scene/GeometrySystemClass.h"

class Parser {
public:
   Parser();
    void ParseLuxRenderScene(const std::string& luxRenderFileName,  CameraSystemClass& cameraSystemClass,  LightSystemClass& lightSystemClass);
    void ParsePLYFile(const std::string& plyFileName,  GeometrySystemClass& geometrySystemClass);
    void ParseImageFile(const std::string& imageFileName,  ImageSystemClass& imageSystemClass);
    
private:
    
    static std::vector<std::string> SceneComponents;
        
};


#endif /* defined(__Archis__Parser__) */
