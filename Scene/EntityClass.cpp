//
//  EntityClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 8/23/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "EntityClass.h"

EntityClass::EntityClass():_entityLookAtVector(new EntityLookAtVector()),_transformationCoord(new TransformationCOORD()) {
    _transformationMatrix = Eigen::Matrix4d::Zero();
    _position = Eigen::Vector3d(0,0,0);
    _scale = Point3D<double>(0,0,0);
    _rotation = Point3D<double>(0,0,0);
    _rotationMatrix = Eigen::Matrix3d::Zero();

}

EntityClass::~EntityClass(){
    if (_entityLookAtVector != nullptr) {
        delete _entityLookAtVector;
        _entityLookAtVector = nullptr;
    }
}

Eigen::Vector3d EntityClass::GetPosition() const {
    return _position;
}
void EntityClass::SetPosition(const Eigen::Vector3d& position) {
    this->_position = position;
    _transformationCoord->_posVec = position;
    _entityLookAtVector->_eyeVector = position;
}

Point3D<double> EntityClass::GetScale() const {
    return _scale;
}
void EntityClass::SetScale(const Point3D<double>& scale) {
    _scale = scale;
}

Point3D<double> EntityClass::GetRotation() const {
    return _rotation;
}
void EntityClass::SetRotation(const Point3D<double>& rotation) {
    _rotation = rotation;
}

Eigen::Vector3d EntityClass::GetLookAtUpVector() {
    return _entityLookAtVector->_upVector;
}
void EntityClass::SetLookAtUpVector(const Eigen::Vector3d& upVector) {
    _entityLookAtVector->_upVector = upVector;
}

Eigen::Vector3d EntityClass::GetLookAtDirectionVector() const {
    return _entityLookAtVector->_directionVector;
}

void EntityClass::SetLookAtDirectionVector(const Eigen::Vector3d& directionVector) {
    _entityLookAtVector->_directionVector = directionVector;
}

Eigen::Vector3d EntityClass::GetUpVector() const {
    return _transformationCoord->_yVec;
}
void EntityClass::SetUpVector(const Eigen::Vector3d& upVector) {
    _transformationCoord->_yVec = upVector;
}

Eigen::Vector3d EntityClass::GetDirectionVector() const{
    return _transformationCoord->_zVec;
}

void EntityClass::SetDirectionVector(const Eigen::Vector3d& directionVector) {
    _transformationCoord->_zVec = directionVector;
}

Eigen::Matrix3d EntityClass::GetRotationMatrix() const{
    return _rotationMatrix;
}
void EntityClass::SetRotationMatrix(const Eigen::Matrix3d& rotationMatrix) {
    _rotationMatrix = rotationMatrix;
}

Eigen::Matrix4d EntityClass::GetLightTransformationMatrix() const {
    return _transformationMatrix;
}
void EntityClass::SetLightTransformation(const Eigen::Matrix4d& lightTransformationMatrix) {
    _transformationMatrix = lightTransformationMatrix;
    DecomposeTransformationMatrix();
}

EntityClass::EntityLookAtVector EntityClass::GetEntityLookAtVector() const{
    return *_entityLookAtVector;
}
void EntityClass::SetEntityLookAtVector(const EntityLookAtVector& entityLookAtVector) {
    *_entityLookAtVector = entityLookAtVector;
    _position = entityLookAtVector._eyeVector;
    _entityLookAtVector->_upVector.normalize();
    *_transformationCoord = GetEntityCoordinateAxesFromLookAtVector();
}

EntityClass::TransformationCOORD EntityClass::GetTransformationCoord() const {
    return *_transformationCoord;
}

void EntityClass::SetTransformationCoord(const EntityClass::TransformationCOORD& transformationCoord) {
    *_transformationCoord = transformationCoord;
}


void EntityClass::DecomposeTransformationMatrix() {
    int rows = (int)_transformationMatrix.rows();
    int cols = (int)_transformationMatrix.cols();
    
    if (rows == 4 && cols == 4) {
        this->_position = _transformationMatrix.topRightCorner(3, 1);
        this->_rotationMatrix = _transformationMatrix.topLeftCorner(3, 3);
        
        double thetaX = std::atan(this->_rotationMatrix(1,2)/this->_rotationMatrix(2,2));
        double thetaY = -std::asin(this->_rotationMatrix(0,2));
        double thetaZ = std::atan(this->_rotationMatrix(0,2)/this->_rotationMatrix(0,1));
        this->_rotation = Point3D<double>(thetaX, thetaY, thetaZ);
       
        this->_transformationCoord->_posVec = this->_position;
        
        this->_transformationCoord->_zVec = this->_rotationMatrix.col(2);
        this->_transformationCoord->_zVec.normalize();
        
        this->_transformationCoord->_yVec = this->_rotationMatrix.col(1);
        this->_transformationCoord->_yVec.normalize();
        
        this->_transformationCoord->_xVec = this->_rotationMatrix.col(0);
        this->_transformationCoord->_xVec.normalize();
        
        /*
         Cannot get up vector from the transformation matrix
        this->_entityLookAtVector->_directionVector = this->_transformationCoord->_posVec - this->_transformationCoord->_zVec;
        this->_entityLookAtVector->_eyeVector = this->_transformationCoord->_posVec;
         */
        
    }
}

EntityClass::TransformationCOORD EntityClass::GetEntityCoordinateAxesFromLookAtVector() {
    
    // Since we have left handed co-ordinate system and
    Eigen::Vector3d zVec = (_entityLookAtVector->_eyeVector - _entityLookAtVector->_directionVector);
    zVec.normalize();
    Eigen::Vector3d xVec = _entityLookAtVector->_upVector.cross(zVec);
    xVec.normalize();
    Eigen::Vector3d yVec = zVec.cross(xVec);
    yVec.normalize();
    
    return (TransformationCOORD(_entityLookAtVector->_eyeVector, xVec, yVec, zVec));
}



