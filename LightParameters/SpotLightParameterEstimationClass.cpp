
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
SpotLightParameterEstimationClass::InputForExponentCalculation* SpotLightParameterEstimationClass::_inputSetter = nullptr;

SpotLightParameterEstimationClass& SpotLightParameterEstimationClass::GetInstance() {
    if (_spotLightParamClass == nullptr) {
        _spotLightParamClass = new SpotLightParameterEstimationClass();
    }
    return *_spotLightParamClass;
}

SpotLightParameterEstimationClass::SpotLightParameterEstimationClass() {
    _systemClass = nullptr;
    _imageSystem = nullptr;
    _cameraSystem = nullptr;
    _geometrySystem = nullptr;
    _lightSystem = nullptr;
    _inputSetter = nullptr;
    exponentOfCoreRegion.clear();
    exponentOfFallOffRegion.clear();
}

void SpotLightParameterEstimationClass::Release() {
    if (_spotLightParamClass != nullptr) {
        
        if (_inputSetter != nullptr) {
            delete _inputSetter;
            _inputSetter = nullptr;
        }
        delete _spotLightParamClass;
        _spotLightParamClass = nullptr;
    }
}

void SpotLightParameterEstimationClass::SetSystemClasses(const SystemClass& systemClass) {
    _systemClass = new SystemClass();
    *_systemClass = systemClass;
    
    *_lightSystem = systemClass._lightSystemClass;
    *_imageSystem = systemClass._imageSystemClass;
    *_cameraSystem = systemClass._cameraSystemClass;
    *_geometrySystem = systemClass._geometrySystemClass;

}

void SpotLightParameterEstimationClass::SetTheInputParameters() {
    if (_inputSetter == nullptr) {
        GetSpotLightExponentInputParameters();
    }
}

void SpotLightParameterEstimationClass::GetSpotLightExponentFromImage() {
    if (_systemClass == nullptr) {
        std::cout << "The input parameters have not been initialized. Initialize the systemclass before calling into the function" << std::endl;
        return;
    }
    VecOf2dDoublePoints centroids = _imageSystem->GetCentroidsOfHighIntensityPixelsinCurrentImage();
    
    //    SpotLightFallOFFIntensityCalculator* spotLightFallOffCalc = new SpotLightFallOFFIntensityCalculator();
    //    for (int index = 0; index < centroids.size(); ++index) {
    //        spotLightFallOffCalc->GetLightFallOffPointsfromCorePoints((_imageSystem.GetCurrentImage()).GetImage2DArrayPixels(), Point2D<int>(centroids[index].y,centroids[index].x));
    //    }
    CalculatePixelValue();
    CalculateExponentParameter();
}

SpotLightParameterEstimationClass::InputForExponentCalculation* SpotLightParameterEstimationClass::GetSpotLightExponentInputParameters() {
    
    if (_inputSetter != nullptr) {
        return _inputSetter;
    }
    const LightEntityClass& light = _lightSystem->GetCurrentLight();
    double materialAlbedo = 0.5;
    
    // Light
    double lightIntensity           = _lightSystem->GetCurrentLightIntensity();
    Eigen::Vector3d lightDirection  = -light.GetDirectionVector();
    lightDirection.normalize();
    Eigen::Vector3d lightPosition   =  light.GetPosition();
    double innerconeangle           =  light.GetSpotLightInnerConeAngle() * M_PI / 180;
    double outerconeangle           =  light.GetSpotLightOuterConeAngle() * M_PI / 180;
    double gammaCorrection          =  light.GetGammaCorrection();
    double cosOfInnerConeAngle      =  cos(innerconeangle);
    double cosOfOuterConeAngle      =  cos(outerconeangle);
    
    // Geometry
    const GeometryEntityClass& geometryEntity = _geometrySystem->GetCurrentGeometry();
   // Point3D<double> vertex = geometryEntity.GetAVertex();
    std::vector<Eigen::Vector3d> vertexNormals = geometryEntity.GetVertexNormals();
    Eigen::Vector3d vnormal = vertexNormals[0];
    vnormal.normalize();
    
    // Image
    _imageWidth = (int)_imageSystem->GetCurrentImageWidth();
    _imageHeight = (int)_imageSystem->GetCurrentImageHeight();
    
    ReprojectionClass* reprojectionClass = new ReprojectionClass();
    std::vector<MapOFImageAndWorldPoints>reprojectedPoints;
    // Uncomment this if we are going to pass in Array<RGBA> of the pixels that we obtained from the circumference of the illumination
    //reprojectedPoints = reprojectionClass->ReprojectImagePixelsTo3DGeometry((_imageSystem->GetCurrentImage()).GetImage2DArrayPixels());
    reprojectedPoints = reprojectionClass->ReprojectImagePixelsTo3DGeometry();
    
    _inputSetter = new InputForExponentCalculation(_imageWidth, _imageHeight, lightIntensity, lightDirection, lightPosition, gammaCorrection,vnormal, cosOfInnerConeAngle, cosOfOuterConeAngle, materialAlbedo, reprojectedPoints);
    
    delete reprojectionClass;
    return _inputSetter;
}

