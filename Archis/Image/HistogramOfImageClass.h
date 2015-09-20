//
//  HistogramOfImageClass.h
//  Archis
//
//  Created by Pratheba Selvaraju on 8/27/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef __Archis__HistogramOfImageClass__
#define __Archis__HistogramOfImageClass__

#include <iostream>
#include "UtilityClass.h"
#include <set>
#include <map>

class HistogramOfImageClass {
public:
    HistogramOfImageClass();
    ~HistogramOfImageClass();
    
    void SetNumOfBins(const int& numOfBins);
    int GetNumOfBins();
    
    double GetMaxIntensityOfImage(const Array2D<Rgba>& imagePixels);
    double GetMinIntensityOfImage(const Array2D<Rgba>& imagePixels);
    
    VecOfInt            GetHistogramOfImage(const Array2D<Rgba>& imagePixels);
    VecOf2dIntPoints    GetAllPixelswithinMaxIntensityRange(const Array2D<Rgba>& imagePixels);
    
private:

    int         _numOfBins;
    int         _binSize;
    
    Array2D<Rgba>* _imagePixels;
    VecOfInt    _histogram;
    
    double      _maxIntensity;
    double      _minIntensity;
    
    std::vector<VecOf2dIntPoints>   _pixelsOfHistogramBin;
    std::map<int,int> _pixelCountsForIntensityRange;
    std::multimap<int, VecOf2dIntPoints> _PixelsForIntensity;
    
    // Range - set
    // VecOfintensityOfPixel
    
    

    void Initialize();
    void Reset();
    int FindInterQuartilePixels();
    int FindFourthQuartilePixelCount();
    void CalculateBinSize();
    int GetMaxPixelCountToBeTakenInHighIntensityRange(const Array2D<Rgba>& imagePixels);
    VecOfInt GetHistogramOfImageWithMaxAndMinIntensity(const Array2D<Rgba>& imagePixels);
        
};


#endif /* defined(__Archis__HistogramOfImageClass__) */
