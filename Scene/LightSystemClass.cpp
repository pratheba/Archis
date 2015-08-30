//
//  LightSystemClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 8/27/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "LightSystemClass.h"

LightSystemClass* LightSystemClass::_lightSystem = nullptr;
LightEntityClass* LightSystemClass::_currentLight = nullptr;

LightSystemClass& LightSystemClass::GetInstance() {
    if (_lightSystem == nullptr) {
        _lightSystem = new LightSystemClass();
    }
    return *_lightSystem;
}

LightSystemClass::LightSystemClass() {
    if (_currentLight == nullptr) {
        _currentLight = new LightEntityClass();
    }
}

void LightSystemClass::ReleaseInstance() {
    if (_lightSystem != nullptr) {
        
        if (_currentLight != nullptr) {
            delete _currentLight;
            _currentLight = nullptr;
        }
        
        delete _lightSystem;
        _lightSystem = nullptr;
    }
}

const LightEntityClass& LightSystemClass::GetCurrentLight() {
    return *_currentLight;
}

void LightSystemClass::SetCurrentLight(const LightEntityClass& currentLight) {
    *_currentLight = currentLight;
}