void SpotLightParameterEstimationClass::CalculateExponentParameter() {
    
    std::ofstream outputExponentValue;
    outputExponentValue.open("../../Output/outputExponentValue-intensity100-gamma1-blend1.txt");
 
    _inputSetter =  GetSpotLightExponentInputParameters();
    std::vector<MapOFImageAndWorldPoints>reprojectedPoints = _inputSetter->reprojectedPoints;
    
    // Reprojected 3D points
    double gamma = _inputSetter->gammaCorrection;
    double cosOfInner_minus_OuterConeAngle  =   _inputSetter->cosOfInnerConeAngle - _inputSetter->cosOfOuterConeAngle;
    if(cosOfInner_minus_OuterConeAngle == 0)
        cosOfInner_minus_OuterConeAngle = EPSILON;
    double maxExponentValue = DBL_MIN;
    double minExponentValue = DBL_MAX;
    
    //double r = _inputSetter->vnormal.dot(Eigen::Vector3d(-4,-4,2) - _inputSetter->lightPosition);
    
    std::vector<Point2D<int>> spotLightCoreExponentVectorPosition;
    for (int index = 0; index < reprojectedPoints.size(); ++index) {
        
        Rgba pixelIntensityValue =(_imageSystem->GetCurrentImage().GetImage2DArrayPixels())[reprojectedPoints[index]._imagepixel.y][reprojectedPoints[index]._imagepixel.x];
        double averagePixelIntensityValue   =  (powl(pixelIntensityValue.r,gamma) + powl(pixelIntensityValue.g,gamma) + powl(pixelIntensityValue.b,gamma))/3;
        
          // light does not reach that pixel;
        if (averagePixelIntensityValue <=  0) {
            continue;
        }
        
        Eigen::Vector3d lightToPointDirectionVector =  reprojectedPoints[index]._worldPoint - _inputSetter->lightPosition;
        double distance = std::sqrt(lightToPointDirectionVector.dot(lightToPointDirectionVector));
        lightToPointDirectionVector.normalize();

        float lambertTerm = (-lightToPointDirectionVector).dot(_inputSetter->vnormal);
        // No light gets reflected from the surface at the particular point
        if (lambertTerm <= 0 ) {
            continue;
        }
    
        double cosOfCurrAngle                   =   _inputSetter->lightDirection.dot(lightToPointDirectionVector);
        
        // Clamp the value of the curr between 0 and 1
        //double IntensityFactorWithoutExponent =  cosOfCurrAngle * std::min(std::max((double)((cosOfCurrAngle - input->cosOfOuterConeAngle) / (cosOfInner_minus_OuterConeAngle)),0.0),1.0);

        double IntensityFactorWithoutExponent = (double)((cosOfCurrAngle - _inputSetter->cosOfOuterConeAngle) / (cosOfInner_minus_OuterConeAngle));
       
        if ((cosOfCurrAngle > _inputSetter->cosOfOuterConeAngle) )
        {
            //double attenuation = 1.0 / (1.0 + (2/r)* distance + (1/(r*r)) * pow(distance, 2));
            double attenuation = 1.0 / (distance*distance);
            double IntensityFactorWithExponent = averagePixelIntensityValue / (_inputSetter->lightIntensity * _inputSetter->materialAlbedo * lambertTerm * attenuation);
            double spotLightExponent = log(IntensityFactorWithExponent)/log(IntensityFactorWithoutExponent);
            
            int i = reprojectedPoints[index]._imagepixel.y * _inputSetter->imageWidth + reprojectedPoints[index]._imagepixel.x;
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
    
}

void SpotLightParameterEstimationClass::CalculatePixelValue() {
    
    _inputSetter =  GetSpotLightExponentInputParameters();
    
    UtilityClass* util = new UtilityClass();
    Rgba* outputPixels = util->GetImagePixelsToWrite(_imageWidth, _imageHeight);
    
    std::ofstream outputPixelValue;
    outputPixelValue.open("../../Output/outputPixel-blend1.txt");
    
    std::ofstream outputOriginalPixelValue;
    outputOriginalPixelValue.open("../../Output/originalPixel-blend1.txt");
    
    
    for(int row = 0; row < _imageHeight; ++row) {
        for (int col = 0; col < _imageWidth; ++col) {
            Rgba p = (_imageSystem->GetCurrentImage().GetImage2DArrayPixels())[row][col];
            double averagePixelIntensityValue   =   (p.r + p.g + p.b)/3;
            
            outputOriginalPixelValue << row << "," << col << " = " << averagePixelIntensityValue << std::endl;
        }
    }
    
    std::vector<MapOFImageAndWorldPoints>reprojectedPoints = _inputSetter->reprojectedPoints;
    double cosOfInner_minus_OuterConeAngle  =   _inputSetter->cosOfInnerConeAngle - _inputSetter->cosOfOuterConeAngle;
    if(cosOfInner_minus_OuterConeAngle == 0)
        cosOfInner_minus_OuterConeAngle = EPSILON;
    double gamma = 1.0/_inputSetter->gammaCorrection;
    
    // Reprojected 3D points
    std::vector<Point2D<int>> spotLightCoreExponentVectorPosition;
    double r = _inputSetter->vnormal.dot(Eigen::Vector3d(-4,-4,2) - _inputSetter->lightPosition);
    
    for (int index = 0; index < reprojectedPoints.size(); ++index) {
        
        Eigen::Vector3d lightToPointDirectionVector =  reprojectedPoints[index]._worldPoint - _inputSetter->lightPosition;
        double distance = std::sqrt(lightToPointDirectionVector.dot(lightToPointDirectionVector));
        lightToPointDirectionVector.normalize();
        
        float lambertTerm = (-lightToPointDirectionVector).dot(_inputSetter->vnormal);
        // No light gets reflected from the surface at the particular point
        
        // Use it for attentuation factor
        double cosOfCurrAngle                   =   _inputSetter->lightDirection.dot(lightToPointDirectionVector);
        double IntensityFactorExponent =  std::pow((double)((cosOfCurrAngle - _inputSetter->cosOfOuterConeAngle) / (cosOfInner_minus_OuterConeAngle)), 4);
      
        Rgba pixelValue;
//        if(cosOfCurrAngle >= input->cosOfInnerConeAngle)
//        {
//            double v = lambertTerm * input->lightIntensity * input->materialAlbedo * attenuation;
//            pixelValue = Rgba(pow(v,gamma),pow(v,gamma),pow(v,gamma),1.0);
//                        
//        }
        if (cosOfCurrAngle > _inputSetter->cosOfOuterConeAngle)
        {
            //double attenuation = 1.0 / (1.0 + (2/r)* distance + (1/(r*r)) * pow(distance, 2));
            double attenuation = 1.0 / (r*r)*(distance*distance);
            if (lambertTerm > 0) {
                double v = (lambertTerm * _inputSetter->lightIntensity * _inputSetter->materialAlbedo * IntensityFactorExponent * attenuation);
                pixelValue = Rgba(pow(v,gamma),pow(v,gamma),pow(v,gamma),1.0);
            }
        }
        else
            pixelValue = Rgba(0,0,0,1);
        
        int i = reprojectedPoints[index]._imagepixel.y * _inputSetter->imageWidth + reprojectedPoints[index]._imagepixel.x;
        outputPixelValue << reprojectedPoints[index]._imagepixel.y << "," << reprojectedPoints[index]._imagepixel.x << " = " << pixelValue.r << std::endl;
        outputPixels[i] = pixelValue;
    }
    
    WriteToImage(outputPixels, "imagepixel-blend1.exr");
    outputPixelValue.close();
    outputOriginalPixelValue.close();
    delete [] outputPixels;
    
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

// scaling
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


