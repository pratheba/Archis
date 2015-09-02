
//  SpotLightParameterEstimationClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 8/27/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "SpotLightParameterEstimationClass.h"

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
    //VecOf2dDoublePoints centroids = _imageSystem.GetCentroidsOfHighIntensityPixelsinCurrentImage();
    
    CalculateExponentParameter();
}


// Refactor 
void SpotLightParameterEstimationClass::CalculateExponentParameter() {
    Rgba pixelIntensityValue;
    std::ofstream outputfile;
    outputfile.open("AlphaTest1.txt");
    
    const LightEntityClass& light = _lightSystem.GetCurrentLight();
    double materialAlbedo = 0.5;
    
    // Light
    double lightIntensity           = light.GetLightgain();
    Eigen::Vector3d lightDirection  = -light.GetDirectionVector();
    lightDirection.normalize();
    Eigen::Vector3d lightPosition   = light.GetPosition();
    
    // Geometry
    const GeometryEntityClass& geometryEntity = _geometrySystem.GetCurrentGeometry();
    std::vector<Eigen::Vector3d> vertexNormals = geometryEntity.GetVertexNormals();
    Eigen::Vector3d vnormal = vertexNormals[0];
    vnormal.normalize();
    
    ReprojectionClass* reprojectionClass = new ReprojectionClass();
    std::vector<MapOFImageAndWorldPoints>reprojectedPoints;
    reprojectedPoints = reprojectionClass->ReprojectImagePixelsTo3DGeometry((_imageSystem.GetCurrentImage()).GetImage2DArrayPixels());
    
    // Image
    int imageWidth = (int)_imageSystem.GetCurrentImage().GetImage2DArrayPixels().width();
    int imageHeight = (int)_imageSystem.GetCurrentImage().GetImage2DArrayPixels().height();
    
    double innerconeangle = 0.00 * M_PI/180; // // with assumption that lightDirection == coneangle
    double outerconeangle = 22.50 * M_PI/180;
    
    double cosOfInnerConeAngle = cos(innerconeangle);
    double cosOfOuterConeAngle = cos(outerconeangle);
    UtilityClass* util = new UtilityClass();
    Rgba* outputPixels = util->GetImagePixelsToWrite(imageWidth, imageHeight);
    
    
    for (int index = 0; index < reprojectedPoints.size(); ++index) {

        pixelIntensityValue =   (_imageSystem.GetCurrentImage().GetImage2DArrayPixels())[reprojectedPoints[index]._imagepixel.y][reprojectedPoints[index]._imagepixel.x];
        double averagePixelIntensityValue   =   (pixelIntensityValue.r + pixelIntensityValue.g + pixelIntensityValue.b)/3;
        
        if (averagePixelIntensityValue <=  0) {
            // light does not reach that pixel;
            continue;
        }
        // convert average pixel intensity to 0 to 255
        averagePixelIntensityValue =  (averagePixelIntensityValue /(pow(2, 16)-1))*255;
        
        Eigen::Vector3d lightToPointDirectionVector =  reprojectedPoints[index]._worldPoint - lightPosition;
        lightToPointDirectionVector.normalize();

        float lambertTerm = (-lightToPointDirectionVector).dot(vnormal);

        if (lambertTerm <= 0 ) {
            // No light gets reflected from the surface at the particular point
            continue;
        }
        
        
        // Use it for attentuation factor
        double distancePointAndLight    =  sqrt(pow(lightPosition.x() - reprojectedPoints[index]._worldPoint.x(),2) +
                                                pow(lightPosition.y() - reprojectedPoints[index]._worldPoint.y(),2) +
                                                pow(lightPosition.z() - reprojectedPoints[index]._worldPoint.z(),2));
        double cosOfCurrAngle                   =   lightDirection.dot(lightToPointDirectionVector);
        //double cosOfInner_minus_OuterConeAngle  =   cosOfInnerConeAngle - cosOfOuterConeAngle;
        
        // Clamp the value of the curr between 0 and 1
        //double IntensityFactorWithoutExponent =  std::min(std::max((double)((cosOfCurrAngle - cosOfOuterConeAngle) / cosOfInner_minus_OuterConeAngle),0.0),1.0);
        double IntensityFactorWithoutExponent = cosOfCurrAngle;
        
        if (cosOfCurrAngle > cosOfOuterConeAngle ) {
           double IntensityFactorWithExponent = averagePixelIntensityValue / (lightIntensity * materialAlbedo * lambertTerm);
            
            double spotLightExponent = log(IntensityFactorWithExponent)/log(IntensityFactorWithoutExponent);
        
        int i = reprojectedPoints[index]._imagepixel.y * imageWidth + reprojectedPoints[index]._imagepixel.x;
        if (spotLightExponent < 0) {
            outputPixels[i].r = 1.0;
        } else {
            outputPixels[i].b = 1.0;
        }
            
            std::cout << reprojectedPoints[index]._imagepixel.y << "," << reprojectedPoints[index]._imagepixel.x << " = " << spotLightExponent << std::endl;
        
        outputfile << reprojectedPoints[index]._imagepixel.y << "," << reprojectedPoints[index]._imagepixel.x << " = " << spotLightExponent << "\n";
        }

    }
    
    _imageSystem.GetCurrentImage().WriteImage2DArrayPixels("output-withcondition.exr", outputPixels, imageWidth, imageHeight);
    outputfile.close();
}

