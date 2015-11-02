
//  ReprojectionClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 8/27/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "ReprojectionClass.h"

class PixelToWorldCoord {
    
public:
    PixelToWorldCoord():_SensorWidth(0),_SensorHeight(0),_PixelWidth(0),_PixelHeight(0){}

    // Pixel Width :: Pixel Height
    //     tan(FOV(degree)/2)  = Image Plane Width In world Units (Image sensor Width) / (2 * Field Of View In world Units);
    //     Image Sensor Width  = tan(FOV(degree)/2) * (Focal Length In world Units) * 2;
    //     Image Sensor Height = Image Sensor Width / Aspect ratio /* Image Sensor Width * ImagePixelHeight/ImagePixelWidth */
    //     Pixel Width         = Image Sensor Width/ ImagePixelWidth;
    //     Pixel Height        = Image Sensor Height / ImagePixelHeight;
    
    // This focal length is in meters/world unit
    PixelToWorldCoord(const double& FOV, const double& focal_length, const double& imageWidth, const double& imageHeight) {
         _SensorWidth  = tan(FOV * 0.5 * M_PI/180) * focal_length * 2;
         _SensorHeight = _SensorWidth * imageHeight / imageWidth;
         _PixelWidth   = _SensorWidth / imageWidth;
         _PixelHeight  = _SensorHeight / imageHeight;
    }
    
    ~PixelToWorldCoord(){}
    
    double GetSensorWidth() { return _SensorWidth;}
    double GetSensorHeight() { return _SensorHeight;}
    double GetPixelWidth() { return _PixelWidth;}
    double GetPixelHeight() { return _PixelHeight;}
    
    void SetSensorWidth(const double& sensorWidth) { _SensorWidth = sensorWidth;}
    void SetSensorHeight(const double& sensorHeight) { _SensorHeight = sensorHeight;}
    void SetPixelWidth(const double& pixelWidth) { _PixelWidth = pixelWidth;}
    void SetPixelHeight(const double& pixelHeight) {  _PixelHeight = pixelHeight;}

    
private:
    double _SensorWidth;
    double _SensorHeight;
    double _PixelWidth;
    double _PixelHeight;
    
    };

ReprojectionClass::ReprojectionClass():imageClass(ImageSystemClass::GetInstance()),geometryClass(GeometrySystemClass::GetInstance()),cameraClass(CameraSystemClass::GetInstance()),imageEntity(imageClass.GetCurrentImage()),geometryEntity(geometryClass.GetCurrentGeometry()),camEntity(cameraClass.GetCurrentCamera()) {}

ReprojectionClass::~ReprojectionClass() {
}

 Eigen::Vector3d ReprojectionClass::GetCenterOfStartOfDataImagePixelsInWorldCoord() {
     
     int imageWidth = (int)(imageEntity.GetImage2DArrayPixels()).width();
     int imageHeight = (int)(imageEntity.GetImage2DArrayPixels()).height();
     
     EntityClass::TransformationCOORD camTransCoord = camEntity.GetTransformationCoord();
     PixelToWorldCoord PixelToWorld(camEntity.GetFieldOfView(), (camEntity.GetFocalLength()).x, imageWidth, imageHeight);
     
     
     // For image pixel location to u,v,w pixel location.
     
     // For any image plane - center pixel is
     // CenterPoint = eye + focal_length * wVec;
     // Point(0,0) = eye + focal_length * wVec - (xresolution * pixelWidth/2) *uVec + (yresolution * pixelHeight/2) * vVec.
     // Pixel(0,0) = Point(0,0) + (pixelWidth/2)*uVec - (pixelHeight/2)*vVec;
     
     // pixel at location X,Y = CenterPixel + (X + pixelWidth/2) * uVec) -(Y + pixelHeight/2)*vVec;
     
     // We assume focal_length.x == focal_length.y
     // Z = -Z , Since camera looks at -z axis;
     
     
     camTransCoord._zVec.normalize();
     Eigen::Vector3d centerPixelInImageInWorld = camTransCoord._posVec + (-1*camTransCoord._zVec) * (camEntity.GetFocalLength()).x;

     Eigen::Vector3d worldPointOfPixel00 =
     centerPixelInImageInWorld
     - ((PixelToWorld.GetSensorWidth()/2)* camTransCoord._xVec)
     + ((PixelToWorld.GetSensorHeight()/2)*camTransCoord._yVec);
     
     Eigen::Vector3d centerOfPixel00 =
     worldPointOfPixel00 + ((PixelToWorld.GetPixelWidth())/2)*camTransCoord._xVec - (PixelToWorld.GetPixelHeight()/2)*camTransCoord._yVec;
     
     return centerOfPixel00;
}

// Refactor this

