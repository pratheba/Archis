
//  SpotLightParameterEstimationClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 8/27/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "SpotLightParameterEstimationClass.h"
#include "../Algorithm/SpotLightFallOFFIntensityCalculator.h"

SpotLightParameterEstimationClass* SpotLightParameterEstimationClass::_spotLightParamClass = nullptr;

SpotLightParameterEstimationClass& SpotLightParameterEstimationClass::GetInstance(const std::string& inputImageFileName) {
    if (_spotLightParamClass == nullptr) {
        _spotLightParamClass = new SpotLightParameterEstimationClass(inputImageFileName);
    }
    return *_spotLightParamClass;
}

SpotLightParameterEstimationClass::SpotLightParameterEstimationClass(const std::string& inputImageFileName):
_imageSystem(ImageSystemClass::GetInstance(inputImageFileName)),
_cameraSystem(CameraSystemClass::GetInstance()),
_lightSystem(LightSystemClass::GetInstance()),
_geometrySystem(GeometrySystemClass::GetInstance())
{
    exponentOfCoreRegion.clear();
    exponentOfFallOffRegion.clear();
}

void SpotLightParameterEstimationClass::Initialize(const INPUTPARAM& inputParameters) {
    Parser* parser = new Parser();
    parser->ParseImageFile(inputParameters._inputImageFileName,_imageSystem );
    parser->ParseLuxRenderScene(inputParameters._inputLuxFileName,_cameraSystem,_lightSystem);
    parser->ParsePLYFile(inputParameters._inputPLYFileName,_geometrySystem);
    delete parser;
}

SpotLightParameterEstimationClass::~SpotLightParameterEstimationClass() {
    ImageSystemClass::ReleaseInstance();
    GeometrySystemClass::ReleaseInstance();
    LightSystemClass::ReleaseInstance();
    CameraSystemClass::ReleaseInstance();
}


void SpotLightParameterEstimationClass::GetSpotLightExponentFromImage(const INPUTPARAM& inputParameters) {
    Initialize(inputParameters);
    VecOf2dDoublePoints centroids = _imageSystem.GetCentroidsOfHighIntensityPixelsinCurrentImage();
    
    SpotLightFallOFFIntensityCalculator* spotLightFallOffCalc = new SpotLightFallOFFIntensityCalculator();
    for (int index = 0; index < centroids.size(); ++index) {
        spotLightFallOffCalc->GetLightFallOffPointsfromCorePoints((_imageSystem.GetCurrentImage()).GetImage2DArrayPixels(), Point2D<int>(centroids[index].y,centroids[index].x));
    }
    
    CalculateExponentParameter();
}

SpotLightParameterEstimationClass::InputForExponentCalculation* SpotLightParameterEstimationClass::SpotLightExponentInputParameters() {
    const LightEntityClass& light = _lightSystem.GetCurrentLight();
    double materialAlbedo = 0.5;
    
    // Light
    double lightIntensity           = _lightSystem.GetCurrentLightIntensity();
    Eigen::Vector3d lightDirection  = -light.GetDirectionVector();
    lightDirection.normalize();
    Eigen::Vector3d lightPosition   =  light.GetPosition();
    double innerconeangle           =  light.GetSpotLightInnerConeAngle() * M_PI / 180;
    double outerconeangle           =  light.GetSpotLightOuterConeAngle() * M_PI / 180;
    double cosOfInnerConeAngle      = cos(innerconeangle);
    double cosOfOuterConeAngle      = cos(outerconeangle);
    
    
    // Geometry
    const GeometryEntityClass& geometryEntity = _geometrySystem.GetCurrentGeometry();
    std::vector<Eigen::Vector3d> vertexNormals = geometryEntity.GetVertexNormals();
    Eigen::Vector3d vnormal = vertexNormals[0];
    vnormal.normalize();
    
    // Image
    int imageWidth = (int)_imageSystem.GetCurrentImage().GetImage2DArrayPixels().width();
    int imageHeight = (int)_imageSystem.GetCurrentImage().GetImage2DArrayPixels().height();
    
    ReprojectionClass* reprojectionClass = new ReprojectionClass();
    std::vector<MapOFImageAndWorldPoints>reprojectedPoints;
    reprojectedPoints = reprojectionClass->ReprojectImagePixelsTo3DGeometry((_imageSystem.GetCurrentImage()).GetImage2DArrayPixels());
    
    InputForExponentCalculation* input = new InputForExponentCalculation(imageWidth, imageHeight, lightIntensity, lightDirection, lightPosition, vnormal, cosOfInnerConeAngle, cosOfOuterConeAngle, materialAlbedo, reprojectedPoints);
    
    delete reprojectionClass;
    return input;
}

