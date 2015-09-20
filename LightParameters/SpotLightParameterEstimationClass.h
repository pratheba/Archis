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
#include "../Utility/Parser.h"
#include "../Algorithm/ReprojectionClass.h"


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
    
    std::vector<double>& GetExponentOfCoreRegion();
    std::vector<double>& GetExponentOfFallOffRegion();
    
private:
    
    struct InputForExponentCalculation {
        int imageWidth ;
        int imageHeight;
        
        double lightIntensity;
        Eigen::Vector3d lightDirection;
        Eigen::Vector3d lightPosition;
        Eigen::Vector3d vnormal;
        double cosOfInnerConeAngle;
        double cosOfOuterConeAngle;
        
        double materialAlbedo;
        
        std::vector<MapOFImageAndWorldPoints> reprojectedPoints;
        
        InputForExponentCalculation(int imageWidth_, int imageHeight_, double lightIntensity_, Eigen::Vector3d lightDirection_,
                    Eigen::Vector3d lightPosition_, Eigen::Vector3d vnormal_, double cosOfInnerConeAngle_,
                    double cosOfOuterConeAngle_, double materialAlbedo_, std::vector<MapOFImageAndWorldPoints> reprojectedPoints_):
                    imageWidth(imageWidth_), imageHeight(imageHeight_), lightIntensity(lightIntensity_),
                    lightDirection(lightDirection_), lightPosition(lightPosition_), vnormal(vnormal_),
                    cosOfInnerConeAngle(cosOfInnerConeAngle_), cosOfOuterConeAngle(cosOfOuterConeAngle_),
                    materialAlbedo(materialAlbedo_), reprojectedPoints(reprojectedPoints_){};
        
        ~InputForExponentCalculation(){}
    };
    
    static SpotLightParameterEstimationClass* _spotLightParamClass;
    
    ImageSystemClass& _imageSystem;
    CameraSystemClass& _cameraSystem;
    LightSystemClass& _lightSystem;
    GeometrySystemClass& _geometrySystem;
    
    std::vector<std::pair<int,double>> exponentOfCoreRegion;
    std::vector<std::pair<int,double>> exponentOfFallOffRegion;
    
    SpotLightParameterEstimationClass(const std::string& inputImageFileName);
    ~SpotLightParameterEstimationClass();
    
    void CalculateExponentParameter();
    InputForExponentCalculation* SpotLightExponentInputParameters();
    void CalculateExponentFromReprojectedPoints(InputForExponentCalculation* input, double averagePixelIntensityValue);
    void WriteExponentValueToImage(double maxExpValue, double minExpValue);
    
    
    
};

#endif /* defined(__Archis__SpotLightParameterEstimationClass__) */
