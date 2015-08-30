//
//  UtilityClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 8/23/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "UtilityClass.h"

double UtilityClass::GetAveragePixelIntensityAroundaPoint(const Array2D<Rgba>& inputImage, const Point2D<int>& corePoint, const int& windowSize) {
    double pixelIntensity = 0.0;
    VecOf2dIntPoints pixelsAroundCorePoint = GetWindowAroundPoint(inputImage, corePoint, windowSize);
    
    if (pixelsAroundCorePoint.size() > 0) {
        for (int index = 0; index < pixelsAroundCorePoint.size(); ++index) {
            Rgba value = inputImage[pixelsAroundCorePoint[index].y][pixelsAroundCorePoint[index].x];
            pixelIntensity += ((value.r + value.g + value.b)/3);
        }
    }
    Rgba Value = inputImage[corePoint.y][corePoint.x];
    pixelIntensity += ((Value.r + Value.g + Value.b)/3);
    pixelIntensity /= (windowSize * windowSize);
    
    return pixelIntensity;
}

VecOf2dIntPoints UtilityClass::GetWindowAroundPoint(const Array2D<Rgba>& inputImage, const Point2D<int>& corePoint, const int& windowSize) {
    int windowRadius = int(windowSize/2);
    double imageWidth = inputImage.width();
    double imageHeight = inputImage.height();
    VecOf2dIntPoints pixelsAroundCorePoint;
    pixelsAroundCorePoint.clear();
    
    for(int row = -windowRadius; row <= windowRadius; ++row) {
        for (int col = -windowRadius; col <= windowRadius; ++col) {
            
            Point2D<int> currPoint = Point2D<int>(corePoint.y + row, corePoint.x + col);
            if ((currPoint.y < 0) || (currPoint.y > imageWidth)
                || (currPoint.x < 0) || (currPoint.x > imageHeight)
                || (row == 0 && col == 0)){
                continue;
            }
            pixelsAroundCorePoint.push_back(currPoint);
        }
    }
    return pixelsAroundCorePoint;
}

Eigen::Matrix3d UtilityClass::GetRotationMatrixByEulerAngle(const Point3D<double>& eulerAngles) {
    return GetAntiClockwiseRotationMatrixGivenEulerAngles(eulerAngles);
}

Eigen::Matrix3d UtilityClass::GetClockwiseRotationMatrixGivenEulerAngles(const Point3D<double>& eulerRotationAngles) {
    
    double CosTheta = std::cos(eulerRotationAngles.x);
    double SinTheta = std::sin(eulerRotationAngles.x);
    double RotXVector[] = {1,0,0,0,CosTheta, SinTheta, 0, -SinTheta, CosTheta};
    Eigen::Matrix3d RotX = Eigen::Matrix3d(RotXVector);
    
    CosTheta = std::cos(eulerRotationAngles.y);
    SinTheta = std::sin(eulerRotationAngles.y);
    double RotYVector[] = {CosTheta, 0, -SinTheta, 0, 1, 0 , SinTheta, 0, CosTheta};
    Eigen::Matrix3d RotY = Eigen::Matrix3d(RotYVector);
    
    CosTheta = std::cos(eulerRotationAngles.z);
    SinTheta = std::sin(eulerRotationAngles.z);
    double RotZVector[] = {CosTheta, SinTheta, 0, -SinTheta, CosTheta, 0, 0, 0, 1};
    Eigen::Matrix3d RotZ = Eigen::Matrix3d(RotZVector);
    
    Eigen::Matrix3d rotationMatrix = RotZ * RotY * RotX;
    return rotationMatrix;
    
}

Eigen::Matrix3d UtilityClass::GetAntiClockwiseRotationMatrixGivenEulerAngles(const Point3D<double>& eulerRotationAngles) {
    
    double CosTheta = std::cos(eulerRotationAngles.x);
    double SinTheta = std::sin(eulerRotationAngles.x);
    double RotXVector[] = {1,0,0,0,CosTheta, -SinTheta, 0, SinTheta, CosTheta};
    Eigen::Matrix3d RotX = Eigen::Matrix3d(RotXVector);
    
    CosTheta = std::cos(eulerRotationAngles.y);
    SinTheta = std::sin(eulerRotationAngles.y);
    double RotYVector[] = {CosTheta, 0, SinTheta, 0, 1, 0 , -SinTheta, 0, CosTheta};
    Eigen::Matrix3d RotY = Eigen::Matrix3d(RotYVector);
    
    CosTheta = std::cos(eulerRotationAngles.z);
    SinTheta = std::sin(eulerRotationAngles.z);
    double RotZVector[] = {CosTheta, -SinTheta, 0, SinTheta, CosTheta, 0, 0, 0, 1};
    Eigen::Matrix3d RotZ = Eigen::Matrix3d(RotZVector);
    
    Eigen::Matrix3d rotationMatrix = RotZ * RotY * RotX;
    return rotationMatrix;
    
}
