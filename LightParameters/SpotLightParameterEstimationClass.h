//
//  SpotLightParameterEstimationClass.h
//  Archis
//
//  Created by Pratheba Selvaraju on 8/27/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef __Archis__SpotLightParameterEstimationClass__
#define __Archis__SpotLightParameterEstimationClass__

#include <iostream>
#include "Parser.h"
#include "ReprojectionClass.h"


struct INPUTPARAM {
    std::string _inputImageFileName;
    std::string _inputLuxFileName;
    std::string _inputPLYFileName;
    
    INPUTPARAM(const std::string& imageFile, const std::string& luxFile, const std::string& plyFile):_inputImageFileName(imageFile),
    _inputLuxFileName(luxFile),_inputPLYFileName(plyFile){};
};


class SpotLightParameterEstimationClass {
public:
    static SpotLightParameterEstimationClass& GetInstance(const std::string& inputImageFileName);
    static void Release();
    
    void Initialize(const INPUTPARAM& inputParameters);
    void GetSpotLightExponentFromImage(const INPUTPARAM& inputParameters);
    
private:
    static SpotLightParameterEstimationClass* _spotLightParamClass;
    
    ImageSystemClass& _imageSystem;
    CameraSystemClass& _cameraSystem;
    LightSystemClass& _lightSystem;
    GeometrySystemClass& _geometrySystem;
    
    
    SpotLightParameterEstimationClass(const std::string& inputImageFileName);
    ~SpotLightParameterEstimationClass();
    
    void CalculateExponentParameter();
    
};

#endif /* defined(__Archis__SpotLightParameterEstimationClass__) */
