
//  AttenuationClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 10/11/15.
//  Copyright © 2015 Pratheba Selvaraju. All rights reserved.
//

#include <iostream>
#include <sstream>
#include "AttenuationClass.hpp"
#include "../Utility/ExtLibraryUtil/GraphUtils.h"

AttenuationClass::AttenuationClass(SpotLightParameterEstimationClass& spotLightParamEstClass):_spotLightParamEstClass(spotLightParamEstClass),_circleData(nullptr) {
    
     const CameraEntityClass& camEntity = _spotLightParamEstClass._cameraSystem->GetCurrentCamera();
    const LightEntityClass& lightEntity = _spotLightParamEstClass._lightSystem->GetCurrentLight();
    _input = new ParametersFromSystemClasses();
    _input->camExtrinsicMatrix   = _spotLightParamEstClass._cameraSystem->GetCurrentCameraExtrinsicMatrix();
    _input->focalLength          = camEntity.GetFocalLength();
    _input->principalPoint       = camEntity.GetPrincipalPoint();
    _input->sensorWidth          = tan(camEntity.GetFieldOfView() * 0.5 * M_PI/180) * (camEntity.GetFocalLength()).x * 2;
    _input->sensorHeight         = _input->sensorWidth * _spotLightParamEstClass._imageHeight / _spotLightParamEstClass._imageWidth;
    _input->camTransCoord        = camEntity.GetTransformationCoord();
    
    _input->imageWidth           = (_spotLightParamEstClass._imageWidth);
    _input->imageHeight          = (_spotLightParamEstClass._imageHeight);
    
    _input->fallOffAngle         = lightEntity.GetSpotLightOuterConeAngle() * M_PI/180;
    _input->lightPosition        = lightEntity.GetPosition();
    _input->lightDirection       = -lightEntity.GetDirectionVector(); // since we get -Zvec which is opposite of lookat direction
    _input->cosOfOuterConeAngle  = cos(lightEntity.GetSpotLightOuterConeAngle()* M_PI / 180);
    _input->cosOfInnerConeAngle  = cos(lightEntity.GetSpotLightInnerConeAngle()* M_PI / 180);
    
    _input->vertex               = (_spotLightParamEstClass._geometrySystem->GetCurrentGeometry()).GetAVertex();
    _input->normalOfPlane        = (_spotLightParamEstClass._geometrySystem->GetCurrentGeometry()).GetVertexNormals()[0];
    (_input->normalOfPlane).normalize();
    _pixelIntensityAlongDiameter.clear();
    _attenuationFactorAndDistance.clear();
    _maxIntensity = DBL_MIN;
    _minIntensity = DBL_MAX;
    
    _minXValue = DBL_MAX;
    _maxXValue = DBL_MIN;
    _minYValue = DBL_MAX;
    _maxYValue = DBL_MIN;
}

AttenuationClass::~AttenuationClass() {
    if (_input != nullptr) {
        delete _input;
        _input = nullptr;
    }
    
    if (_circleData != nullptr) {
        delete _circleData;
        _circleData = nullptr;
    }
}

