//
//  UtilityClass.h
//  Archis
//
//  Created by Pratheba Selvaraju on 8/23/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef __Archis__UtilityClass__
#define __Archis__UtilityClass__

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

/*
template <class Type>
struct Vec3D {
    
    Type data[3];
    
    Vec3D(){ data[0] = 0; data[1] = 0; data[2] = 0;}
    Vec3D(Type x_, Type y_, Type z_){data[0] = x_; data[1] = y_; data[2] = z_;}
    
    void operator=(const Vec3D<Type>& inVector){ *this =  inVector;}
    double dot(const Vec3D<Type>& inVector){
        double value = this->data[0]*inVector.data[0] + this->data[1]*inVector.data[1] + this->data[2]*inVector.data[2];
        return value;
    }
    
    Vec3D<Type> cross(const Vec3D<Type>& inVector) {
        Vec3D<Type> crossProduct;
        crossProduct.data[0] = this->data[1]*inVector.data[2] - this->data[2]*inVector.data[1];
        crossProduct.data[1] = this->data[0]*inVector.data[2] - this->data[2]*inVector.data[0];
        crossProduct.data[2] = this->data[0]*inVector.data[1] - this->data[1]*inVector.data[0];
        return crossProduct;
    }
};
 */



template <class Type>
struct Point2D {
    
    Type x; // col
    Type y; // row
    
    Point2D() { x=0.0; y=0.0;}
    Point2D(const Type& x_, const Type& y_):x(x_), y(y_){}
    Point2D(const Point2D<Type>& point_):x(point_.x),y(point_.y){}
    
    Point2D& operator=(const Point2D<Type>& inPoint){
        if(this == &inPoint)
            return *this;
        x = inPoint.x;
        y = inPoint.y;
        return *this;
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

class UtilityClass {
public:
    std::ofstream outputfile;
    
    double GetAveragePixelIntensityAroundaPoint(const Array2D<Rgba>& inputImage, const Point2D<int>& corePoint, const int& windowSize);
    Eigen::Matrix3d GetRotationMatrixByEulerAngle(const Point3D<double>& eulerAngles);
    
    Rgba* GetImagePixelsToWrite(const int& width,const int& height);
    
private:
    Eigen::Matrix3d GetClockwiseRotationMatrixGivenEulerAngles(const Point3D<double>& eulerRotationAngles);
    Eigen::Matrix3d GetAntiClockwiseRotationMatrixGivenEulerAngles(const Point3D<double>& eulerRotationAngles);
    VecOf2dIntPoints GetWindowAroundPoint(const Array2D<Rgba>& inputImage, const Point2D<int>& corePoint, const int& windowSize);
    
};

#endif /* defined(__Archis__UtilityClass__) */
