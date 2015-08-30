//
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
    
    CalculateExponentParameter();
}


// Refactor 
void SpotLightParameterEstimationClass::CalculateExponentParameter() {
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
    
    ReprojectionClass* reprojectionClass = new ReprojectionClass();
    std::vector<MapOFImageAndWorldPoints>reprojectedPoints = reprojectionClass->ReprojectImagePixelsTo3DGeometry((_imageSystem.GetCurrentImage()).GetImage2DArrayPixels());
    
    for (int index = 0; index < reprojectedPoints.size(); ++index) {
        Eigen::Vector3d directionVector =  reprojectedPoints[index]._worldPoint - lightPosition;
        directionVector.normalize();
        
        double cosTheta =  lightDirection.dot(directionVector);
        std::cout << reprojectedPoints[index]._imagepixel.x << "::" << reprojectedPoints[index]._imagepixel.y << std::endl;
        
        Rgba pixelValue = (_imageSystem.GetCurrentImage().GetImage2DArrayPixels())[reprojectedPoints[index]._imagepixel.x][reprojectedPoints[index]._imagepixel.y];
        
        double averagePixelValue = (pixelValue.r + pixelValue.g + pixelValue.b)/3;
        double pixelIntensity = std::abs(averagePixelValue/(vnormal.dot(-directionVector)* lightIntensity * materialAlbedo));
        if (pixelIntensity > 0) {
            
            double alpha =  log(pixelIntensity)/log(cosTheta);
            //double alphaInDegree = alpha * 180 / M_PI;
            //if (alpha > 0) {
              //  outputImage.at<cv::Vec3b>(reprojectedPoints[index].pixel) = cv::Vec3b(0,0,alpha);
           // }
            
            std::cout << "alpha for " << (reprojectedPoints[index]._worldPoint).x() << " ::" << reprojectedPoints[index]._imagepixel.x << " = " << alpha << std::endl;;//<< " :: " << alphaInDegree << std::endl;
            
           // outputfile << reprojectedPoints[index].pixel << " :: " << alpha << std::endl;
            
        }
    }
}

