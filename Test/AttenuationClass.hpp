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

private:
    struct circleMetaData {
        Eigen::Vector3d center;
        double radius;
        std::vector<Eigen::Vector3d> Axis;
        
        circleMetaData():center(Eigen::Vector3d()), radius(0.0),Axis(std::vector<Eigen::Vector3d>()){}
    };
    
    SpotLightParameterEstimationClass& _spotLightParamEstClass;
    SpotLightParameterEstimationClass::InputForExponentCalculation* input;
    
    void CalculateAttenuationFactor();
    
    double GetErrorOforiginalAndReprojectedPixelValues();
    circleMetaData* GetCircleMetaData();
    std::vector<Eigen::Vector3d> GetPointsCircle(const int numOfVertices);
    void GetPixelCoordFromWorldPoints();
    
  };

#endif /* AttenuationClass_hpp */