void AttenuationClass::Reinitialize(SpotLightParameterEstimationClass &spotLightParamEstClass) {
    if (_input != nullptr) {
        delete _input;
        _input = nullptr;
    }
    
    if (_circleData != nullptr) {
        delete _circleData;
        _circleData = nullptr;
    }
    
    const CameraEntityClass& camEntity = _spotLightParamEstClass._cameraSystem->GetCurrentCamera();
    const LightEntityClass& lightEntity = _spotLightParamEstClass._lightSystem->GetCurrentLight();
    _input = new ParametersFromSystemClasses();
    _input->camExtrinsicMatrix   = _spotLightParamEstClass._cameraSystem->GetCurrentCameraExtrinsicMatrix();
    _input->focalLength          = camEntity.GetFocalLength();
    _input->principalPoint       = camEntity.GetPrincipalPoint();
    _input->sensorWidth          = tan(camEntity.GetFieldOfView() * 0.5 * M_PI/180) * (camEntity.GetFocalLength()).x * 2;
    _input->sensorHeight         = _input->sensorWidth * _spotLightParamEstClass._imageHeight / _spotLightParamEstClass._imageWidth;
    _input->camTransCoord        = camEntity.GetTransformationCoord();
    
    _input->imageWidth           = (_spotLightParamEstClass._imageWidth);
    _input->imageHeight          = (_spotLightParamEstClass._imageHeight);
    
    _input->fallOffAngle         = lightEntity.GetSpotLightOuterConeAngle() * M_PI/180;
    _input->lightPosition        = lightEntity.GetPosition();
    _input->lightDirection       = -lightEntity.GetDirectionVector();
    _input->cosOfOuterConeAngle  = cos(lightEntity.GetSpotLightOuterConeAngle()* M_PI / 180);
    _input->cosOfInnerConeAngle  = cos(lightEntity.GetSpotLightInnerConeAngle()* M_PI / 180);
    
    _input->vertex               = (_spotLightParamEstClass._geometrySystem->GetCurrentGeometry()).GetAVertex();
    _input->normalOfPlane        = (_spotLightParamEstClass._geometrySystem->GetCurrentGeometry()).GetVertexNormals()[0];
    (_input->normalOfPlane).normalize();
    _pixelIntensityAlongDiameter.clear();
}

