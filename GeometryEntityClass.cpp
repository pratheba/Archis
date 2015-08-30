//
//  GeometryEntityClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 8/27/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "GeometryEntityClass.h"

GeometryEntityClass::GeometryEntityClass() {
    _geometryType = NONE;
    _vertices.clear();
    _vertexNormals.clear();
    _faceNormals.clear();
}

GeometryEntityClass::~GeometryEntityClass() {
    
}

GEOMETRYTYPE GeometryEntityClass::GetGeometryType() const {
    return _geometryType;
}
void GeometryEntityClass::SetGeometryType(const GEOMETRYTYPE& geometryType) {
    _geometryType = geometryType;
}


std::vector<Point3D<double>> GeometryEntityClass::GetVertices() const {
    return _vertices;
}

void GeometryEntityClass::SetVertices(const std::vector<Point3D<double>>& vertices) {
    _vertices = vertices;
}

std::vector<Eigen::Vector3d> GeometryEntityClass::GetVertexNormals() const {
    return _vertexNormals;
}
void GeometryEntityClass::SetVertexNormal(const std::vector<Eigen::Vector3d>& vertexNormals) {
    _vertexNormals = vertexNormals;
}

MaxMinCoord GeometryEntityClass::GetMaxMinOfGeometry() const{
    MaxMinCoord maxminCoord;
    for (int index=0; index < _vertices.size(); ++index) {
        if (_vertices[index].x > maxminCoord._maxX)
            maxminCoord._maxX = _vertices[index].x;
        if (_vertices[index].x < maxminCoord._minX)
            maxminCoord._minX = _vertices[index].x;
        if (_vertices[index].y > maxminCoord._maxY)
            maxminCoord._maxY = _vertices[index].y;
        if (_vertices[index].y < maxminCoord._maxY)
            maxminCoord._minY = _vertices[index].y;
        if (_vertices[index].y > maxminCoord._minY)
            maxminCoord._maxZ = _vertices[index].z;
        if (_vertices[index].z < maxminCoord._maxZ)
            maxminCoord._maxZ = _vertices[index].z;
    }
    
    return maxminCoord;
    
}
