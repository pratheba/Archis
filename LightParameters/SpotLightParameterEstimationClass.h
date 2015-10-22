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
#include "InitializerClass.hpp"
#include "../Utility/Parser.h"
#include "../Algorithm/ReprojectionClass.h"
#include "../Utility/SystemClass.hpp"


class AttenuationClass;

class SpotLightParameterEstimationClass {
    friend class AttenuationClass;
public:
    
    static SpotLightParameterEstimationClass& GetInstance();
    static void Release();
    
    void SetSystemClasses(const SystemClass& systemClass);
    void GetSpotLightExponentFromImage();
    
    std::vector<double>& GetExponentOfCoreRegion();
    std::vector<double>& GetExponentOfFallOffRegion();
    
    // Testing class for calling attenuation class. To be removed.
    void SetTheInputParameters();
    
private:
    
    struct InputForExponentCalculation {
        int imageWidth ;
        int imageHeight;
        
        double lightIntensity;
        double gammaCorrection;
        Eigen::Vector3d lightDirection;
        Eigen::Vector3d lightPosition;
        Eigen::Vector3d vnormal;
        double cosOfInnerConeAngle;
        double cosOfOuterConeAngle;
        
        double materialAlbedo;
        
        std::vector<MapOFImageAndWorldPoints> reprojectedPoints;
        
        InputForExponentCalculation(int imageWidth_, int imageHeight_, double lightIntensity_,  Eigen::Vector3d lightDirection_,
                    Eigen::Vector3d lightPosition_, double gammaCorrection_,Eigen::Vector3d vnormal_, double cosOfInnerConeAngle_,
                    double cosOfOuterConeAngle_, double materialAlbedo_, std::vector<MapOFImageAndWorldPoints> reprojectedPoints_):
                    imageWidth(imageWidth_), imageHeight(imageHeight_), lightIntensity(lightIntensity_),lightDirection
                    (lightDirection_), lightPosition(lightPosition_), gammaCorrection(gammaCorrection_),vnormal(vnormal_),
                    cosOfInnerConeAngle(cosOfInnerConeAngle_), cosOfOuterConeAngle(cosOfOuterConeAngle_),
                    materialAlbedo(materialAlbedo_), reprojectedPoints(reprojectedPoints_){};
        
        ~InputForExponentCalculation(){}
    };
    
    static SpotLightParameterEstimationClass* _spotLightParamClass;
    
    SystemClass* _systemClass;
    ImageSystemClass* _imageSystem;
    CameraSystemClass* _cameraSystem;
    LightSystemClass* _lightSystem;
    GeometrySystemClass* _geometrySystem;
    static InputForExponentCalculation* _inputSetter;
    
    int _imageWidth;
    int _imageHeight;
    
    std::vector<std::pair<int,double>> exponentOfCoreRegion;
    std::vector<std::pair<int,double>> exponentOfFallOffRegion;
    
    SpotLightParameterEstimationClass();
    //~SpotLightParameterEstimationClass();
    
    void CalculateExponentParameter();
    InputForExponentCalculation* GetSpotLightExponentInputParameters();
    void CalculateExponentFromReprojectedPoints(InputForExponentCalculation* input, double averagePixelIntensityValue);
    void WriteExponentValueToImage(double maxExpValue, double minExpValue);
    void WriteExponentValueToImage();
    Rgba* GetImagePixelsToWrite();
    void WriteToImage(Rgba* outputPixels,const std::string& fileName);

    
    // Test
    void CalculatePixelValue();
};

#endif /* defined(__Archis__SpotLightParameterEstimationClass__) */
