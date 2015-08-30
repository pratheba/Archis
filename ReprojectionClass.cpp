//
//  ReprojectionClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 8/27/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "ReprojectionClass.h"

ReprojectionClass::ReprojectionClass():imageClass(ImageSystemClass::GetInstance()),geometryClass(GeometrySystemClass::GetInstance()),cameraClass(CameraSystemClass::GetInstance()),imageEntity(imageClass.GetCurrentImage()),geometryEntity(geometryClass.GetCurrentGeometry()),camEntity(cameraClass.GetCurrentCamera()) {}

ReprojectionClass::~ReprojectionClass() {
}

 Eigen::Vector3d ReprojectionClass::GetCenterOfStartOfDataImagePixelsInWorldCoord() {
     
     int imageWidth = (int)(imageEntity.GetImage2DArrayPixels()).width();
     int imageHeight = (int)(imageEntity.GetImage2DArrayPixels()).height();
     
     double pixelWidth              = camEntity.GetPixelWidth();
     double pixelHeight             = camEntity.GetPixelHeight();
     Point2D<double> focal_length   = camEntity.GetFocalLengthInPixels();
     EntityClass::TransformationCOORD camTransCoord = camEntity.GetTransformationCoord();
     
     
     // For image pixel location to u,v,w pixel location.
     
     // For any image plane - center pixel is
     // CenterPoint = eye + focal_length * wVec;
     // Point(0,0) = eye + focal_length * wVec - (xresolution * pixelWidth/2) *uVec + (yresolution * pixelHeight/2) * vVec.
     // Pixel(0,0) = Point(0,0) + (pixelWidth/2)*uVec - (pixelHeight/2)*vVec;
     
     // pixel at location X,Y = CenterPixel + (X + pixelWidth/2) * uVec) -(Y + pixelHeight/2)*vVec;
     
     // We assume focal_length.x == focal_length.y
     // Z = -Z , Since camera looks at -z axis;
     Eigen::Vector3d centerPixelInImageInWorld = camTransCoord._posVec + (-1*camTransCoord._zVec) * focal_length.x;
     Eigen::Vector3d worldPointOfPixel00 =
        centerPixelInImageInWorld
     - ((imageWidth * pixelWidth/2)* camTransCoord._xVec)
     + ((imageHeight * pixelHeight/2)*camTransCoord._yVec);
     
     Eigen::Vector3d centerOfPixel00 =
     worldPointOfPixel00 + (pixelWidth/2)*camTransCoord._xVec - (pixelHeight/2)*camTransCoord._yVec;
     
     return centerOfPixel00;
     
}

// Refactor this

std::vector<MapOFImageAndWorldPoints> ReprojectionClass::ReprojectImagePixelsTo3DGeometry(const Array2D<Rgba>& imagePixels) {
    std::vector<MapOFImageAndWorldPoints> reprojectedPoints;
    
    /**** Geometry Data ****/
    std::vector<Point3D<double>> vertices;
    std::vector<Eigen::Vector3d> vertexnormals;
    Point3D<double> vertex;
    Eigen::Vector3d vnormal;
    
    GEOMETRYTYPE geometryType = geometryEntity.GetGeometryType();
    if (geometryType == PLANE) {
        vertex = (geometryEntity.GetVertices()).at(0);
        vnormal = (geometryEntity.GetVertexNormals()).at(0);
    }
    else {
        vertices       = geometryEntity.GetVertices();
        vertexnormals  = geometryEntity.GetVertexNormals();
    }
    MaxMinCoord maxminCoord = geometryEntity.GetMaxMinOfGeometry();
    
    /**** Camera Data ****/
    Eigen::Vector3d camposition = camEntity.GetPosition();
    EntityClass::TransformationCOORD camTransCoord = camEntity.GetTransformationCoord();
    
    /**** Image Data ****/
    Eigen::Vector3d centerOfPixel00 = GetCenterOfStartOfDataImagePixelsInWorldCoord();
    int imageWidth = (int)(imageEntity.GetImage2DArrayPixels()).width();
    int imageHeight = (int)(imageEntity.GetImage2DArrayPixels()).height();
    
    
    /***** Implementation ****/
    double numerator = (Eigen::Vector3d(vertex.x,vertex.y,vertex.z) - camposition).dot(vnormal);
    double denom = 0;
    for (int row = 0; row < imageHeight; ++row) {
        for (int col = 0; col < imageWidth; ++col) {
            Eigen::Vector3d pixel = centerOfPixel00 + col*camTransCoord._xVec - row*camTransCoord._yVec;
            denom = (pixel - camposition).dot(vnormal);
            
            if (denom != 0)
            {
                double t = numerator/denom;
                if (t > 0)
                {
                    Eigen::Vector3d intersectionPoint = camposition + t*(pixel - camposition);
                    if ((intersectionPoint.x() >= maxminCoord._minX && intersectionPoint.x() <= maxminCoord._maxX)
                        &&(intersectionPoint.y() >= maxminCoord._minY && intersectionPoint.y() <= maxminCoord._maxY))
                    {
                        reprojectedPoints.push_back(MapOFImageAndWorldPoints(Point2D<int>(col,row),intersectionPoint));
                    }
                }
            }
        }
    }
    return reprojectedPoints;
}

