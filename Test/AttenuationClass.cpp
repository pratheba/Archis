//
//  AttenuationClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 10/11/15.
//  Copyright © 2015 Pratheba Selvaraju. All rights reserved.
//

#include "AttenuationClass.hpp"

AttenuationClass::AttenuationClass(SpotLightParameterEstimationClass& spotLightParamEstClass):_spotLightParamEstClass(spotLightParamEstClass),circleData(nullptr) {
    //input =  _spotLightParamEstClass.GetSpotLightExponentInputParameters();
    
    input = new ParametersFromSystemClasses();
    input->camExtrinsicMatrix   = _spotLightParamEstClass._cameraSystem->GetCurrentCameraExtrinsicMatrix();
    input->focalLength          = (_spotLightParamEstClass._cameraSystem->GetCurrentCamera()).GetFocalLength();
    input->principalPoint       = (_spotLightParamEstClass._cameraSystem->GetCurrentCamera()).GetPrincipalPoint();
    input->sensorWidth          = tan((_spotLightParamEstClass._cameraSystem->GetCurrentCamera()).GetFieldOfView() \
                                      * 0.5 * M_PI/180) * ((_spotLightParamEstClass._cameraSystem->GetCurrentCamera()) \
                                                           .GetFocalLength()).x * 2;
    input->sensorHeight         = input->sensorWidth * _spotLightParamEstClass._imageHeight / _spotLightParamEstClass._imageWidth;
    input->camTransCoord        = (_spotLightParamEstClass._cameraSystem->GetCurrentCamera()).GetTransformationCoord();
    
    input->imageWidth           = (_spotLightParamEstClass._imageWidth);
    input->imageHeight          = (_spotLightParamEstClass._imageHeight);
    input->fallOffAngle         = (_spotLightParamEstClass._lightSystem->GetCurrentLight()).GetSpotLightOuterConeAngle() * M_PI/
                                    180;
    input->lightPosition        = (_spotLightParamEstClass._lightSystem->GetCurrentLight()).GetPosition();
    input->vertex               = (_spotLightParamEstClass._geometrySystem->GetCurrentGeometry()).GetAVertex();
    input->normalOfPlane        = (_spotLightParamEstClass._geometrySystem->GetCurrentGeometry()).GetVertexNormals()[0];
    (input->normalOfPlane).normalize();
}

AttenuationClass::~AttenuationClass() {
    if (input != nullptr) {
        delete input;
        input = nullptr;
    }
    
    if (circleData != nullptr) {
        delete circleData;
        circleData = nullptr;
    }
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
    if (circleData != nullptr) {
        return circleData;
    }
    
    circleData = new circleMetaData();
    
    Eigen::Vector3d pointOnThePlane = Eigen::Vector3d(input->vertex.x,input->vertex.y,input->vertex.z);
    
    // Project the vector from lightposition to pointonplane onto the normal of plane to get the
    // perpendicular distance between the lightsurface and the plane
    double distanceToPlane = (input->lightPosition - pointOnThePlane).dot(input->normalOfPlane);
    // Since light's normal direction is opposite to that of normal of the plane if we want to
    // illuminate the plane
    Eigen::Vector3d centerOfProjection = input->lightPosition + distanceToPlane*(-1)*input->normalOfPlane;
    
    // Get the 3 Axis with normal of plane as an axis
    Eigen::Vector3d axisZ = input->normalOfPlane;
    Eigen::Vector3d axizX = (centerOfProjection - pointOnThePlane);
    axizX.normalize();
    Eigen::Vector3d axisY = axisZ.cross(axizX);
    axisY.normalize();
    
    double radiusOfCircle = tan(input->fallOffAngle)*distanceToPlane;
    
    circleData->center = centerOfProjection;
    circleData->radius = radiusOfCircle;
    circleData->Axis.push_back(axizX);
    circleData->Axis.push_back(axisY);
    circleData->Axis.push_back(axisZ);
    
    return circleData;
}

std::vector<Eigen::Vector3d> AttenuationClass::GetPointsCircle(const int numOfVertices)
{
    circleData = GetCircleMetaData();
    std::vector<Eigen::Vector3d> vertex = std::vector<Eigen::Vector3d>(numOfVertices);
    
    for (int step=0;step<numOfVertices;++step)
    {
        float angle=step*M_PI*2/numOfVertices;
        vertex[step]= circleData->center + circleData->radius*cos(angle)*circleData->Axis[0] + circleData->radius*sin(angle)*circleData->Axis[1];
    }
    return vertex;
}

std::vector<Point2D<double>> AttenuationClass::GetStartPointAndEndPoint() {
    std::vector<Eigen::Vector3d> points = GetPointsCircle(2);
    
    std::vector<Point2D<double>> outputPoints(2);
    Eigen::Vector4d point;
    Point2D<int> ScreenCoordFromWorldToPixelUnits;
    
    for (int index = 0; index < points.size(); ++index) {
        point << points[index] , 1;
        Eigen::Vector4d worldtoCam = input->camExtrinsicMatrix * point;
        Point2D<double> camToScreenCoord = Point2D<double>
        (input->focalLength.x * (double)(worldtoCam[0]/worldtoCam[2]), \
         input->focalLength.x  *(double)(worldtoCam[1]/worldtoCam[2]));
        
        // convert camToScreenCoord in pixel Units
        
        ScreenCoordFromWorldToPixelUnits.y = camToScreenCoord.y *
        (_spotLightParamEstClass._imageWidth) / (input->sensorWidth);
        ScreenCoordFromWorldToPixelUnits.x = camToScreenCoord.x *
        (_spotLightParamEstClass._imageHeight) / (input->sensorHeight);
        
         Point2D<double> screenToPixel  = Point2D<double>(-(ScreenCoordFromWorldToPixelUnits.y - input->principalPoint.y) ,
                                              (ScreenCoordFromWorldToPixelUnits.x + input->principalPoint.x ));
        
        std::cout << screenToPixel.y << "::" << screenToPixel.x << std::endl;
        
    }
    
    return outputPoints;
}

