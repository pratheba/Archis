//
//  CameraClass.h
//  Archis
//
//  Created by Pratheba Selvaraju on 8/23/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef __Archis__CameraEntityClass__
#define __Archis__CameraEntityClass__

#include <iostream>
#include "EntityClass.h"

class CameraEntityClass : public EntityClass {

public:
    CameraEntityClass();
    CameraEntityClass &operator=(const CameraEntityClass& cameraEntityClass);
    ~CameraEntityClass();
    
    Point2D<double> GetFocalLength()const;
    Point2D<double> GetFocalLengthInPixels() const;
    void SetFocalLength(const Point2D<double>& focalLength);
    
    double GetFieldOfView()const;
    void SetFieldOfView(const double& fieldOfView);
    
    double GetPixelWidth()const;
    void SetPixelWidth(const double& pixelWidth);
    
    double GetPixelHeight()const;
    void SetPixelHeight(const double& pixelHeight);
    
    Point2D<double> GetPrincipalPoint()const;
    void SetPrincipalPoint(const Point2D<double>& principalPoint);
    
private:
   
    double              _fieldOfView;
    double              _pixelWidth;
    double              _pixelHeight;
    Point2D<double>     _focal_length;
    Point2D<double>     _focal_lengthInPixels;
    Point2D<double>     _principalPoint;
   
    void ConvertFocalLengthToPixels();
};

#endif /* defined(__Archis__CameraEntityClass__) */
