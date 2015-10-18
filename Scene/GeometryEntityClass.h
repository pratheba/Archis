//
//  GeometryEntityClass.h
//  Archis
//
//  Created by Pratheba Selvaraju on 8/27/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef __Archis__GeometryEntityClass__
#define __Archis__GeometryEntityClass__

#include <iostream>
#include "EntityClass.h"


enum GEOMETRYTYPE {
    NONE = 0,
    PLANE  = 1,
    CUBE    = 2,
};

struct MaxMinCoord {
    double _maxX;
    double _maxY;
    double _maxZ;
    
    double _minX;
    double _minY;
    double _minZ;
    
    MaxMinCoord():_maxX(DBL_MIN),_maxY(DBL_MIN),_maxZ(DBL_MIN),_minX(DBL_MAX),_minY(DBL_MAX),_minZ(DBL_MAX){}
};

class GeometryEntityClass : public EntityClass {
public:
    GeometryEntityClass();
    ~GeometryEntityClass();
    
    GEOMETRYTYPE GetGeometryType() const;
    void SetGeometryType(const GEOMETRYTYPE& geometryType);
    void SetGeometryType(const std::string& geometryType);
    
    void SetNumberOfVertices(const int& numberOfVertices);
    
    std::vector<Point3D<double>> GetVertices() const;
    void SetVertices(const std::vector<Point3D<double>>& vertices);
    
    std::vector<Eigen::Vector3d> GetVertexNormals() const;
    void SetVertexNormal(const std::vector<Eigen::Vector3d>& vertexNormals);
    
    Point3D<double> GetAVertex() const;
    
    MaxMinCoord GetMaxMinOfGeometry()const;
    
private:
    GEOMETRYTYPE _geometryType;
    int _numOfVertices;
    std::vector<Point3D<double>> _vertices;
    std::vector<Eigen::Vector3d> _vertexNormals;
    std::vector<Eigen::Vector3d> _faceNormals;

};

#endif /* defined(__Archis__GeometryEntityClass__)*/
