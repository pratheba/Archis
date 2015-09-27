//
//  UtilityClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 8/23/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "UtilityClass.h"

double UtilityClass::GetAveragePixelIntensityAroundaPoint(const Array2D<Rgba>& inputImage, const Point2D<int>& corePoint, const int& windowRadius) {
    double pixelIntensity = 0.0;
    VecOf2dIntPoints pixelsAroundCorePoint = GetWindowAroundPoint(inputImage, corePoint, windowRadius);
    
    if (pixelsAroundCorePoint.size() > 0) {
        for (int index = 0; index < pixelsAroundCorePoint.size(); ++index) {
            Rgba value = inputImage[pixelsAroundCorePoint[index].y][pixelsAroundCorePoint[index].x];
            pixelIntensity += ((value.r + value.g + value.b)/3);
        }
    }
    Rgba Value = inputImage[corePoint.y][corePoint.x];
    pixelIntensity += ((Value.r + Value.g + Value.b)/3);
    pixelIntensity /= ((2*windowRadius+1) * (2*windowRadius+1));
    
    return pixelIntensity;
}

VecOf2dIntPoints UtilityClass::GetWindowAroundPoint(const Array2D<Rgba>& inputImage, const Point2D<int>& corePoint, const int& windowRadius) {
    double imageWidth = inputImage.width();
    double imageHeight = inputImage.height();
    VecOf2dIntPoints pixelsAroundCorePoint;
    pixelsAroundCorePoint.clear();
    
    for(int row = -windowRadius; row <= windowRadius; ++row) {
        for (int col = -windowRadius; col <= windowRadius; ++col) {
            
            Point2D<int> currPoint = Point2D<int>(corePoint.y + row, corePoint.x + col);
            if ((currPoint.y < 0) || (currPoint.y > imageHeight)
                || (currPoint.x < 0) || (currPoint.x > imageWidth)
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

// Normalized gaussian Filter



double rad2deg(const double& radian) {
    return (radian * 180 / M_PI);
}

double deg2rad(const double& degree) {
    return (degree * M_PI / 180);
}

Rgba* UtilityClass::GetImagePixelsToWrite(const int& width, const int& height) {
    Rgba* pixels = new Rgba[width * height];
    
    for (int row = 0; row < height; ++row) {
        for (int col = 0; col < width; ++col) {
            pixels[row* width + col].r = 0.0;
            pixels[row* width + col].g = 0.0;
            pixels[row* width + col].b = 0.0;
            pixels[row* width + col].a = 0.0;
        }
    }
    return pixels;
}

void UtilityClass::WriteImage2DArrayPixels(const std::string& fileName, const Rgba* pixels, const int width, const int height) {
    
    Header header(width, height);
    RgbaOutputFile file(fileName.c_str(), header, WRITE_RGBA);
    file.setFrameBuffer(pixels, 1, width);
    file.writePixels(height);
}
