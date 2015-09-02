//
//  SpotLightFallOFFIntensityCalculator.h
//  Archis
//
//  Created by Pratheba Selvaraju on 8/27/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef __Archis__SpotLightFallOFFIntensityCalculator__
#define __Archis__SpotLightFallOFFIntensityCalculator__

#include <iostream>
#include "../Utility/UtilityClass.h"

class SpotLightFallOFFIntensityCalculator {
public:
    SpotLightFallOFFIntensityCalculator();
    ~SpotLightFallOFFIntensityCalculator();

    void GetLightFallOffPointsfromCorePoints(const Array2D<Rgba> inputImage_, const Point2D<int> corePoint_);
    
private:
    VecOf2dIntPoints   fallOffPoints;
    void Initialize(const Array2D<Rgba>& inputImage_, const Point2D<int>& corePoint_);
    void GetLightFallOffPointsfromCorePoints_UsingBlocksOfPixels(const Array2D<Rgba>& inputImage_, const Point2D<int>& corePoint_);
    void GetLightFallOffPointsfromCorePoints_UsingConeSector(const Array2D<Rgba>& inputImage_, const Point2D<int>& corePoint_);
    void GetLightFallOffPointsfromCorePoints_UsingTableMapOfSectorsAndSegment(const Array2D<Rgba>& inputImage_, const Point2D<int>&corePoint_);
    void GetLightFallOffPointsfromCorePoints_UsingGradientEstimation(const Array2D<Rgba>& inputImage_, const Point2D<int>& corePoint_);
};

#endif /* defined(__Archis__SpotLightFallOFFIntensityCalculator__) */
