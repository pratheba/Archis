//
//  DBSCAN_Class.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 8/27/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "DBSCAN_Class.h"


DBSCAN_Class::DBSCAN_Class():_epsilon(DBL_MAX),_minPoints(0) {
    _data.clear();
}

DBSCAN_Class::~DBSCAN_Class() {
}

void DBSCAN_Class::SetEpsilon(const double &epsilon) {
    _epsilon = epsilon;
}

void DBSCAN_Class::SetMinPoints(const int &minPoints) {
    _minPoints = minPoints;
}

void DBSCAN_Class::Initialize(const VecOf2dIntPoints& dataPoints) {
    _data        =   dataPoints;
    Reset();
}

void DBSCAN_Class::Reset() {
    size_t dataSize     =   _data.size();
    _neighbourPoints    =   std::vector<VecOfInt>(dataSize, VecOfInt());
    _isVisited          =   VecOfBool(dataSize, false);
    _isMemberOfCluster  =   VecOfBool(dataSize, false);
    _dataPointType      =   std::vector<POINT_TYPE>(dataSize, CLUSTER);
}

void DBSCAN_Class::GetAllNeighbouringPointsWithinRadius() {
    
    int dataSize =   (int)_data.size();
    
    for (int refPoint = 0; refPoint < dataSize; refPoint++) {
        for(int currPoint = 0; currPoint < refPoint; currPoint++) {
            double distance = sqrt(pow(_data[refPoint].x - _data[currPoint].x, 2) +
                                   pow(_data[refPoint].y - _data[currPoint].y, 2));
            
            // Include intensity difference
            //if ((distance <= epsilon)  && (intensityDifference <= 5))
            
            if (distance <= _epsilon) {
                _neighbourPoints[currPoint].push_back(refPoint);
                _neighbourPoints[refPoint].push_back(currPoint);
            }
        }
    }
}

std::vector<VecOf2dIntPoints> DBSCAN_Class::GetClustersOfDataPoints(const VecOf2dIntPoints& dataPoints) {
    Initialize(dataPoints);
    GetAllNeighbouringPointsWithinRadius();
    ClusterDataPoints();
    
    return _clusterPoints;
}

void DBSCAN_Class::ClusterDataPoints() {
    
    int clusterIndex = 0;
    
    for (int index = 0; index < _data.size(); ++index) {
        if (_isVisited[index])
            continue;
        
        _isVisited[index]   =   true;
        _corePoint          =   _data[index];
        _corePointNeighbours    = _neighbourPoints.at(index);
        
        if(_corePointNeighbours.size() < _minPoints)
            _dataPointType[index] = NOISE;
        else {
            std::vector<Point2D<int>> cluster;
            cluster.push_back(_corePoint);
            _clusterPoints.push_back(cluster);
            
            ExpandCluster(clusterIndex);
            clusterIndex++;
        }
    }
}

void DBSCAN_Class::ExpandCluster(const int clusterIndex) {
    
    int neighbourPointSize = (int)_corePointNeighbours.size();
    for (int index = 0; index < neighbourPointSize; ++index) {
        
        if (!_isVisited[_corePointNeighbours[index]]) {
            _isVisited[_corePointNeighbours[index]] = true;
            
            std::vector<int> neighbourPoints = _neighbourPoints.at(_corePointNeighbours[index]);
            if (_corePointNeighbours.size() >= _minPoints) {
                _corePointNeighbours.insert(_corePointNeighbours.end(), neighbourPoints.begin(), neighbourPoints.end());
                neighbourPointSize = (int)_corePointNeighbours.size();
            }
        }
        
        if (!_isMemberOfCluster[_corePointNeighbours[index]] && (_dataPointType[_corePointNeighbours[index]] != NOISE)) {
            _isMemberOfCluster[_corePointNeighbours[index]] = true;
            _clusterPoints[clusterIndex].push_back(_data[_corePointNeighbours[index]]);
        }
    }
}

VecOf2dDoublePoints DBSCAN_Class::GetCentroidOfClusteredPoints(std::vector<VecOf2dIntPoints>& clusterPoints) {
    
    VecOf2dDoublePoints centroidPoints;
    centroidPoints.clear();
    
    if (clusterPoints.size() == 0) {
        return centroidPoints;
    }
    
    int sumx = 0, sumy = 0;
    for (int cluster = 0; cluster < clusterPoints.size(); ++cluster) {
        sumx = 0;
        sumy = 0;
        for (int index = 0; index < clusterPoints[cluster].size(); index++) {
            sumx += clusterPoints[cluster][index].x;
            sumy += clusterPoints[cluster][index].y;
        }
        Point2D<double> centroid = Point2D<double>(sumy/clusterPoints[cluster].size(), sumx/clusterPoints[cluster].size());
        centroidPoints.push_back(centroid);
    }
    return centroidPoints;
}

