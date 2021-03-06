//
//  DBSCAN_Class.h
//  Archis
//
//  Created by Pratheba Selvaraju on 8/27/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef __Archis__DBSCAN_Class__
#define __Archis__DBSCAN_Class__

#include <iostream>
#include "UtilityClass.h"

class DBSCAN_Class{
public:
    DBSCAN_Class();
    ~DBSCAN_Class();
    
    void SetEpsilon(const double& epsilon);
    void SetMinPoints(const int& minPoints);
    
    std::vector<VecOf2dIntPoints> GetClustersOfDataPoints(const VecOf2dIntPoints& dataPoints);
    VecOf2dDoublePoints GetCentroidOfClusteredPoints(std::vector<VecOf2dIntPoints>& centroidPoints);
    
private:
    
    enum POINT_TYPE {
        CLUSTER,
        NOISE
    };
    
    double  _epsilon;
    int     _minPoints;
    
    VecOf2dIntPoints        _data;
    VecOfBool               _isVisited;
    VecOfBool               _isMemberOfCluster;
    std::vector<VecOfInt>   _neighbourPoints;
    std::vector<POINT_TYPE> _dataPointType;
    
    Point2D<int>            _corePoint;
    VecOfInt                _corePointNeighbours;
    std::vector<VecOf2dIntPoints> _clusterPoints;
    
    void Initialize(const VecOf2dIntPoints& dataPoints);
    void Reset();
    
    void GetAllNeighbouringPointsWithinRadius();
    void ClusterDataPoints();
    void ExpandCluster(const int clusterIndex);
 
};

#endif /* defined(__Archis__DBSCAN_Class__) */
