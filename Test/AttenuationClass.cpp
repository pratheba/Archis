////
////  AttenuationClass.cpp
////  Archis
////
////  Created by Pratheba Selvaraju on 10/11/15.
////  Copyright © 2015 Pratheba Selvaraju. All rights reserved.
////
//
//#include "AttenuationClass.hpp"
//
//AttenuationClass::AttenuationClass(SpotLightParameterEstimationClass& spotLightParamEstClass):_spotLightParamEstClass(spotLightParamEstClass) {
//    input =  _spotLightParamEstClass.SpotLightExponentInputParameters();
//
//}
//
//AttenuationClass::~AttenuationClass() {
//}
//
//// Private Functions
//double AttenuationClass::GetErrorOforiginalAndReprojectedPixelValues() {
//    return 0.0;
//}
//
////
////void AttenuationClass::GetOriginalPixelValues() {
////    
////    std::ofstream outputOriginalValue;
////    outputOriginalValue.open("../../Output/outputOriginalValue.txt");
////    
////    int imageWidth  =   _spotLightParamEstClass._imageWidth;
////    int imageHeight =   _spotLightParamEstClass._imageHeight;
////    
////    for(int row = 0; row < imageHeight; ++row) {
////        for (int col = 0; col < imageWidth; ++col) {
////            Rgba p = (_spotLightParamEstClass._imageSystem.GetCurrentImage().GetImage2DArrayPixels())[row][col];
////            outputOriginalValue << "[" << row <<"][" << col << "]" << "=" << p.r << "\t" << p.g << "\t" << p.b << std::endl;
////            //double averagePixelIntensityValue   =   (p.r + p.g + p.b)/3;
////        }
////    }
////     outputOriginalValue.close();
////}
////
////void AttenuationClass::GetReprojectedPixelValues() {
////    
////    std::ofstream outputReprojValue;
////    outputReprojValue.open("../../Output/outputReprojectedValue.txt");
////
////    std::vector<MapOFImageAndWorldPoints>reprojectedPoints = input->reprojectedPoints;
////    for (int index = 0; index < reprojectedPoints.size(); ++index) {
////        Rgba p = (_spotLightParamEstClass._imageSystem.GetCurrentImage().GetImage2DArrayPixels())[reprojectedPoints[index]._imagepixel.y][reprojectedPoints[index]._imagepixel.x];
////        outputReprojValue << "["<<reprojectedPoints[index]._imagepixel.y<<"]["<<reprojectedPoints[index]._imagepixel.x<<"]"<<"="<<p.r << "\t" << p.g << "\t" << p.b << std::endl;
////    }
////    outputReprojValue.close();
////}
////
////double GetFallOffAngle() {
////    return 0.0;
////}
//
//
//// Assumption light direction perpendicular to plane
//// Plane is XY
//void AttenuationClass::CalculateAttenuationFactor() {
//    
//}
//
//AttenuationClass::circleMetaData* AttenuationClass::GetCircleMetaData() {
//    circleMetaData* circleData = new circleMetaData();
//    
//    
//    double fallOffAngle = GetFallOffAngle();
//    Eigen::Vector3d lightPosition = input->lightPosition;
//    Point3D<double> vertex = (_spotLightParamEstClass._geometrySystem.GetCurrentGeometry()).GetAVertex();
//    Eigen::Vector3d pointOnThePlane = Eigen::Vector3d(vertex.x,vertex.y,vertex.z);
//    Eigen::Vector3d normalOfPlane = input->vnormal;
//    normalOfPlane.normalize();
//    
//    // Project the vector from lightposition to pointonplane onto the normal of plane to get the
//    // perpendicular distance between the lightsurface and the plane
//    double distanceToPlane = (lightPosition - pointOnThePlane).dot(normalOfPlane);
//    Eigen::Vector3d centerOfProjection = lightPosition + distanceToPlane*normalOfPlane;
//    
//    // Get the 3 Axis with normal of plane as an axis
//    Eigen::Vector3d axisZ = normalOfPlane;
//    Eigen::Vector3d axizX = (centerOfProjection - pointOnThePlane);
//    axizX.normalize();
//    Eigen::Vector3d axisY = axisZ.cross(axizX);
//    axisY.normalize();
//    
//    double radiusOfCircle = tan(fallOffAngle)*distanceToPlane;
//    
//    circleData->center = centerOfProjection;
//    circleData->radius = radiusOfCircle;
//    circleData->Axis.push_back(axizX);
//    circleData->Axis.push_back(axisY);
//    circleData->Axis.push_back(axisZ);
//    
//    return circleData;
//}
//
//std::vector<Eigen::Vector3d> AttenuationClass::GetPointsCircle(const int numOfVertices)
//{
//    circleMetaData* circleData = GetCircleMetaData();
//    std::vector<Eigen::Vector3d> vertex = std::vector<Eigen::Vector3d>(numOfVertices);
//    
//    for (int step=0;step<numOfVertices;++step)
//    {
//        float angle=step*M_PI*2/numOfVertices;
//        vertex[step]= circleData->center + circleData->radius*cos(angle)*circleData->Axis[0] + circleData->radius*sin(angle)*circleData->Axis[1];
//    }
//    
//    delete circleData;
//    return vertex;
//}
//
//void AttenuationClass::GetPixelCoordFromWorldPoints() {
// 
//    
//    Eigen::MatrixXd camExtrinsicMatrix = _spotLightParamEstClass._cameraSystem.GetCurrentCameraExtrinsicMatrix();
//    Point2D<double> focalLength = (_spotLightParamEstClass._cameraSystem.GetCurrentCamera()).GetFocalLength();
//    Point2D<double> focalLengthinPixel = (_spotLightParamEstClass._cameraSystem.GetCurrentCamera()).GetFocalLengthInPixels();
//    Point2D<double> principalPoint = (_spotLightParamEstClass._cameraSystem.GetCurrentCamera()).GetPrincipalPoint();
//    
//    
//    EntityClass::TransformationCOORD camTransCoord = (_spotLightParamEstClass._cameraSystem.GetCurrentCamera()).GetTransformationCoord();
//   
//    
//    std::vector<Eigen::Vector3d> points = GetPointsCircle(5);
//    
//    for (int index = 0; index < points.size(); ++index) {
//        Eigen::Vector3d worldtoCam = camExtrinsicMatrix * points[index];
//        Point2D<double> camToScreenCoord = Point2D<double>(focalLength.x*(double)(worldtoCam[0]/worldtoCam[2]), focalLength.x*(double)(worldtoCam[1]/worldtoCam[2]));
//        
//        Point2D<double> camToScreenCoordPixel = Point2D<double>(focalLengthinPixel.x*(double)(worldtoCam[0]/worldtoCam[2]), focalLengthinPixel.x*(double)(worldtoCam[1]/worldtoCam[2]));
//        
//        std::cout << camToScreenCoord.y << "::" << camToScreenCoord.x << std::endl;
//        std::cout << camToScreenCoordPixel.y << "::" << camToScreenCoordPixel.y << std::endl;
//        
//        Point2D<double> screenToPixel = Point2D<double>(camToScreenCoord.y - principalPoint.y ,camToScreenCoord.x + principalPoint.x );
//    }
//}
//
//  
//    
//
//
////void AttenuationClass::CalculateAttenuationFactor() {
////    
////    for (int alpha = 1; alpha < 20; ++alpha) {
////        int perpDistance[3] = {3,5,6};
////        
////        double fallOffAngle = GetFallOffAngle();
////        for (int theta=0; theta < fallOffAngle; ++theta) {
////            
////        }
////    }
////    
////}
//
//    /*
//    double cosOfInner_minus_OuterConeAngle  =   input->cosOfInnerConeAngle - input->cosOfOuterConeAngle;
//    if(cosOfInner_minus_OuterConeAngle == 0)
//        cosOfInner_minus_OuterConeAngle = 0.001;
//    double gamma = 1.0/input->gammaCorrection;
//    
//    // Reprojected 3D points
//    std::vector<MapOFImageAndWorldPoints>reprojectedPoints = input->reprojectedPoints;
//    std::vector<Point2D<int>> spotLightCoreExponentVectorPosition;
//    
//    for (int index = 0; index < reprojectedPoints.size(); ++index) {
//        
//        Eigen::Vector3d lightToPointDirectionVector =  reprojectedPoints[index]._worldPoint - input->lightPosition;
//        double distance                             = std::sqrt(lightToPointDirectionVector.dot(lightToPointDirectionVector));
//        lightToPointDirectionVector.normalize();
//        
//        float lambertTerm                       = (-lightToPointDirectionVector).dot(input->vnormal);
//        double cosOfCurrAngle                   =   input->lightDirection.dot(lightToPointDirectionVector);
//        double IntensityFactorExponent          =  std::pow((double)((cosOfCurrAngle - input->cosOfOuterConeAngle) / (cosOfInner_minus_OuterConeAngle)), 1);
//        
//        Rgba pixelValue;
//       
//        if (cosOfCurrAngle >= input->cosOfOuterConeAngle)
//        {
//            //double attenuation = 1.0 / (1.0 + 1* distance + 1 * pow(distance, 2));
//            double attenuation = 1.0 / (distance*distance);
//            if (lambertTerm > 0) {
//                double v = (lambertTerm * input->lightIntensity * input->materialAlbedo * IntensityFactorExponent * attenuation);
//                pixelValue = Rgba(pow(v,gamma),pow(v,gamma),pow(v,gamma),1.0);
//            }
//        }
//        else
//            pixelValue = Rgba(0,0,0,1);
//        
//        int i = reprojectedPoints[index]._imagepixel.y * input->imageWidth + reprojectedPoints[index]._imagepixel.x;
//        outputPixelValue << reprojectedPoints[index]._imagepixel.y << "," << reprojectedPoints[index]._imagepixel.x << " = " << pixelValue.r << std::endl;
//        outputPixels[i] = pixelValue;
//}
//
//void AttenuationClass::CalculateAttenuationFactor() {
//        
//        UtilityClass* util = new UtilityClass();
//        Rgba* outputPixels = util->GetImagePixelsToWrite(_imageWidth, _imageHeight);
//        
//        std::ofstream outputPixelValue;
//        outputPixelValue.open("../../Output/outputPixel-blend1.txt");
//        
//        std::ofstream outputOriginalPixelValue;
//        outputOriginalPixelValue.open("../../Output/originalPixel-blend1.txt");
//        
//        for(int row = 0; row < _imageHeight; ++row) {
//            for (int col = 0; col < _imageWidth; ++col) {
//                Rgba p = (_imageSystem.GetCurrentImage().GetImage2DArrayPixels())[row][col];
//                double averagePixelIntensityValue   =   (p.r + p.g + p.b)/3;
//                
//                outputOriginalPixelValue << row << "," << col << " = " << averagePixelIntensityValue << std::endl;
//            }
//        }
//        
//        InputForExponentCalculation* input =  SpotLightExponentInputParameters();
//        std::vector<MapOFImageAndWorldPoints>reprojectedPoints = input->reprojectedPoints;
//        double cosOfInner_minus_OuterConeAngle  =   input->cosOfInnerConeAngle - input->cosOfOuterConeAngle;
//        if(cosOfInner_minus_OuterConeAngle == 0)
//            cosOfInner_minus_OuterConeAngle = EPSILON;
//        double gamma = 1.0/input->gammaCorrection;
//        
//        // Reprojected 3D points
//        std::vector<Point2D<int>> spotLightCoreExponentVectorPosition;
//        for (int index = 0; index < reprojectedPoints.size(); ++index) {
//            
//            Eigen::Vector3d lightToPointDirectionVector =  reprojectedPoints[index]._worldPoint - input->lightPosition;
//            double distance = std::sqrt(lightToPointDirectionVector.dot(lightToPointDirectionVector));
//            lightToPointDirectionVector.normalize();
//            
//            float lambertTerm = (-lightToPointDirectionVector).dot(input->vnormal);
//            // No light gets reflected from the surface at the particular point
//            
//            // Use it for attentuation factor
//            double cosOfCurrAngle                   =   input->lightDirection.dot(lightToPointDirectionVector);
//            double IntensityFactorExponent =  std::pow((double)((cosOfCurrAngle - input->cosOfOuterConeAngle) / (cosOfInner_minus_OuterConeAngle)), 1);
//            
//            Rgba pixelValue;
//            //        if(cosOfCurrAngle >= input->cosOfInnerConeAngle)
//            //        {
//            //            double v = lambertTerm * input->lightIntensity * input->materialAlbedo * attenuation;
//            //            pixelValue = Rgba(pow(v,gamma),pow(v,gamma),pow(v,gamma),1.0);
//            //
//            //        }
//            if (cosOfCurrAngle > input->cosOfOuterConeAngle)
//            {
//                //double attenuation = 1.0 / (1.0 + 1* distance + 1 * pow(distance, 2));
//                double attenuation = 1.0 / (distance*distance);
//                if (lambertTerm > 0) {
//                    double v = (lambertTerm * input->lightIntensity * input->materialAlbedo * IntensityFactorExponent * attenuation);
//                    pixelValue = Rgba(pow(v,gamma),pow(v,gamma),pow(v,gamma),1.0);
//                }
//            }
//            else
//                pixelValue = Rgba(0,0,0,1);
//            
//            int i = reprojectedPoints[index]._imagepixel.y * input->imageWidth + reprojectedPoints[index]._imagepixel.x;
//            outputPixelValue << reprojectedPoints[index]._imagepixel.y << "," << reprojectedPoints[index]._imagepixel.x << " = " << pixelValue.r << std::endl;
//            outputPixels[i] = pixelValue;
//        }
//        
//        WriteToImage(outputPixels, "imagepixel-blend1.exr");
//        outputPixelValue.close();
//        outputOriginalPixelValue.close();
//        delete [] outputPixels;
//        delete input;
//}*/