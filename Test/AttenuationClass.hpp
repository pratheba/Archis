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

class AttenuationClass {
public:
    AttenuationClass(SpotLightParameterEstimationClass& spotLightParamEstClass);
    ~AttenuationClass();
    
    void GetOriginalPixelValues();
    void GetReprojectedPixelValues();
    void GetPointsAndIntensityToCalculateAttenuationFactor();
    void GetPixelCoordFromWorldPoints();

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
        Eigen::Vector3d normalOfPlane;
        Point3D<double> vertex;
        double fallOffAngle;
    };
    
    SpotLightParameterEstimationClass& _spotLightParamEstClass;
    ParametersFromSystemClasses* input;
    circleMetaData* circleData;
    //SpotLightParameterEstimationClass::InputForExponentCalculation* input;
    
    void CalculateAttenuationFactor();
    
    double GetErrorOforiginalAndReprojectedPixelValues();
    circleMetaData* GetCircleMetaData();
    std::vector<Eigen::Vector3d> GetPointsCircle(const int numOfVertices);
    //void GetPixelCoordFromWorldPoints();
    std::vector<Point2D<double>> GetStartPointAndEndPoint();
    std::vector<Point2D<double>> GetIntensityOfPoints(std::vector<Point2D<double>>& outputPoints);
    void GetPointsOnTheLine(Point2D<double>& startPoint, Point2D<double>& endPoint);
    
    // Testing
    void WriteToImage(std::vector<Point2D<double>>& outputPoints);
    
  };

#endif /* AttenuationClass_hpp */
