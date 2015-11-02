//
//  AttenuationClass.hpp
//  Archis
//
//  Created by Pratheba Selvaraju on 10/11/15.
//  Copyright © 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef AttenuationClass_hpp
#define AttenuationClass_hpp

#include <iostream>
#include <vector>
#include "../LightParameters/SpotLightParameterEstimationClass.h"
#include <opencv2/highgui/highgui.hpp>

class AttenuationClass {
public:
    AttenuationClass(SpotLightParameterEstimationClass& spotLightParamEstClass);
    ~AttenuationClass();
    
    void GetOriginalPixelValues();
    
   
    void GetPixelCoordFromWorldPoints();
    void CalculateAttenuationFactor(cv::Mat& backgroundImage);

private:
    struct circleMetaData {
        Eigen::Vector3d center;
        double radius;
        std::vector<Eigen::Vector3d> Axis;
        
        circleMetaData():center(Eigen::Vector3d()), radius(0.0),Axis(std::vector<Eigen::Vector3d>()){}
    };
    
    struct ParametersFromSystemClasses {
        Eigen::MatrixXd camExtrinsicMatrix;
        Point2D<double> focalLength ;
        Point2D<double> principalPoint;
        double sensorWidth;
        double sensorHeight;
        EntityClass::TransformationCOORD camTransCoord ;
        int imageWidth;
        int imageHeight;
        Eigen::Vector3d lightPosition;
        Eigen::Vector3d lightDirection;
        Eigen::Vector3d normalOfPlane;
        double cosOfOuterConeAngle;
        double cosOfInnerConeAngle;
        Point3D<double> vertex;
        double fallOffAngle;
    };
    
    SpotLightParameterEstimationClass&  _spotLightParamEstClass;
    ParametersFromSystemClasses*        _input;
    circleMetaData*                     _circleData;
    std::vector<MapOfPixelAndIntensity> _pixelIntensityAlongDiameter;
    double                              _maxIntensity, _minIntensity;
    
    void GetPointsAndIntensityToCalculateAttenuationFactor();
    
    double GetErrorOforiginalAndReprojectedPixelValues();
    circleMetaData* GetCircleMetaData();
    std::vector<Eigen::Vector3d> GetPointsCircle(const int numOfVertices);
    //void GetPixelCoordFromWorldPoints();
    std::vector<Point2D<double>> GetStartPointAndEndPoint();
    std::vector<Point2D<double>> GetIntensityOfPoints(std::vector<Point2D<double>>& outputPoints);
    void GetPointsOnTheLine(Point2D<double>& startPoint, Point2D<double>& endPoint);
    std::vector<MapOFImageAndWorldPoints> GetReprojectedPixelValues();
    
    
    
    // Testing
    void WriteToImage(std::vector<Point2D<double>>& outputPoints);
    void TestIfReprojectedPixelsMatchedTheImagePixel(const std::vector<MapOFImageAndWorldPoints>& reprojectedPoints);
    void DrawPixelVsIntensityScaled(cv::Mat& backgroundImage);
  };

#endif /* AttenuationClass_hpp */
