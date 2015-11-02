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
#include "Test/AttenuationClass.hpp"

InitializerClass::InitializerClass() {
}

InitializerClass::~InitializerClass() {
}

void InitializerClass::ParseResouceFileAndRunSpotLightParameterEstimation(const std::string& resourceFileName) {
    SpotLightParameterEstimationClass spotLightParamEstClass = SpotLightParameterEstimationClass::GetInstance();
    
    Parser *parser = new Parser();
    std::vector<std::string> resourceFiles = parser->ParseResourceFileAndFilesToBeProcessed(resourceFileName);
    std::string inputFileName = "/Users/prathebaselvaraju/4-Projects/Archis/Input/BackGroundImage.jpg";
    cv::Mat inputFile = cv::imread(inputFileName);
    
    for (int file = 0; file < resourceFiles.size(); ++file) {
        SystemClass* systemClass = new SystemClass();
        parser->ParseResourceFile(resourceFiles[file], *systemClass);
        spotLightParamEstClass.SetSystemClasses(*systemClass);
        //spotLightParamEstClass.GetSpotLightExponentFromImage();
        spotLightParamEstClass.SetTheInputParameters();
        
        AttenuationClass* attClass = new AttenuationClass(spotLightParamEstClass);
        
        attClass->CalculateAttenuationFactor(inputFile);
        //attClass->GetPixelCoordFromWorldPoints();
        
        delete systemClass;
        delete attClass;
    }
    delete parser;
    spotLightParamEstClass.Release();
}

