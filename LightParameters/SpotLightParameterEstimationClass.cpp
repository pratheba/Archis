
//  SpotLightParameterEstimationClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 8/27/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "SpotLightParameterEstimationClass.h"
#include "../Algorithm/SpotLightFallOFFIntensityCalculator.h"
#define EPSILON 0.0001

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
    _imageWidth = (int)_imageSystem.GetCurrentImage().GetImage2DArrayPixels().width();
    _imageHeight = (int)_imageSystem.GetCurrentImage().GetImage2DArrayPixels().height();
    
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
    
//    SpotLightFallOFFIntensityCalculator* spotLightFallOffCalc = new SpotLightFallOFFIntensityCalculator();
//    for (int index = 0; index < centroids.size(); ++index) {
//        spotLightFallOffCalc->GetLightFallOffPointsfromCorePoints((_imageSystem.GetCurrentImage()).GetImage2DArrayPixels(), Point2D<int>(centroids[index].y,centroids[index].x));
//    }
    CalculatePixelValue();
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
    double gammaCorrection          =  light.GetGammaCorrection();
    double cosOfInnerConeAngle      =  cos(innerconeangle);
    double cosOfOuterConeAngle      =  cos(outerconeangle);
    
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
    
    InputForExponentCalculation* input = new InputForExponentCalculation(imageWidth, imageHeight, lightIntensity, lightDirection, lightPosition, gammaCorrection,vnormal, cosOfInnerConeAngle, cosOfOuterConeAngle, materialAlbedo, reprojectedPoints);
    
    delete reprojectionClass;
    return input;
}

void SpotLightParameterEstimationClass::CalculateExponentParameter() {
    
    std::ofstream outputExponentValue;
    outputExponentValue.open("../../Output/outputExponentValue-intensity100-gamma1-blend1.txt");
 
    InputForExponentCalculation* input =  SpotLightExponentInputParameters();
    std::vector<MapOFImageAndWorldPoints>reprojectedPoints = input->reprojectedPoints;
    
    // Reprojected 3D points
    double gamma = input->gammaCorrection;
    double cosOfInner_minus_OuterConeAngle  =   input->cosOfInnerConeAngle - input->cosOfOuterConeAngle;
    if(cosOfInner_minus_OuterConeAngle == 0)
        cosOfInner_minus_OuterConeAngle = EPSILON;
    double maxExponentValue = DBL_MIN;
    double minExponentValue = DBL_MAX;
    
    std::vector<Point2D<int>> spotLightCoreExponentVectorPosition;
    for (int index = 0; index < reprojectedPoints.size(); ++index) {
        
        Rgba pixelIntensityValue = (_imageSystem.GetCurrentImage().GetImage2DArrayPixels())[reprojectedPoints[index]._imagepixel.y]
                                [reprojectedPoints[index]._imagepixel.x];
        double averagePixelIntensityValue   =  (powl(pixelIntensityValue.r,gamma) + powl(pixelIntensityValue.g,gamma) + powl(pixelIntensityValue.b,gamma))/3;
        
          // light does not reach that pixel;
        if (averagePixelIntensityValue <=  0) {
            continue;
        }
        
        Eigen::Vector3d lightToPointDirectionVector =  reprojectedPoints[index]._worldPoint - input->lightPosition;
        double distance = lightToPointDirectionVector.dot(lightToPointDirectionVector);
        lightToPointDirectionVector.normalize();

        float lambertTerm = (-lightToPointDirectionVector).dot(input->vnormal);
        // No light gets reflected from the surface at the particular point
        if (lambertTerm <= 0 ) {
            continue;
        }
    
        double cosOfCurrAngle                   =   input->lightDirection.dot(lightToPointDirectionVector);
        
        // Clamp the value of the curr between 0 and 1
        //double IntensityFactorWithoutExponent =  cosOfCurrAngle * std::min(std::max((double)((cosOfCurrAngle - input->cosOfOuterConeAngle) / (cosOfInner_minus_OuterConeAngle)),0.0),1.0);

        double IntensityFactorWithoutExponent = (double)((cosOfCurrAngle - input->cosOfOuterConeAngle) / (cosOfInner_minus_OuterConeAngle));
       
        if ((cosOfCurrAngle > input->cosOfOuterConeAngle) )
        {
            double attenuation = 1.0 / (1.0 + 1* distance + 1 * pow(distance, 2));
            double IntensityFactorWithExponent = averagePixelIntensityValue / (input->lightIntensity * input->materialAlbedo * lambertTerm * attenuation);
            double spotLightExponent = log(IntensityFactorWithExponent)/log(IntensityFactorWithoutExponent);
            
            int i = reprojectedPoints[index]._imagepixel.y * input->imageWidth + reprojectedPoints[index]._imagepixel.x;
            outputExponentValue << reprojectedPoints[index]._imagepixel.y << "," << reprojectedPoints[index]._imagepixel.x << " = " << spotLightExponent << std::endl;
            
            if ((maxExponentValue < spotLightExponent)&&(spotLightExponent <= 10)) {
                maxExponentValue = spotLightExponent;
            }
            if (minExponentValue > spotLightExponent) {
                minExponentValue = spotLightExponent;
            }
            
            exponentOfFallOffRegion.push_back(std::make_pair(i,spotLightExponent));
        }
    }
    
    WriteExponentValueToImage();
    WriteExponentValueToImage(maxExponentValue,minExponentValue);
    outputExponentValue.close();
    delete input;
}

