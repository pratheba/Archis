//
//  UtilityClass.h
//  Archis
//
//  Created by Pratheba Selvaraju on 8/23/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef __Archis__UtilityClass__
#define __Archis__UtilityClass__

#pragma once
#include <iostream>
#include <vector>
#include <eigen3/Eigen/Dense>
#include <OpenEXR/ImfRgbaFile.h>
#include <OpenEXR/ImfArray.h>
#include <fstream>

template <class Type>
struct Point2D;

typedef std::vector<Point2D<double>> VecOf2dDoublePoints;
typedef std::vector<Point2D<int>> VecOf2dIntPoints;
typedef std::vector<Eigen::MatrixXd> VecOfMat;
typedef std::vector<int> VecOfInt;
typedef std::vector<bool> VecOfBool;

using namespace Imf_2_2;

#define MINPIXELINBIN 50



template <class Type>
struct Point2D {
    
    Type x; // col
    Type y; // row
    
    Point2D() { x=0.0; y=0.0;}
    Point2D(const Type& y_, const Type& x_):y(y_), x(x_){}
    Point2D(const Point2D<Type>& point_):x(point_.x),y(point_.y){}
    
    Point2D& operator=(const Point2D<Type>& inPoint){
        if(this == &inPoint)
            return *this;
        x = inPoint.x;
        y = inPoint.y;
        return *this;
    }
    
    Point2D operator+(const Point2D<Type>& inPoint) {
        Point2D<Type> outputPoint;
        outputPoint.x = this->x + inPoint.x;
        outputPoint.y = this->y + inPoint.y;
        return outputPoint;
    }
};

template <class Type>
struct Point3D {
    
    Type x;
    Type y;
    Type z;
    
    Point3D() { x=0.0; y=0.0; z=0.0;}
    Point3D(const Type& x_, const Type& y_, const Type& z_):x(x_), y(y_), z(z_){}
    Point3D(const Point3D<Type>& point_):x(point_.x),y(point_.y),z(point_.z){}
    
    Point3D& operator=(const Point3D<Type>& inPoint) {
        if(this == &inPoint) return *this;
        
        x = inPoint.x;
        y = inPoint.y;
        z = inPoint.z;
        
        return *this;
    }
};

struct MapOFImageAndWorldPoints {
    Point2D<int>    _imagepixel;
    Eigen::Vector3d _worldPoint;
    
    MapOFImageAndWorldPoints(){}
    MapOFImageAndWorldPoints(const Point2D<int>& imagepixel, const Eigen::Vector3d& worldPoint):_imagepixel(imagepixel), _worldPoint(worldPoint){}
};

double rad2deg(const double& radian);
double deg2rad(const double& degree);

class UtilityClass {
public:
    std::ofstream outputfile;
    
    double GetAveragePixelIntensityAroundaPoint(const Array2D<Rgba>& inputImage, const Point2D<int>& corePoint, const int& windowSize);
    Eigen::Matrix3d GetRotationMatrixByEulerAngle(const Point3D<double>& eulerAngles);
    Rgba* GetImagePixelsToWrite(const int& width,const int& height);
    void WriteImage2DArrayPixels(const std::string& fileName, const Rgba* pixels, const int width, const int height);
    
private:
    Eigen::Matrix3d GetClockwiseRotationMatrixGivenEulerAngles(const Point3D<double>& eulerRotationAngles);
    Eigen::Matrix3d GetAntiClockwiseRotationMatrixGivenEulerAngles(const Point3D<double>& eulerRotationAngles);
    VecOf2dIntPoints GetWindowAroundPoint(const Array2D<Rgba>& inputImage, const Point2D<int>& corePoint, const int& windowSize);
    
};

#endif /* defined(__Archis__UtilityClass__) */
