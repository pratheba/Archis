//
//  AttenuationClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 10/11/15.
//  Copyright © 2015 Pratheba Selvaraju. All rights reserved.
//

#include "AttenuationClass.hpp"

AttenuationClass::AttenuationClass(SpotLightParameterEstimationClass& spotLightParamEstClass):_spotLightParamEstClass(spotLightParamEstClass) {
    input =  _spotLightParamEstClass.GetSpotLightExponentInputParameters();
}

AttenuationClass::~AttenuationClass() {
}

// Private Functions
double AttenuationClass::GetErrorOforiginalAndReprojectedPixelValues() {
    return 0.0;
}

//
//void AttenuationClass::GetOriginalPixelValues() {
//    
//    std::ofstream outputOriginalValue;
//    outputOriginalValue.open("../../Output/outputOriginalValue.txt");
//    
//    int imageWidth  =   _spotLightParamEstClass._imageWidth;
//    int imageHeight =   _spotLightParamEstClass._imageHeight;
//    
//    for(int row = 0; row < imageHeight; ++row) {
//        for (int col = 0; col < imageWidth; ++col) {
//            Rgba p = (_spotLightParamEstClass._imageSystem.GetCurrentImage().GetImage2DArrayPixels())[row][col];
//            outputOriginalValue << "[" << row <<"][" << col << "]" << "=" << p.r << "\t" << p.g << "\t" << p.b << std::endl;
//            //double averagePixelIntensityValue   =   (p.r + p.g + p.b)/3;
//        }
//    }
//     outputOriginalValue.close();
//}
//
//void AttenuationClass::GetReprojectedPixelValues() {
//    
//    std::ofstream outputReprojValue;
//    outputReprojValue.open("../../Output/outputReprojectedValue.txt");
//
//    std::vector<MapOFImageAndWorldPoints>reprojectedPoints = input->reprojectedPoints;
//    for (int index = 0; index < reprojectedPoints.size(); ++index) {
//        Rgba p = (_spotLightParamEstClass._imageSystem.GetCurrentImage().GetImage2DArrayPixels())[reprojectedPoints[index]._imagepixel.y][reprojectedPoints[index]._imagepixel.x];
//        outputReprojValue << "["<<reprojectedPoints[index]._imagepixel.y<<"]["<<reprojectedPoints[index]._imagepixel.x<<"]"<<"="<<p.r << "\t" << p.g << "\t" << p.b << std::endl;
//    }
//    outputReprojValue.close();
//}
//
//double GetFallOffAngle() {
//    return 0.0;
//}


// Assumption light direction perpendicular to plane
// Plane is XY
void AttenuationClass::CalculateAttenuationFactor() {
    
}

AttenuationClass::circleMetaData* AttenuationClass::GetCircleMetaData() {
    circleMetaData* circleData = new circleMetaData();
    
    // Proxy variable
    double fallOffAngle = 30 * M_PI/180;//GetFallOffAngle();
    Eigen::Vector3d lightPosition = input->lightPosition;
    Point3D<double> vertex = (_spotLightParamEstClass._geometrySystem->GetCurrentGeometry()).GetAVertex();
    Eigen::Vector3d pointOnThePlane = Eigen::Vector3d(vertex.x,vertex.y,vertex.z);
    Eigen::Vector3d normalOfPlane = input->vnormal;
    normalOfPlane.normalize();
    
    // Project the vector from lightposition to pointonplane onto the normal of plane to get the
    // perpendicular distance between the lightsurface and the plane
    double distanceToPlane = (lightPosition - pointOnThePlane).dot(normalOfPlane);
    // Since light's normal direction is opposite to that of normal of the plane if we want to
    // illuminate the plane
    Eigen::Vector3d centerOfProjection = lightPosition + distanceToPlane*(-1)*normalOfPlane;
    
    // Get the 3 Axis with normal of plane as an axis
    Eigen::Vector3d axisZ = normalOfPlane;
    Eigen::Vector3d axizX = (centerOfProjection - pointOnThePlane);
    axizX.normalize();
    Eigen::Vector3d axisY = axisZ.cross(axizX);
    axisY.normalize();
    
    double radiusOfCircle = tan(fallOffAngle)*distanceToPlane;
    
    circleData->center = centerOfProjection;
    circleData->radius = radiusOfCircle;
    circleData->Axis.push_back(axizX);
    circleData->Axis.push_back(axisY);
    circleData->Axis.push_back(axisZ);
    
    return circleData;
}