void SpotLightParameterEstimationClass::CalculatePixelValue() {
    
    UtilityClass* util = new UtilityClass();
    Rgba* outputPixels = util->GetImagePixelsToWrite(_imageWidth, _imageHeight);
    
    std::ofstream outputPixelValue;
    outputPixelValue.open("../../Output/outputPixel-blend1.txt");
    
    std::ofstream outputOriginalPixelValue;
    outputOriginalPixelValue.open("../../Output/originalPixel-blend1.txt");
    
    for(int row = 0; row < _imageHeight; ++row) {
        for (int col = 0; col < _imageWidth; ++col) {
            Rgba p = (_imageSystem.GetCurrentImage().GetImage2DArrayPixels())[row][col];
            double averagePixelIntensityValue   =   (p.r + p.g + p.b)/3;
            
            outputOriginalPixelValue << row << "," << col << " = " << averagePixelIntensityValue << std::endl;
        }
    }
    
    InputForExponentCalculation* input =  SpotLightExponentInputParameters();
    std::vector<MapOFImageAndWorldPoints>reprojectedPoints = input->reprojectedPoints;
    double cosOfInner_minus_OuterConeAngle  =   input->cosOfInnerConeAngle - input->cosOfOuterConeAngle;
    if(cosOfInner_minus_OuterConeAngle == 0)
        cosOfInner_minus_OuterConeAngle = EPSILON;
    double gamma = 1.0/input->gammaCorrection;
    
    // Reprojected 3D points
    std::vector<Point2D<int>> spotLightCoreExponentVectorPosition;
    for (int index = 0; index < reprojectedPoints.size(); ++index) {
        
        Eigen::Vector3d lightToPointDirectionVector =  reprojectedPoints[index]._worldPoint - input->lightPosition;
        double distance = lightToPointDirectionVector.dot(lightToPointDirectionVector);
        lightToPointDirectionVector.normalize();
        
        float lambertTerm = (-lightToPointDirectionVector).dot(input->vnormal);
        // No light gets reflected from the surface at the particular point
        
        // Use it for attentuation factor
        double cosOfCurrAngle                   =   input->lightDirection.dot(lightToPointDirectionVector);
        double IntensityFactorExponent =  std::pow((double)((cosOfCurrAngle - input->cosOfOuterConeAngle) / (cosOfInner_minus_OuterConeAngle)), 4);
      
        Rgba pixelValue;
//        if(cosOfCurrAngle >= input->cosOfInnerConeAngle)
//        {
//            double v = lambertTerm * input->lightIntensity * input->materialAlbedo * attenuation;
//            pixelValue = Rgba(pow(v,gamma),pow(v,gamma),pow(v,gamma),1.0);
//                        
//        }
        if (cosOfCurrAngle > input->cosOfOuterConeAngle)
        {
            double attenuation = 1.0 / (1.0 + 1* distance + 1 * pow(distance, 2));
            if (lambertTerm > 0) {
                double v = (lambertTerm * input->lightIntensity * input->materialAlbedo * IntensityFactorExponent * attenuation);
                pixelValue = Rgba(pow(v,gamma),pow(v,gamma),pow(v,gamma),1.0);
            }
        }
        else
            pixelValue = Rgba(0,0,0,1);
        
        int i = reprojectedPoints[index]._imagepixel.y * input->imageWidth + reprojectedPoints[index]._imagepixel.x;
        outputPixelValue << reprojectedPoints[index]._imagepixel.y << "," << reprojectedPoints[index]._imagepixel.x << " = " << pixelValue.r << std::endl;
        outputPixels[i] = pixelValue;
    }
    
    WriteToImage(outputPixels, "imagepixel-blend1.exr");
    outputPixelValue.close();
    outputOriginalPixelValue.close();
    delete [] outputPixels;
    delete input;
}

void SpotLightParameterEstimationClass::WriteToImage(Rgba* outputPixels, const std::string& fileName) {
    UtilityClass* util = new UtilityClass();
    std::string outputfileName = "../../Output/" + fileName;
    util->WriteImage2DArrayPixels(outputfileName, outputPixels, _imageWidth, _imageHeight);
    delete util;
}

// No scaling
void SpotLightParameterEstimationClass::WriteExponentValueToImage() {
    UtilityClass* util = new UtilityClass();
    Rgba* outputPixels = util->GetImagePixelsToWrite(_imageWidth,_imageHeight);
    
    for (int index = 0; index < exponentOfFallOffRegion.size(); ++index)
        outputPixels[exponentOfFallOffRegion[index].first].g = exponentOfFallOffRegion[index].second;
    WriteToImage(outputPixels,"ExponentValue1.exr");
    delete [] outputPixels;
    delete util;
}

// No scaling
void SpotLightParameterEstimationClass::WriteExponentValueToImage(double maxExpValue, double minExpValue) {
    UtilityClass* util = new UtilityClass();
    Rgba* outputPixels = util->GetImagePixelsToWrite(_imageWidth,_imageHeight);
    
    // convert expoenentValues between 0 and 1
    std::vector<double>spotLightFallOffExponentVector = std::vector<double>(exponentOfFallOffRegion.size(),0);
    for (int index = 0; index < exponentOfFallOffRegion.size(); ++index) {
        spotLightFallOffExponentVector[index] = ((exponentOfFallOffRegion[index].second) - minExpValue)/(maxExpValue - minExpValue);
        outputPixels[exponentOfFallOffRegion[index].first].g = spotLightFallOffExponentVector[index];
    }
     WriteToImage(outputPixels,"ExponentScaled1.exr");
    delete[] outputPixels;
    delete util;
}