std::vector<MapOFImageAndWorldPoints> ReprojectionClass::ReprojectImagePixelsTo3DGeometry(const std::vector<Point2D<double>>& imagePixels) {
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
        vnormal.normalize();
    }
    else {
        vertices       = geometryEntity.GetVertices();
        vertexnormals  = geometryEntity.GetVertexNormals();
    }
    MaxMinCoord maxminCoord = geometryEntity.GetMaxMinOfGeometry();
    
    /**** Camera Data ****/
    EntityClass::TransformationCOORD camTransCoord = camEntity.GetTransformationCoord();
    
       /**** Image Data ****/
    Eigen::Vector3d centerOfPixel00 = GetCenterOfStartOfDataImagePixelsInWorldCoord();
    int imageWidth = (int)(imageEntity.GetImage2DArrayPixels()).width();
    int imageHeight = (int)(imageEntity.GetImage2DArrayPixels()).height();
    
    PixelToWorldCoord PixelToWorld(camEntity.GetFieldOfView(), (camEntity.GetFocalLength()).x, imageWidth, imageHeight);
    double PixelWidth = PixelToWorld.GetPixelWidth();
    double PixelHeight = PixelToWorld.GetPixelHeight();
    
    /***** Implementation ****/
//    double d = -(Eigen::Vector3d(vertex.x,vertex.y,vertex.z).dot(vnormal));
//    double numerator =  -(d + camTransCoord._posVec.dot(vnormal));
    double numerator = (Eigen::Vector3d(vertex.x,vertex.y,vertex.z) - camTransCoord._posVec).dot(vnormal);
    double denom = 0;
    
    for(int index = 0; index < imagePixels.size(); ++index) {
        Eigen::Vector3d pixel = centerOfPixel00 + imagePixels[index].x* PixelWidth* camTransCoord._xVec - imagePixels[index].y* PixelHeight*camTransCoord._yVec;
        Eigen::Vector3d ray = pixel - camTransCoord._posVec;
        ray.normalize();
        denom = ray.dot(vnormal);
        
        if (denom != 0)
        {
            double t = numerator/denom;
            if (t > 0)
            {
                Eigen::Vector3d intersectionPoint = camTransCoord._posVec + t*(ray);
                if ((intersectionPoint.x() >= maxminCoord._minX && intersectionPoint.x() <= maxminCoord._maxX)
                    &&(intersectionPoint.y() >= maxminCoord._minY && intersectionPoint.y() <= maxminCoord._maxY))
                {
                    reprojectedPoints.push_back(MapOFImageAndWorldPoints(Point2D<int>(imagePixels[index].y,imagePixels[index].x),intersectionPoint));
                }
            }
        }
    }
    return reprojectedPoints;
}

std::vector<MapOFImageAndWorldPoints> ReprojectionClass::ReprojectImagePixelsTo3DGeometry() {
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
        vnormal.normalize();
    }
    else {
        vertices       = geometryEntity.GetVertices();
        vertexnormals  = geometryEntity.GetVertexNormals();
    }
    MaxMinCoord maxminCoord = geometryEntity.GetMaxMinOfGeometry();
    
    /**** Camera Data ****/
    EntityClass::TransformationCOORD camTransCoord = camEntity.GetTransformationCoord();
    
    /**** Image Data ****/
    Eigen::Vector3d centerOfPixel00 = GetCenterOfStartOfDataImagePixelsInWorldCoord();
    int imageWidth = (int)(imageEntity.GetImage2DArrayPixels()).width();
    int imageHeight = (int)(imageEntity.GetImage2DArrayPixels()).height();
    
    PixelToWorldCoord PixelToWorld(camEntity.GetFieldOfView(), (camEntity.GetFocalLength()).x, imageWidth, imageHeight);
    double PixelWidth = PixelToWorld.GetPixelWidth();
    double PixelHeight = PixelToWorld.GetPixelHeight();
    
    /***** Implementation ****/
    //    double d = -(Eigen::Vector3d(vertex.x,vertex.y,vertex.z).dot(vnormal));
    //    double numerator =  -(d + camTransCoord._posVec.dot(vnormal));
    double numerator = (Eigen::Vector3d(vertex.x,vertex.y,vertex.z) - camTransCoord._posVec).dot(vnormal);
    double denom = 0;
    
    for (int row = 0; row < imageHeight; ++row) {
        for (int col = 0; col < imageWidth; ++col) {
            
            Eigen::Vector3d pixel = centerOfPixel00 + col* PixelWidth* camTransCoord._xVec - row* PixelHeight*camTransCoord._yVec;
            Eigen::Vector3d ray = pixel - camTransCoord._posVec;
            ray.normalize();
            denom = ray.dot(vnormal);
            
            if (denom != 0)
            {
                double t = numerator/denom;
                if (t > 0)
                {
                    Eigen::Vector3d intersectionPoint = camTransCoord._posVec + t*(ray);
                    if ((intersectionPoint.x() >= maxminCoord._minX && intersectionPoint.x() <= maxminCoord._maxX)
                        &&(intersectionPoint.y() >= maxminCoord._minY && intersectionPoint.y() <= maxminCoord._maxY))
                    {
                        reprojectedPoints.push_back(MapOFImageAndWorldPoints(Point2D<int>(row,col),intersectionPoint));
                    }
                }
            }
        }
    }
    return reprojectedPoints;
}