void SpotLightParameterEstimationClass::CalculateExponentParameter() {
    
    std::ofstream outputExponentValue;
    outputExponentValue.open("../../Output/outputExponentValue-intensity100-gamma1.txt");
 
    InputForExponentCalculation* input =  SpotLightExponentInputParameters();
    std::vector<MapOFImageAndWorldPoints>reprojectedPoints = input->reprojectedPoints;
    
    // Reprojected 3D points
    double maxExponentValue = DBL_MIN;
    double minExponentValue = DBL_MAX;
    
    std::vector<Point2D<int>> spotLightCoreExponentVectorPosition;
    for (int index = 0; index < reprojectedPoints.size(); ++index) {
        
        Rgba pixelIntensityValue =   (_imageSystem.GetCurrentImage().GetImage2DArrayPixels())[reprojectedPoints[index]._imagepixel.y]
                                [reprojectedPoints[index]._imagepixel.x];
        double averagePixelIntensityValue   =   (pixelIntensityValue.r + pixelIntensityValue.g + pixelIntensityValue.b)/3;
        
          // light does not reach that pixel;
        if (averagePixelIntensityValue <=  0) {
            continue;
        }
        
        // convert average pixel intensity to 0 to 255
        // averagePixelIntensityValue =  (averagePixelIntensityValue /(pow(2, 16)-1));//*255;
        
        Eigen::Vector3d lightToPointDirectionVector =  reprojectedPoints[index]._worldPoint - input->lightPosition;
        lightToPointDirectionVector.normalize();

        float lambertTerm = (-lightToPointDirectionVector).dot(input->vnormal);
        // No light gets reflected from the surface at the particular point
        if (lambertTerm <= 0 ) {
            continue;
        }
        
        // Use it for attentuation factor
        double cosOfCurrAngle                   =   input->lightDirection.dot(lightToPointDirectionVector);
        double cosOfInner_minus_OuterConeAngle  =   input->cosOfInnerConeAngle - input->cosOfOuterConeAngle;
        
        // Clamp the value of the curr between 0 and 1
        double IntensityFactorWithoutExponent =  std::min(std::max((double)((cosOfCurrAngle - input->cosOfOuterConeAngle) / cosOfInner_minus_OuterConeAngle),0.0),1.0);
        
        if ((cosOfCurrAngle > input->cosOfOuterConeAngle) )
        {
            double IntensityFactorWithExponent = averagePixelIntensityValue / (input->lightIntensity * input->materialAlbedo *
                                                                               lambertTerm);
            double spotLightExponent = log(IntensityFactorWithExponent)/log(IntensityFactorWithoutExponent);
            
            int i = reprojectedPoints[index]._imagepixel.y * input->imageWidth + reprojectedPoints[index]._imagepixel.x;
            
            outputExponentValue << reprojectedPoints[index]._imagepixel.y << "," << reprojectedPoints[index]._imagepixel.x << " = " << spotLightExponent << std::endl;
            
            if (spotLightExponent > 10) {
                spotLightCoreExponentVectorPosition.push_back(Point2D<int>(reprojectedPoints[index]._imagepixel.y, reprojectedPoints[index]._imagepixel.x));
                exponentOfCoreRegion.push_back(std::make_pair(i,spotLightExponent));
                continue;
            }
            if ((maxExponentValue < spotLightExponent)&&(spotLightExponent <= 10)) {
                maxExponentValue = spotLightExponent;
            }
            if (minExponentValue > spotLightExponent) {
                minExponentValue = spotLightExponent;
            }
            exponentOfFallOffRegion.push_back(std::make_pair(i,spotLightExponent));
        }
    }
    
    WriteExponentValueToImage(maxExponentValue, minExponentValue);
    
    outputExponentValue.close();
    delete input;
}



void SpotLightParameterEstimationClass::WriteExponentValueToImage(double maxExpValue, double minExpValue) {
    
    int imageWidth = (int)_imageSystem.GetCurrentImage().GetImage2DArrayPixels().width();
    int imageHeight = (int)_imageSystem.GetCurrentImage().GetImage2DArrayPixels().height();

    UtilityClass* util = new UtilityClass();
    Rgba* outputPixels = util->GetImagePixelsToWrite(imageWidth, imageHeight);
    
    // convert expoenentValues between 0 and 1
    std::vector<double>spotLightFallOffExponentVector = std::vector<double>(exponentOfFallOffRegion.size(),0);
    
    for (int index = 0; index < exponentOfFallOffRegion.size(); ++index) {
        spotLightFallOffExponentVector[index] = ((exponentOfFallOffRegion[index].second) - minExpValue)/(maxExpValue - minExpValue);
        
        if (spotLightFallOffExponentVector[index] > 0.5)
            outputPixels[exponentOfFallOffRegion[index].first].b = spotLightFallOffExponentVector[index];
        else
            outputPixels[exponentOfFallOffRegion[index].first].g = spotLightFallOffExponentVector[index];
    }
    
    for (int index = 0; index < exponentOfCoreRegion.size(); ++index) {
        outputPixels[exponentOfCoreRegion[index].first].r = 1.0;
    }

    _imageSystem.GetCurrentImage().WriteImage2DArrayPixels("../../Output/imagePixelValueExponent-intensity100-gamma1.exr", outputPixels, imageWidth, imageHeight);
   

    delete [] outputPixels;
    delete util;

}




