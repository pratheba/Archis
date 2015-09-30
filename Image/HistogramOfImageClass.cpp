//
//  HistogramOfImageClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 8/27/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "HistogramOfImageClass.h"
#include <numeric>
#include <iterator>



HistogramOfImageClass::HistogramOfImageClass():_imagePixels(nullptr) {
    _numOfBins = 0;
    Reset();
}

HistogramOfImageClass::~HistogramOfImageClass() {
    if (_imagePixels != nullptr) {
        delete _imagePixels;
        _imagePixels = nullptr;
    }
}

void HistogramOfImageClass::SetNumOfBins(const int& numOfBins) {
    _numOfBins = numOfBins;
}

int HistogramOfImageClass::GetNumOfBins() {
    return _numOfBins;
}

void HistogramOfImageClass::Initialize() {
    Reset();
}

void HistogramOfImageClass::Reset() {
//    _histogram           =   VecOfInt(256,0);
//    _pixelsOfHistogramBin    =   std::vector<VecOf2dIntPoints>(256, VecOf2dIntPoints());
//    for (int index = 0; index < 256; ++index) {
//        _histogram.at(index) = 0;
//        _pixelsOfHistogramBin.at(index).clear();
//    }

    //_VecOfintensityOfPixels.clear();
    //_rangeOfPixelIntensity.clear();
    _histogram.clear();
    _pixelsOfHistogramBin.clear();
    _maxIntensity = DBL_MIN;
    _minIntensity = DBL_MAX;
}

double HistogramOfImageClass::GetMaxIntensityOfImage(const Array2D<Rgba>& imagePixels) {
    if (_maxIntensity == DBL_MIN)
        GetHistogramOfImageWithMaxAndMinIntensity(imagePixels);
    return _maxIntensity;
}

double HistogramOfImageClass::GetMinIntensityOfImage(const Array2D<Rgba>& imagePixels) {
    if (_minIntensity == DBL_MAX)
        GetHistogramOfImageWithMaxAndMinIntensity(imagePixels);
    return _minIntensity;
}

int HistogramOfImageClass::GetMaxPixelCountToBeTakenInHighIntensityRange(const Array2D<Rgba>& imagePixels) {
    int maxCountOfPixels = 0;
    int totalNumberOfPixels = (int)(imagePixels.height() * imagePixels.width());
    int sum_fourthQuartile = FindFourthQuartilePixelCount();
    
    int ratioOfHighintensityPixels = totalNumberOfPixels /sum_fourthQuartile;
    
    if (ratioOfHighintensityPixels > 100) {
        std::cout << "not enough pixels in high intensity range.." << std::endl;
        maxCountOfPixels = ratioOfHighintensityPixels;
    }
    else if(sum_fourthQuartile > 1000) {
        maxCountOfPixels = 1000;
    }
    else
        maxCountOfPixels = sum_fourthQuartile;
    
    return maxCountOfPixels;
}

VecOf2dIntPoints HistogramOfImageClass::GetAllPixelswithinMaxIntensityRange(const Array2D<Rgba>& imagePixels) {
    
    VecOf2dIntPoints pixelsWithMaxIntensityRange;
    pixelsWithMaxIntensityRange.clear();
   
    if (_pixelCountsForIntensityRange.size() == 0)
        GetHistogramOfImage(imagePixels);
    
    //_binSize = (int)(_pixelCountsForIntensityRange.size()/_numOfBins);
    
    int maxCountOfPixels = GetMaxPixelCountToBeTakenInHighIntensityRange(imagePixels);
    
    std::map<int,int>::reverse_iterator rev_iter = _pixelCountsForIntensityRange.rbegin();
    for (; rev_iter != _pixelCountsForIntensityRange.rend(); ++rev_iter) {
        pixelsWithMaxIntensityRange.insert(pixelsWithMaxIntensityRange.end(), _pixelsOfHistogramBin[(*rev_iter).first].begin(),_pixelsOfHistogramBin[(*rev_iter).first].end());
   
        // remove the hardcoded value and use the
        // 1. imageSize
        // 2. Histogram -> get local maxima of pixels and find the intensity of each to evaluate how big the illumination is out
        //                  of the whole image. This way we get a fair idea of how many pixel counts we can have
        // to evaluate the amount of pixels.
        
        if (pixelsWithMaxIntensityRange.size() > maxCountOfPixels) {
            break;
        }
    }
    return pixelsWithMaxIntensityRange;
}