std::vector<Eigen::Vector3d> AttenuationClass::GetPointsCircle(const int numOfVertices)
{
    circleMetaData* circleData = GetCircleMetaData();
    std::vector<Eigen::Vector3d> vertex = std::vector<Eigen::Vector3d>(numOfVertices);
    
    for (int step=0;step<numOfVertices;++step)
    {
        float angle=step*M_PI*2/numOfVertices;
        vertex[step]= circleData->center + circleData->radius*cos(angle)*circleData->Axis[0] + circleData->radius*sin(angle)*circleData->Axis[1];
    }
    
    delete circleData;
    return vertex;
}

void AttenuationClass::GetPixelCoordFromWorldPoints() {
    
    /*
     1. First get the extrinsic matrix
     2. Multiply the circle points by the extrinsic matrix to get the points in camera coordinate system
     3. Divide the X and Y by Z and multiple by focal lenth - all in world units
     4. Now we have U = X*f/Z and V = Y*f/Z which is in the screen coordinate system
     5. Now convert this into pixel coordinate by 
     
     Get the 00 of the pixel coordinate and then + U*(uVec) + V*(-vVec) gives the pixel point
     
     */
 
    UtilityClass* util = new UtilityClass();
    Rgba* outputPixels = util->GetImagePixelsToWrite(_spotLightParamEstClass._imageWidth, _spotLightParamEstClass._imageHeight);
    
    for(int row = 0; row < _spotLightParamEstClass._imageHeight; ++row) {
        for (int col = 0; col < _spotLightParamEstClass._imageWidth; ++col) {
            Rgba p = (_spotLightParamEstClass._imageSystem->GetCurrentImage().GetImage2DArrayPixels())[row][col];
            int i = row * input->imageWidth + col;
            outputPixels[i] = p;
        }
    }
    
   
    
    
    Eigen::MatrixXd camExtrinsicMatrix = _spotLightParamEstClass._cameraSystem->GetCurrentCameraExtrinsicMatrix();
    Point2D<double> focalLength =  (_spotLightParamEstClass._cameraSystem->GetCurrentCamera()).GetFocalLength();
    Point2D<double> principalPoint = (_spotLightParamEstClass._cameraSystem->GetCurrentCamera()).GetPrincipalPoint();
    
    EntityClass::TransformationCOORD camTransCoord = (_spotLightParamEstClass._cameraSystem->GetCurrentCamera()).GetTransformationCoord();
    
    std::vector<Eigen::Vector3d> points = GetPointsCircle(60);
    Eigen::Vector4d point;
    for (int index = 0; index < points.size(); ++index) {
        point << points[index] , 1;
        Eigen::Vector4d worldtoCam = camExtrinsicMatrix * point;
        Point2D<double> camToScreenCoord = Point2D<double>(focalLength.x*(double)(worldtoCam[0]/worldtoCam[2]), focalLength.x*(double)(worldtoCam[1]/worldtoCam[2]));
        
        // convert camToScreenCoord in pixel Units
        Point2D<int> ScreenCoordFromWorldToPixelUnits;
        double sensorWidth = tan((_spotLightParamEstClass._cameraSystem->GetCurrentCamera()).GetFieldOfView() * 0.5 * M_PI/180) * ((_spotLightParamEstClass._cameraSystem->GetCurrentCamera()).GetFocalLength()).x * 2;
        double sensorHeight = sensorWidth * _spotLightParamEstClass._imageHeight / _spotLightParamEstClass._imageWidth;
        
        ScreenCoordFromWorldToPixelUnits.y = camToScreenCoord.y * (_spotLightParamEstClass._imageHeight) / (sensorHeight);
        ScreenCoordFromWorldToPixelUnits.x = camToScreenCoord.x * (_spotLightParamEstClass._imageWidth) / (sensorWidth);
        
        std::cout << "cam to screen = " << ScreenCoordFromWorldToPixelUnits.y << "::" << ScreenCoordFromWorldToPixelUnits.x << std::endl;
        
        Point2D<double> screenToPixel = Point2D<double>(-(ScreenCoordFromWorldToPixelUnits.y - principalPoint.y) ,(ScreenCoordFromWorldToPixelUnits.x + principalPoint.x ));
        
        std::cout << "screen to pixel = " << screenToPixel.y << "::" << screenToPixel.x << std::endl;
        
        int i = screenToPixel.y * input->imageWidth + screenToPixel.x;
        outputPixels[i] = Rgba(1,0,0);
        
    }
    
   
    std::string outputfileName = "../../Output/ciclepoints.exr";
    util->WriteImage2DArrayPixels(outputfileName, outputPixels, input->imageWidth, input->imageHeight);
    delete util;
}

//void AttenuationClass::CalculateAttenuationFactor() {
//    
//    for (int alpha = 1; alpha < 20; ++alpha) {
//        int perpDistance[3] = {3,5,6};
//        
//        double fallOffAngle = GetFallOffAngle();
//        for (int theta=0; theta < fallOffAngle; ++theta) {
//            
//        }
//    }
//    
//}