void AttenuationClass::SetNumberOfInputFilesForCalculatingAttenuationFactor(int numOfFiles) {
    _attenuationFactorAndDistance = std::vector<std::vector<std::vector<Point2D<double>>>>(numOfFiles);
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
//    std::vector<MapOFImageAndWorldPoints>reprojectedPoints = _input->reprojectedPoints;
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

AttenuationClass::circleMetaData* AttenuationClass::GetCircleMetaData() {
    if (_circleData != nullptr) {
        return _circleData;
    }
    
    _circleData = new circleMetaData();
    
    Eigen::Vector3d pointOnThePlane = Eigen::Vector3d(_input->vertex.x,_input->vertex.y,_input->vertex.z);
    
    // Project the vector from lightposition to pointonplane onto the normal of plane to get the
    // perpendicular distance between the lightsurface and the plane
    double distanceToPlane = (_input->lightPosition - pointOnThePlane).dot(_input->normalOfPlane);
    // Since light's normal direction is opposite to that of normal of the plane if we want to
    // illuminate the plane
    Eigen::Vector3d centerOfProjection = _input->lightPosition + distanceToPlane*(-1)*_input->normalOfPlane;
    
    // Get the 3 Axis with normal of plane as an axis
    Eigen::Vector3d axisZ = _input->normalOfPlane;
    Eigen::Vector3d axizX = (centerOfProjection - pointOnThePlane);
    axizX.normalize();
    Eigen::Vector3d axisY = axisZ.cross(axizX);
    axisY.normalize();
    
    double radiusOfCircle = tan(_input->fallOffAngle)*distanceToPlane;
    
    _circleData->center = centerOfProjection;
    _circleData->radius = radiusOfCircle;
    _circleData->Axis.push_back(axizX);
    _circleData->Axis.push_back(axisY);
    _circleData->Axis.push_back(axisZ);
    
    return _circleData;
}


std::vector<Eigen::Vector3d> AttenuationClass::GetPointsCircle(const int numOfVertices)
{
    _circleData = GetCircleMetaData();
    std::vector<Eigen::Vector3d> vertex = std::vector<Eigen::Vector3d>(numOfVertices);
    
    for (int step=0;step<numOfVertices;++step)
    {
        float angle=step*M_PI*2/numOfVertices;
        vertex[step]= _circleData->center + _circleData->radius*cos(angle)*_circleData->Axis[0] + _circleData->radius*sin(angle)*_circleData->Axis[1];
    }
    return vertex;
}


std::vector<Point2D<double>> AttenuationClass::GetStartPointAndEndPoint() {
    std::vector<Eigen::Vector3d> points = GetPointsCircle(2);
    
    std::vector<Point2D<double>> outputPoints;
    Eigen::Vector4d point;
    Point2D<int> ScreenCoordFromWorldToPixelUnits;
    
    for (int index = 0; index < points.size(); ++index) {
        point << points[index] , 1;
        Eigen::Vector4d worldtoCam = _input->camExtrinsicMatrix * point;
        Point2D<double> camToScreenCoord = Point2D<double>
        (_input->focalLength.x * (double)(worldtoCam[0]/worldtoCam[2]), \
         _input->focalLength.x  *(double)(worldtoCam[1]/worldtoCam[2]));
        
        // convert camToScreenCoord in pixel Units
        
        ScreenCoordFromWorldToPixelUnits.y = camToScreenCoord.y *
        (_spotLightParamEstClass._imageWidth) / (_input->sensorWidth);
        ScreenCoordFromWorldToPixelUnits.x = camToScreenCoord.x *
        (_spotLightParamEstClass._imageHeight) / (_input->sensorHeight);
        
         Point2D<double> screenToPixel  = Point2D<double>(-(ScreenCoordFromWorldToPixelUnits.y - _input->principalPoint.y) ,
                                              (ScreenCoordFromWorldToPixelUnits.x + _input->principalPoint.x ));
        
        std::cout << screenToPixel.y << "::" << screenToPixel.x << std::endl;
        outputPoints.push_back(Point2D<double>(screenToPixel.y, screenToPixel.x));
        
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
    std::vector<Point2D<double>> pixelsOnLine = GetIntensityOfPoints(outputPoints);
    
    WriteToImage(pixelsOnLine);
    //CalculateAttenuationFactor();
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
std::vector<Point2D<double>> AttenuationClass::GetIntensityOfPoints(std::vector<Point2D<double>>& outputPoints) {
    
    std::vector<Point2D<double>> pixelsOnLine;
    
    Point2D<double> start(outputPoints[0]);
    Point2D<double> end(outputPoints[1]);
    
    double deltaX = std::abs(end.x - start.x);
    double deltaY = std::abs(end.y - start.y);
    
    bool isYTobeIncremented = false;
    if (deltaY > deltaX) {
        isYTobeIncremented = true;
        std::swap(start.x, start.y);
        std::swap(end.x, end.y);
    }
    
    if (start.x > end.x) {
        std::swap(start, end);
    }
    
    const int ystep = (start.y < end.y) ? 1 : -1;
    int yindex = start.y;
    double error = deltaX/2.0;

    Rgba intensity;
    double averageIntensity;
    for (int xindex = start.x; xindex <= end.x; ++xindex) {
        if (isYTobeIncremented) {
            pixelsOnLine.push_back(Point2D<double>(xindex, yindex));
            intensity = ((_spotLightParamEstClass._imageSystem)->GetCurrentImage().GetImage2DArrayPixels())[xindex][yindex];
            averageIntensity = (intensity.r + intensity.g + intensity.b)/3;
            //std::cout << "[" << xindex << "," << yindex << "]" << " = " << averageIntensity << std::endl;
            _pixelIntensityAlongDiameter.push_back(MapOfPixelAndIntensity(Point2D<double>(xindex, yindex),averageIntensity));

        }
        else {
            pixelsOnLine.push_back(Point2D<double>(yindex, xindex));
            intensity = ((_spotLightParamEstClass._imageSystem)->GetCurrentImage().GetImage2DArrayPixels())[yindex][xindex];
            averageIntensity = (intensity.r + intensity.g + intensity.b)/3;
            //std::cout << "[" << yindex << "," << xindex << "]" << " = " << averageIntensity << std::endl;
            _pixelIntensityAlongDiameter.push_back(MapOfPixelAndIntensity(Point2D<double>(yindex, xindex),averageIntensity));
        }
        
        if (_maxIntensity < averageIntensity) {
            _maxIntensity = averageIntensity;
        }
        if (_minIntensity > averageIntensity) {
            _minIntensity = averageIntensity;
        }
        
        error = error - deltaY;
        if (error < 0) {
            yindex = yindex + ystep;
            error = error + deltaX;
        }
    }
    
    return pixelsOnLine;
}


void AttenuationClass::GetPointsOnTheLine(Point2D<double>& startPoint, Point2D<double>& endPoint) {
    double deltaX = endPoint.x - startPoint.x;
    double deltaY = endPoint.y - startPoint.y;
    double slope = deltaY / deltaX;
    
    double error = 0;
    int y = startPoint.y;
    
    for(int index = startPoint.x; startPoint.x <= endPoint.x; ++index) {
        error = error + slope;
        while (error >= 0.5) {
            y = y + (endPoint.y - startPoint.y);
            error = error - 1.0;
        }
    }
}


void AttenuationClass::CalculateAttenuationFactor(int fileIndex) {
    
    GetPointsAndIntensityToCalculateAttenuationFactor();
    std::vector<MapOFImageAndWorldPoints> reprojectedPoints = GetReprojectedPixelValues();
    // Reprojected 3D points
   
    double gamma = ((_spotLightParamEstClass._lightSystem)->GetCurrentLight()).GetGammaCorrection();
    double cosOfInner_minus_OuterConeAngle  =   _input->cosOfInnerConeAngle - _input->cosOfOuterConeAngle;
    if(cosOfInner_minus_OuterConeAngle == 0)
        cosOfInner_minus_OuterConeAngle = 0.000001;
    
_attenuationFactorAndDistance[fileIndex] = std::vector<std::vector<Point2D<double>>>((int)reprojectedPoints.size(),std::vector<Point2D<double>>(10));
    for (int index = 0; index < reprojectedPoints.size(); ++index) {
        
        Rgba pixelIntensityValue =(_spotLightParamEstClass._imageSystem->GetCurrentImage().GetImage2DArrayPixels())[reprojectedPoints[index]._imagepixel.y][reprojectedPoints[index]._imagepixel.x];
        double averagePixelIntensityValue   =  (powl(pixelIntensityValue.r,gamma) + powl(pixelIntensityValue.g,gamma) + powl(pixelIntensityValue.b,gamma))/3;
        
        Eigen::Vector3d lightToPointDirectionVector =  reprojectedPoints[index]._worldPoint - _input->lightPosition;
        double distance = std::sqrt(lightToPointDirectionVector.dot(lightToPointDirectionVector));
        lightToPointDirectionVector.normalize();
        float lambertTerm = (-lightToPointDirectionVector).dot(_input->normalOfPlane);
       
        if (lambertTerm <= 0) {
            continue;
        }
        double cosOfCurrAngle                   = _input->lightDirection.dot(lightToPointDirectionVector);
        double IntensityFactorWithoutExponent = cosOfCurrAngle;//(double)((cosOfCurrAngle - _input->cosOfOuterConeAngle) / (cosOfInner_minus_OuterConeAngle));
        
        //std::cout << "Distance = " << distance << " = ";
        for (int alpha=1; alpha <=6; ++alpha) {
            double attenuationFactor = averagePixelIntensityValue / (100 * 0.5 * lambertTerm * pow(IntensityFactorWithoutExponent, alpha));
            //std::cout << attenuationFactor << ",";
            _attenuationFactorAndDistance[fileIndex][index][alpha] = Point2D<double>(attenuationFactor, distance);
            if (_minXValue > distance) {
                _minXValue = distance;
            }
            if (_minYValue > attenuationFactor) {
                _minYValue = attenuationFactor;
            }
            if (_maxYValue < attenuationFactor) {
                _maxYValue = attenuationFactor;
            }
            if (_maxXValue < distance) {
                _maxXValue = distance;
            }
            
        }
        //std::cout << std::endl;
    }
    
    std::cout << _minXValue << "," << _maxXValue << "," << _minYValue << "," << _maxYValue << std::endl;
}

void AttenuationClass::DrawGraph(cv::Mat& outputGraph) {
    
    std::string outputFileName = "finalImage.jpg";
    for (int fileIndex = 0; fileIndex < _attenuationFactorAndDistance.size(); ++fileIndex) {
        std::cout << "Drawing graph for fileIndex = " << fileIndex << std::endl;
        for (int alpha = 1; alpha <=10; ++alpha) {
            _attenFactorVsDistance.clear();
            _attenFactorVsDistance = std::vector<Point2D<double>>((int)_attenuationFactorAndDistance[fileIndex].size());
            for (int index = 0; index < (int)_attenuationFactorAndDistance[fileIndex].size(); ++index) {
                _attenFactorVsDistance[index] = (_attenuationFactorAndDistance[fileIndex][index][alpha]);
            }
            DrawAttenuationFactorVsDistanceFromLight(outputGraph, outputFileName);
        }
    }
}

void AttenuationClass::DrawGraphForAlpha(cv::Mat& outputGraph, int alpha, std::string& outputFileName) {
    for (int fileIndex = 0; fileIndex < _attenuationFactorAndDistance.size(); ++fileIndex) {
        std::cout << "Drawing graph for fileIndex = " << fileIndex << std::endl;
        _attenFactorVsDistance.clear();
        _attenFactorVsDistance = std::vector<Point2D<double>>((int)_attenuationFactorAndDistance[fileIndex].size());
        
        for (int index = 0; index < (int)_attenuationFactorAndDistance[fileIndex].size(); ++index)
            _attenFactorVsDistance[index] = (_attenuationFactorAndDistance[fileIndex][index][alpha]);
           
        DrawAttenuationFactorVsDistanceFromLight(outputGraph, outputFileName);
        
    }
}

std::vector<MapOFImageAndWorldPoints> AttenuationClass::GetReprojectedPixelValues() {
    ReprojectionClass* reprojectionClass = new ReprojectionClass();
    std::vector<MapOFImageAndWorldPoints>reprojectedPoints;
    
    std::vector<Point2D<double>> reprojectPixelsAlongDiameter;
    for (int index =0; index < _pixelIntensityAlongDiameter.size(); ++index)
        reprojectPixelsAlongDiameter.push_back(_pixelIntensityAlongDiameter[index]._imagePixel);
    
    reprojectedPoints = reprojectionClass->ReprojectImagePixelsTo3DGeometry(reprojectPixelsAlongDiameter);
    //TestIfReprojectedPixelsMatchedTheImagePixel(reprojectedPoints);
    
    delete reprojectionClass;
    return reprojectedPoints;
}

void AttenuationClass::DrawAttenuationFactorVsDistanceFromLight(cv::Mat& backgroundImage, std::string& outputFileName) {
    if (_attenFactorVsDistance.empty()) {
        return;
    }
    
    drawGraph(_attenFactorVsDistance, backgroundImage, _minXValue, _maxXValue, _minYValue, _maxYValue, "Distance", "AttenuationFactor");
    
    //showImage(outputGraph, 0, "Rotation Angle");
    cv::imwrite(outputFileName, backgroundImage);
}

void AttenuationClass::DrawPixelVsIntensityScaled(cv::Mat& backgroundImage) {
    GetPointsAndIntensityToCalculateAttenuationFactor();
    if (_pixelIntensityAlongDiameter.empty()) {
        std::cout << "No pixels to calculate attenuation" << std::endl;
        return;
    }
    /*
     1. Get the pixel count
     2. Get the pixel intensity in an array
     3. start graphing
     */
    
    // Convert to scale of 0 to 255
    double diffMaxMin = _maxIntensity - _minIntensity;
    //std::vector<double> scaledPixelIntensity = std::vector<double>(_pixelIntensityAlongDiameter.size(), 0);
    std::vector<Point2D<double>> scaledPixelIntensity = std::vector<Point2D<double>>(_pixelIntensityAlongDiameter.size());
    for (int index = 0; index < _pixelIntensityAlongDiameter.size(); ++index)
        scaledPixelIntensity[index] = Point2D<double>((((_pixelIntensityAlongDiameter[index]._intensityAverage - _minIntensity)/(diffMaxMin))*255), index);
  
    int width = backgroundImage.cols;
    int height = backgroundImage.rows;
    drawGraph(scaledPixelIntensity, DATATYPE::DOUBLE, backgroundImage, (double)0, (double)255, width , height , "Intensity");
    showImage(backgroundImage, 0, "Rotation Angle");
    cv::imwrite("finalImage.jpg", backgroundImage);
}


void AttenuationClass::WriteToImage(std::vector<Point2D<double>>& outputPoints) {
    
    // Test
    static int fileNumber = 1;
    string Result;
    std::ostringstream convert;
    convert << fileNumber;
    Result = convert.str();
    
    ///////
    
    UtilityClass* util = new UtilityClass();
    Rgba* outputPixels = util->GetImagePixelsToWrite(_spotLightParamEstClass._imageWidth, _spotLightParamEstClass._imageHeight);
    
    for(int row = 0; row < _spotLightParamEstClass._imageHeight; ++row) {
        for (int col = 0; col < _spotLightParamEstClass._imageWidth; ++col) {
            Rgba p = (_spotLightParamEstClass._imageSystem->GetCurrentImage().GetImage2DArrayPixels())[row][col];
            int i = row * _input->imageWidth + col;
            outputPixels[i] = p;
        }
    }
    
    for (int index = 0; index < outputPoints.size(); ++index) {
        int i = outputPoints[index].y * _input->imageWidth + outputPoints[index].x;
        outputPixels[i] = Rgba(1,0,0);
    }
    
    std::string outputfileName = "../../Output/circlepoints"+ Result + ".exr";
    util->WriteImage2DArrayPixels(outputfileName, outputPixels, _input->imageWidth, _input->imageHeight);
    delete util;
    delete [] outputPixels;
    fileNumber++;
}


void AttenuationClass::TestIfReprojectedPixelsMatchedTheImagePixel(const std::vector<MapOFImageAndWorldPoints>& reprojectedPoints ) {
    for (int index = 0; index < reprojectedPoints.size(); ++index) {
        Rgba pixelIntensityValue =(_spotLightParamEstClass._imageSystem->GetCurrentImage().GetImage2DArrayPixels())[reprojectedPoints[index]._imagepixel.y][reprojectedPoints[index]._imagepixel.x];
        double averagePixelIntensityValue   =  (pixelIntensityValue.r + pixelIntensityValue.g + pixelIntensityValue.b)/3;
         std::cout << reprojectedPoints[index]._imagepixel.x << "," << reprojectedPoints[index]._imagepixel.y << "=" <<  averagePixelIntensityValue << std::endl;
    }
    std::cout << "****************************************" << std::endl;
    for (int index =0; index < _pixelIntensityAlongDiameter.size(); ++index) {
        std::cout << _pixelIntensityAlongDiameter[index]._imagePixel.x << "," << _pixelIntensityAlongDiameter[index]._imagePixel.y << "=" <<  _pixelIntensityAlongDiameter[index]._intensityAverage << std::endl;
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
        Eigen::Vector4d worldtoCam = _input->camExtrinsicMatrix * point;
        Point2D<double> camToScreenCoord = Point2D<double>(
                                                           _input->focalLength.x*(double)(worldtoCam[0]/worldtoCam[2]),
                                                           _input->focalLength.x*(double)(worldtoCam[1]/worldtoCam[2]));
        
        // convert camToScreenCoord in pixel Units
        Point2D<int> ScreenCoordFromWorldToPixelUnits;
        ScreenCoordFromWorldToPixelUnits.y = camToScreenCoord.y * (_input->imageHeight) / (_input->sensorHeight);
        ScreenCoordFromWorldToPixelUnits.x = camToScreenCoord.x * (_input->imageWidth) / (_input->sensorWidth);
        
        Point2D<double> screenToPixel = Point2D<double>(-(ScreenCoordFromWorldToPixelUnits.y - _input->principalPoint.y) ,(ScreenCoordFromWorldToPixelUnits.x + _input->principalPoint.x ));
        
        outputPoints[index] = Point2D<double>(screenToPixel.y, screenToPixel.x);
    }
}