int map_acc(int lhs, const std::pair<int, int> & rhs)
{
    return lhs + rhs.second;
}

int HistogramOfImageClass::FindFourthQuartilePixelCount() {
    int Q1 = (int)_pixelCountsForIntensityRange.size() / 4;
    int Q2 = (int)_pixelCountsForIntensityRange.size() / 2;
    int Q3 = (int) Q1 + Q2;
    
    std::map<int,int>::iterator it = _pixelCountsForIntensityRange.begin();
    auto start = std::next(it,Q3);

    int sum_fourthQuartile = std::accumulate(start, _pixelCountsForIntensityRange.end(), 0,map_acc);
    return sum_fourthQuartile;
}

int HistogramOfImageClass::FindInterQuartilePixels() {
    
    int Q1 = (int)_pixelCountsForIntensityRange.size() / 4;
    int Q2 = (int)_pixelCountsForIntensityRange.size() / 2;
    int Q3 = (int) Q1 + Q2;
   
    std::map<int,int>::iterator it = _pixelCountsForIntensityRange.begin();
    
    auto start = std::next(it,Q1);
    auto end = std::next(start,Q3);
 
    int sum_interQuartile = std::accumulate(start, end, 0,map_acc);
    return sum_interQuartile;
}

void HistogramOfImageClass::CalculateBinSize() {
    
    int sum_interQuartile = FindInterQuartilePixels();
    int tempBinPixelCount = sum_interQuartile, count = 0;

    while (tempBinPixelCount > 0 ) {
        tempBinPixelCount = (sum_interQuartile / std::pow(2, count));
        if(tempBinPixelCount > MINPIXELINBIN) {
            ++count;
        }
        else {
            _numOfBins =(int)(_pixelCountsForIntensityRange.size()/tempBinPixelCount);
            break;
        }
    }
}

VecOfInt HistogramOfImageClass::GetHistogramOfImageWithMaxAndMinIntensity(const Array2D<Rgba>& imagePixels) {
    Reset();
    
    int numRows = (int)imagePixels.height();
    int numCols = (int)imagePixels.width();
    
    if (numRows == 0 || numCols == 0) {
        return _histogram;
    }
    
    std::ofstream outputfile;
    outputfile.open("../../Output/imagePixelValue-Histogram.txt");

    std::set<int> rangeOfPixelIntensity = std::set<int>();
    for (int row = 0; row < numRows; ++row) {
        for (int col=0; col < numCols; ++col) {
            Rgba pixel = imagePixels[row][col];
            double averagePixelIntensityValue   =   (pixel.r + pixel.g + pixel.b)/3;
            int intensityValue = (int)((pixel.r + pixel.g + pixel.b)/3);
            
            outputfile << row << "," << col << " = " << averagePixelIntensityValue << std::endl;
            rangeOfPixelIntensity.insert(intensityValue);
            _pixelCountsForIntensityRange[intensityValue]++;
            
            if(_maxIntensity < intensityValue)
                _maxIntensity = intensityValue;
            if (_minIntensity > intensityValue)
                _minIntensity = intensityValue;
        }
    }
    std::cout << "maxIntensity = " << _maxIntensity << std::endl;
    
    outputfile.close();
    // Doubt : Should I use Map or Vector?
    // With map i allocate only requires pixels.
    // With vector i have empty resources which carry no pixel values.
    // Which to use when?
    
    _pixelsOfHistogramBin = std::vector<VecOf2dIntPoints>(_maxIntensity+1,VecOf2dIntPoints());
    
    for (int row = 0; row < numRows; ++row) {
        for (int col=0; col < numCols; ++col) {
            Rgba pixel = imagePixels[row][col];
            int intensityValue = (int)((pixel.r + pixel.g + pixel.b)/3);
       
            _pixelsOfHistogramBin[intensityValue].push_back(Point2D<int>(row, col));
          
        }
    }
    
    /* Uncomment if you want to draw a histogram, which makes use of bin size */
    //CalculateBinSize();
    return _histogram;
}

VecOfInt HistogramOfImageClass::GetHistogramOfImage(const Array2D<Rgba>& imagePixels) {
    return GetHistogramOfImageWithMaxAndMinIntensity(imagePixels);
}

