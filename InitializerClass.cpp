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
    
    
    std::vector<std::vector<std::vector<Point2D<double>>>> attenuationVsDistance;
    AttenuationClass* attClass = nullptr;
    for (int file = 0; file < resourceFiles.size(); ++file) {
        SystemClass* systemClass = new SystemClass();
        parser->ParseResourceFile(resourceFiles[file], *systemClass);
        spotLightParamEstClass.SetSystemClasses(*systemClass);
        //spotLightParamEstClass.GetSpotLightExponentFromImage();
        spotLightParamEstClass.SetTheInputParameters();
        
        if (attClass == nullptr) {
            attClass = new AttenuationClass(spotLightParamEstClass);
            attClass->SetNumberOfInputFilesForCalculatingAttenuationFactor((int)resourceFiles.size());
        } else {
            attClass->Reinitialize(spotLightParamEstClass);
        }
        
        attClass->CalculateAttenuationFactor(file);
        //attClass->DrawGraph(inputFile);
        //attClass->GetPixelCoordFromWorldPoints();
        
        delete systemClass;
     //   delete attClass;
    }
    attClass->DrawGraph(inputFile);
    delete parser;
    delete attClass;
    spotLightParamEstClass.Release();
}

