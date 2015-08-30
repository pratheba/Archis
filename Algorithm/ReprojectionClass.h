//
//  ReprojectionClass.h
//  Archis
//
//  Created by Pratheba Selvaraju on 8/27/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef __Archis__ReprojectionClass__
#define __Archis__ReprojectionClass__

#include <iostream>
#include "../Utility/UtilityClass.h"
#include "../Scene/GeometrySystemClass.h"
#include "../Scene/CameraSystemClass.h"
#include "../Image/ImageSystemClass.h"

class ReprojectionClass {
public:
    ReprojectionClass();
    ~ReprojectionClass();
    
    std::vector<MapOFImageAndWorldPoints> ReprojectImagePixelsTo3DGeometry(const Array2D<Rgba>& imagePixels);

private:
    const GeometrySystemClass& geometryClass;
    const CameraSystemClass& cameraClass;
    const ImageSystemClass& imageClass;
    
    const Image2DClass& imageEntity;
    const GeometryEntityClass& geometryEntity;
    const CameraEntityClass& camEntity;
    
    
    Eigen::Vector3d GetCenterOfStartOfDataImagePixelsInWorldCoord();
};

#endif /* defined(__Archis__ReprojectionClass__) */
