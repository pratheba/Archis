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
#include <set>
#include "../Utility/UtilityClass.h"

typedef std::vector<std::vector<std::vector<Point2D<int>>>> SectorsInImage;
typedef std::vector<std::vector<Point2D<int>>> SegmentInEachSector;
typedef std::vector<std::vector<std::vector<Rgba>>> IntensityInImageSectors;
typedef std::vector<std::vector<Rgba>> IntensitySegmentInEachSector;

class SpotLightFallOFFIntensityCalculator {
public:
    SpotLightFallOFFIntensityCalculator();
    ~SpotLightFallOFFIntensityCalculator();

    void GetLightFallOffPointsfromCorePoints(const Array2D<Rgba>& inputImage_, const Point2D<int> corePoint_);
    
private:
    VecOf2dIntPoints   fallOffPoints;
    void Initialize(const Array2D<Rgba>& inputImage_, const Point2D<int>& corePoint_);
    void GetLightFallOffPointsfromCorePoints_UsingBlocksOfPixels(const Array2D<Rgba>& inputImage_, const Point2D<int>& corePoint_);
    void GetLightFallOffPointsfromCorePoints_UsingConeSector(const Array2D<Rgba>& inputImage_, const Point2D<int>& corePoint_);
    void GetLightFallOffPointsfromCorePoints_UsingTableMapOfSectorsAndSegment(const Array2D<Rgba>& inputImage_, const Point2D<int>&corePoint_);
    void GetLightFallOffPointsfromCorePoints_UsingGradientEstimation(const Array2D<Rgba>& inputImage_, const Point2D<int>& corePoint_);
    // From 3D information applied on 2D
    void GetLightFallOffPointsfromCorePoints_UsingAngleEstimationFromLightPosition(const Array2D<Rgba>& inputImage_, const Point2D<int>& corePoint_);
    int GetNumberOfRadiusSegments(const Array2D<Rgba>& inputImage_, const Point2D<int>& corePoint_);
    void InitializeSectorsOfImage(const Array2D<Rgba>& inputImage_, const Point2D<int>&corePoint_);
    void GetGradientDifferenceForEachSectorSegments(const int& maxNumberOfRadiusSegment);
    
    UtilityClass* utilityClass;
    
    SectorsInImage Sector;
    IntensityInImageSectors SectorIntensity;
};



#endif /* defined(__Archis__SpotLightFallOFFIntensityCalculator__) */
