//
//  InitializerClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 10/16/15.
//  Copyright © 2015 Pratheba Selvaraju. All rights reserved.
//

#include "InitializerClass.hpp"
#include "Parser.h"
#include "LightParameters/SpotLightParameterEstimationClass.h"
#include "Utility/SystemClass.hpp"

InitializerClass::InitializerClass() {
}

InitializerClass::~InitializerClass() {
}

void InitializerClass::ParseResouceFileAndRunSpotLightParameterEstimation(const std::string& resourceFileName) {
    SpotLightParameterEstimationClass spotLightParamEstClass = SpotLightParameterEstimationClass::GetInstance();
    
    Parser *parser = new Parser();
    std::vector<std::string> resourceFiles = parser->ParseResourceFileAndFilesToBeProcessed(resourceFileName);
    
    for (int file = 0; file < resourceFiles.size(); ++file) {
        SystemClass* systemClass = new SystemClass();
        parser->ParseResourceFile(resourceFiles[file], *systemClass);
        spotLightParamEstClass.SetSystemClasses(*systemClass);
        spotLightParamEstClass.GetSpotLightExponentFromImage();
        delete systemClass;
    }
    delete parser;
    spotLightParamEstClass.Release();
}