void AttenuationClass::GetPointsAndIntensityToCalculateAttenuationFactor() {
    /*
     1. Get the starting point in pixels
     2. Get the radius of the circle
     3. Convert it into pixel units
     4. Move from starting to radius and duplicate the other end
     5. [ Check if it passes through the center of the center of projection
     */
    
    // Getting the 2 points means we will get it on opposite ends
    
    std::vector<Point2D<double>> outputPoints = GetStartPointAndEndPoint();
    
    double radius = circleData->radius;
    // convert to pixel units
    // check if the returned points are
    
}

// Bresenham's method
/*******************************************************/
//  AbsValue  |  ActualValue  | X and Y
//------------------------------------------------------
//    > 1     |    > 1        | (x0 > x1 && y0 > y1)
//   (inc Y)  |               | (x0 < x1 && y0 < y1)
//------------------------------------------------------
//            |    < 0        | (x0 > x1 && y0 < y1)
//            |               | (x0 < x1 && y0 > y1)
//------------------------------------------------------
//    < 1     |    < 1        | (x0 < x1 && y0 < y1)
//   (inc X)  |               | (x0 > x1 && y0 > y1)
//------------------------------------------------------
//            |    < 0        | (x0 > x1 && y0 < y1)
//            |               | (x0 < x1 && y0 > y1)
/*******************************************************/
// We deal with images and start (0,0) is from top left.
void AttenuationClass::GetIntensityOfPoints(std::vector<Point2D<double>>& outputPoints) {
    double deltaX = (outputPoints[1].x - outputPoints[0].x);
    double deltaY = (outputPoints[1].y - outputPoints[0].y);

    double slope = 0;
    if (deltaX > 0) {
        slope = deltaY / deltaX;
    }
  
    if (slope > 0) {
        if (std::abs(slope) <= 1) { // increment X
            if (outputPoints[0].x > outputPoints[1].x)
                std::swap(outputPoints[0], outputPoints[1]);
            
            // start with increment along X
        }
        else if(std::abs(slope) > 1) { // increment Y
            if (outputPoints[0].y > outputPoints[1].y)
                std::swap(outputPoints[0], outputPoints[1]);
            
            // start with increment along Y
        }
    }
    else if(slope < 0) {
        if (std::abs(slope) <= 1) { // increment X
            if (outputPoints[0].x > outputPoints[1].x)
                std::swap(outputPoints[0], outputPoints[1]);
            
            // start with increment along X
        }
        else if(std::abs(slope) > 1) { // increment Y
            if (outputPoints[0].y > outputPoints[1].y)
                std::swap(outputPoints[0], outputPoints[1]);
            
            // start with increment along Y
        }
        
    }
    
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
    
    std::vector<Eigen::Vector3d> points = GetPointsCircle(60);
    Eigen::Vector4d point;
    std::vector<Point2D<double>> outputPoints(60, Point2D<double>());
    for (int index = 0; index < points.size(); ++index) {
        point << points[index] , 1;
        Eigen::Vector4d worldtoCam = input->camExtrinsicMatrix * point;
        Point2D<double> camToScreenCoord = Point2D<double>(
                                                           input->focalLength.x*(double)(worldtoCam[0]/worldtoCam[2]),
                                                           input->focalLength.x*(double)(worldtoCam[1]/worldtoCam[2]));
        
        // convert camToScreenCoord in pixel Units
        Point2D<int> ScreenCoordFromWorldToPixelUnits;
        ScreenCoordFromWorldToPixelUnits.y = camToScreenCoord.y * (input->imageHeight) / (input->sensorHeight);
        ScreenCoordFromWorldToPixelUnits.x = camToScreenCoord.x * (input->imageWidth) / (input->sensorWidth);
        
        Point2D<double> screenToPixel = Point2D<double>(-(ScreenCoordFromWorldToPixelUnits.y - input->principalPoint.y) ,(ScreenCoordFromWorldToPixelUnits.x + input->principalPoint.x ));
         
        outputPoints[index] = Point2D<double>(screenToPixel.y, screenToPixel.x);
    }
    WriteToImage(outputPoints);
}

void AttenuationClass::WriteToImage(std::vector<Point2D<double>>& outputPoints) {
    UtilityClass* util = new UtilityClass();
    Rgba* outputPixels = util->GetImagePixelsToWrite(_spotLightParamEstClass._imageWidth, _spotLightParamEstClass._imageHeight);
    
    for(int row = 0; row < _spotLightParamEstClass._imageHeight; ++row) {
        for (int col = 0; col < _spotLightParamEstClass._imageWidth; ++col) {
            Rgba p = (_spotLightParamEstClass._imageSystem->GetCurrentImage().GetImage2DArrayPixels())[row][col];
            int i = row * input->imageWidth + col;
            outputPixels[i] = p;
        }
    }
    
    for (int index = 0; index < outputPoints.size(); ++index) {
        int i = outputPoints[index].y * input->imageWidth + outputPoints[index].x;
        outputPixels[i] = Rgba(1,0,0);
    }
    
    std::string outputfileName = "../../Output/circlepoints.exr";
    util->WriteImage2DArrayPixels(outputfileName, outputPixels, input->imageWidth, input->imageHeight);
    delete util;
    delete [] outputPixels;
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

