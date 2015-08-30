//
//  CameraClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 8/23/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "CameraEntityClass.h"
#include <cmath>

CameraEntityClass::CameraEntityClass():_focal_length(0,0),_focal_lengthInPixels(0,0),_fieldOfView(0),_pixelWidth(0),_pixelHeight(0) {
}

CameraEntityClass::~CameraEntityClass() {
}

CameraEntityClass& CameraEntityClass::operator=(const CameraEntityClass& cameraEntityClass) {
    if (this == &cameraEntityClass) {
        return *this;
    }
    
    _fieldOfView = cameraEntityClass._fieldOfView;
    _focal_length = cameraEntityClass._focal_length;
    _focal_lengthInPixels = cameraEntityClass._focal_lengthInPixels;
    _pixelWidth = cameraEntityClass._pixelWidth;
    _pixelHeight = cameraEntityClass._pixelHeight;
    _principalPoint = cameraEntityClass._principalPoint;
    
    return *this;
}

Point2D<double> CameraEntityClass::GetFocalLengthInPixels() const{
    return _focal_lengthInPixels;
}

Point2D<double> CameraEntityClass::GetFocalLength() const{
    return _focal_length;
}

void CameraEntityClass::SetFocalLength(const Point2D<double>& focalLength) {
    ConvertFocalLengthToPixels();
    _focal_length = focalLength;
}

double CameraEntityClass::GetFieldOfView() const{
    return _fieldOfView;
}
void CameraEntityClass::SetFieldOfView(const double& fieldOfView) {
    _fieldOfView = fieldOfView;
}

double CameraEntityClass::GetPixelWidth() const{
    return _pixelWidth;
}
void CameraEntityClass::SetPixelWidth(const double& pixelWidth) {
    _pixelWidth = pixelWidth;
}

double CameraEntityClass::GetPixelHeight()const {
    return  _pixelHeight;
}
void CameraEntityClass::SetPixelHeight(const double& pixelHeight) {
    _pixelHeight = pixelHeight;
}

Point2D<double> CameraEntityClass::GetPrincipalPoint()const {
    return _principalPoint;
}

void CameraEntityClass::SetPrincipalPoint(const Point2D<double>& principalPoint) {
    _principalPoint = principalPoint;
}


void CameraEntityClass::ConvertFocalLengthToPixels() {
    
    // tan(theta/2) = (d/2*f)
    // f = d*0.5/(tan(theta*0.5)
    
    
    //    if(sensorWidth != 0)
    //        focal_pixel = (focalLength.x / sensorWidth) * imagePlane.rows;
    //    else if(fieldOfView != 0)
    //        focal_pixel = (imagePlane.rows * 0.5)/(tan(fieldOfView*0.5* (M_PI/180)));
    //
    
    double focal_pixel = 0;
    
    if (_principalPoint.x  <= _principalPoint.y)
        focal_pixel = (_principalPoint.x)/(tan(_fieldOfView*0.5* (M_PI/180)));
    else
        focal_pixel = (_principalPoint.y)/(tan(_fieldOfView*0.5* (M_PI/180)));
    
    _focal_lengthInPixels.x = focal_pixel;
    _focal_lengthInPixels.y = focal_pixel;
    
}

