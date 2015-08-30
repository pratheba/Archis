//
//  CameraSystemClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 8/23/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "CameraSystemClass.h"

CameraSystemClass* CameraSystemClass::_cameraSystem = nullptr;
CameraEntityClass* CameraSystemClass::_currentCamera = nullptr;

CameraSystemClass& CameraSystemClass::GetInstance() {
    if (_cameraSystem == nullptr) {
        _cameraSystem = new CameraSystemClass();
    }
    return *_cameraSystem;
}

CameraSystemClass::CameraSystemClass() {
    if (_currentCamera == nullptr) {
        _currentCamera = new CameraEntityClass();
    }
}

void CameraSystemClass::ReleaseInstance() {
    if (_cameraSystem != nullptr) {
        
        if (_currentCamera != nullptr) {
            delete _currentCamera;
            _currentCamera = nullptr;
        }
        
        delete _cameraSystem;
        _cameraSystem = nullptr;
    }
}

const CameraEntityClass& CameraSystemClass::GetCurrentCamera() const{
    return *_currentCamera;
}

void CameraSystemClass::SetCurrentCamera(const CameraEntityClass& currentCamera) const{
    *_currentCamera = currentCamera;
}

Eigen::Matrix3d CameraSystemClass::GetCurrentCameraIntrinsicMatrix() {
    
    Eigen::Matrix3d intrinsicMatrix(Eigen::Matrix3d::Zero(3, 3));
    Point2D<double> focalLength     = _currentCamera->GetFocalLengthInPixels();
    Point2D<double> principalPoint  = _currentCamera->GetPrincipalPoint();
    intrinsicMatrix(0,0) = focalLength.y;
    intrinsicMatrix(1,1) = focalLength.x;
    intrinsicMatrix(0,2) = principalPoint.y;
    intrinsicMatrix(1,2) = principalPoint.x;
    intrinsicMatrix(2,2) = 1;
    
    return intrinsicMatrix;
}

Eigen::Matrix3Xd CameraSystemClass::GetCurrentCameraExtrinsicMatrix() {
    
    return GetCurrentCameraExtrinsicMatrixByLookAtVector();
    
}

Eigen::Matrix3Xd CameraSystemClass::GetCurrentCameraExtrinsicMatrixByLookAtVector() {
    Eigen::Matrix3Xd extrinsicMatrix(Eigen::Matrix3Xd::Zero(3, 4));
    CameraEntityClass::TransformationCOORD localCoord = _currentCamera->GetEntityCoordinateAxesFromLookAtVector();
    
    extrinsicMatrix.col(0) = localCoord._xVec;
    extrinsicMatrix.col(1) = localCoord._yVec;
    extrinsicMatrix.col(2) = localCoord._zVec;
    extrinsicMatrix.col(3) = Eigen::Vector3d(-1*(_currentCamera->GetPosition()).dot(localCoord._xVec),
                                             -1*(_currentCamera->GetPosition()).dot(localCoord._yVec),
                                             -1*(_currentCamera->GetPosition()).dot(localCoord._zVec));
    
    return extrinsicMatrix;
}

Eigen::Matrix3Xd CameraSystemClass::GetCurrentCameraExtrinsicMatrixByEulerAngle() {
    UtilityClass* utilClass = new UtilityClass();
    Eigen::Matrix3Xd rotationMatrix = utilClass->GetRotationMatrixByEulerAngle(_currentCamera->GetRotation());
    
    delete utilClass;
    return rotationMatrix;
}
