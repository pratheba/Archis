//
//  EntityClass.h
//  Archis
//
//  Created by Pratheba Selvaraju on 8/23/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef __Archis__EntityClass__
#define __Archis__EntityClass__

#include <iostream>
#include <vector>
#include "UtilityClass.h"


class EntityClass {
public:
    
    EntityClass();
    ~EntityClass();
    
    struct EntityLookAtVector {
        Eigen::Vector3d _eyeVector;
        Eigen::Vector3d _upVector;
        Eigen::Vector3d _directionVector;
        
        EntityLookAtVector():_eyeVector(0,0,0),_upVector(0,0,0),_directionVector(0,0,0){}
        ~EntityLookAtVector(){}
    };
    
    struct TransformationCOORD {
        Eigen::Vector3d _posVec;
        Eigen::Vector3d _xVec; // right
        Eigen::Vector3d _yVec; // up
        Eigen::Vector3d _zVec; // -lookat
        
        TransformationCOORD():_posVec(0,0,0), _xVec(0,0,0),_yVec(0,0,0),_zVec(0,0,0){}
        TransformationCOORD(const Eigen::Vector3d& posVec, const Eigen::Vector3d& xVec, const Eigen::Vector3d& yVec, const Eigen::Vector3d& zVec):_posVec(posVec),_xVec(xVec), _yVec(yVec), _zVec(zVec){}
        TransformationCOORD(const TransformationCOORD& coord):_posVec(coord._posVec),_xVec(coord._xVec), _yVec(coord._yVec), _zVec(coord._zVec){};
        ~TransformationCOORD(){}
    };
    
    Eigen::Vector3d GetPosition() const;
    void SetPosition(const Eigen::Vector3d& position)const;
    
    Point3D<double> GetScale() const;
    void SetScale(const Point3D<double>& scale);
    
    Point3D<double> GetRotation() const;
    void SetRotation(const Point3D<double>& rotation);
    
    Eigen::Vector3d GetLookAtUpVector();
    void SetLookAtUpVector(const Eigen::Vector3d& upVector);
    
    Eigen::Vector3d GetLookAtDirectionVector() const;
    void SetLookAtDirectionVector(const Eigen::Vector3d& directionVector);
    
    Eigen::Vector3d GetUpVector() const;
    void SetUpVector(const Eigen::Vector3d& upVector);
    
    Eigen::Vector3d GetDirectionVector() const;
    void SetDirectionVector(const Eigen::Vector3d& directionVector);
    
    Eigen::Matrix3d GetRotationMatrix() const;
    void SetRotationMatrix(const Eigen::Matrix3d& rotationMatrix);
    
    EntityLookAtVector GetEntityLookAtVector();
    void SetEntityLookAtVector(const EntityLookAtVector& entityLookAtVector);
    
    Eigen::Matrix4d GetLightTransformationMatrix();
    void SetLightTransformation(const Eigen::Matrix4d& lightTransformationMatrix);
    
    TransformationCOORD GetTransformationCoord() const;
    void SetTransformationCoord(const TransformationCOORD& transformationCoord);
    
    void DecomposeTransformationMatrix();
    TransformationCOORD GetEntityCoordinateAxesFromLookAtVector();
    
    
    
protected:
    
    Eigen::Vector3d _position;
    Point3D<double> _scale;
    Point3D<double> _rotation;
    Eigen::Matrix3d _rotationMatrix;
    Eigen::Matrix4d _transformationMatrix;
    EntityLookAtVector* _entityLookAtVector;
    TransformationCOORD* _transformationCoord;
    
};

#endif /* defined(__Archis__EntityClass__) */
