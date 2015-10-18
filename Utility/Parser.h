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
#include "SystemClass.hpp"


class Parser {
public:
   Parser();
   
    void ParseResourceFile(const std::string& resourceFileName,  SystemClass& systemClass);
    std::vector<std::string> ParseResourceFileAndFilesToBeProcessed(const std::string& resourceFileName);
    
private:
    
    void ParseSceneFile(const std::string& fileName,SystemClass& systemClass);
    bool CheckIfEndOfModule(std::string& line);
    
    void ParseImageModule(std::ifstream& sceneFileName,ImageSystemClass& imageSystemClass);
    void ParseCameraModule(std::ifstream& sceneFileName,CameraSystemClass& cameraSystemClass);
    void ParseLightModule(std::ifstream& sceneFileName,LightSystemClass& lightSystemClass);
    void ParseGeometryModule(std::ifstream& sceneFileName,GeometrySystemClass& geometrySystemClass);
        
};


#endif /* defined(__Archis__Parser__) */
