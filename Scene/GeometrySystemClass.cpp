//
//  GeometrySystemClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 8/27/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "GeometrySystemClass.h"

GeometrySystemClass* GeometrySystemClass::_geometrySystem = nullptr;
GeometryEntityClass* GeometrySystemClass::_currentGeometry = nullptr;

GeometrySystemClass& GeometrySystemClass::GetInstance() {
    if (_geometrySystem == nullptr) {
        _geometrySystem = new GeometrySystemClass();
    }
    return *_geometrySystem;
}

void GeometrySystemClass::ReleaseInstance() {
    if (_geometrySystem != nullptr) {
        if (_currentGeometry != nullptr) {
            delete _currentGeometry;
            _currentGeometry = nullptr;
        }
        delete _geometrySystem;
        _geometrySystem = nullptr;
    }
}

GeometrySystemClass::GeometrySystemClass() {
    if (_currentGeometry == nullptr) {
        _currentGeometry = new GeometryEntityClass();
    }
}

const GeometryEntityClass& GeometrySystemClass::GetCurrentGeometry() const{
    return *_currentGeometry;
}

void GeometrySystemClass::SetCurrentGeometry(const GeometryEntityClass& currentGeometry) const{
    *_currentGeometry = currentGeometry;
}