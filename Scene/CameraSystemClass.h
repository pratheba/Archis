//
//  CameraSystemClass.h
//  Archis
//
//  Created by Pratheba Selvaraju on 8/23/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef __Archis__CameraSystemClass__
#define __Archis__CameraSystemClass__

#include "CameraEntityClass.h"

class CameraSystemClass {
public:
    
    static CameraSystemClass& GetInstance();
    static void ReleaseInstance();
    
    const CameraEntityClass& GetCurrentCamera()const;
    void SetCurrentCamera(const CameraEntityClass& currentCamera) const;
    
    Eigen::Matrix3d GetCurrentCameraIntrinsicMatrix();
    Eigen::Matrix3Xd GetCurrentCameraExtrinsicMatrix();
    
private:
    CameraSystemClass();
    ~CameraSystemClass(){}
    
    static CameraSystemClass* _cameraSystem;
    static CameraEntityClass* _currentCamera;
    
    std::vector<CameraEntityClass*> _cameras;
    
    Eigen::Matrix3Xd GetCurrentCameraExtrinsicMatrixByEulerAngle();
    Eigen::Matrix3Xd GetCurrentCameraExtrinsicMatrixByLookAtVector();

    
};

#endif /* defined(__Archis__CameraSystemClass__) */
