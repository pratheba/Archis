//
//  LightEntityClass.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 8/27/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "LightEntityClass.h"

LightEntityClass::LightEntityClass() {
    
}

LIGHTTYPE LightEntityClass::GetLightType() {
    return _lightType;
}

void LightEntityClass::SetLightType(const LIGHTTYPE& lightType) {
    _lightType = lightType;
}

double LightEntityClass::GetLightgain() const {
    return _lightGain;
}

void LightEntityClass::SetLightGain(const double& lightGain) {
    _lightGain = lightGain;
}

double LightEntityClass::GetLightPower() const {
    return _lightPower;
}

void LightEntityClass::SetLightPower(const double& lightPower) {
    _lightPower = lightPower;
}

double LightEntityClass::GetSpotLightConeAngle() const{
    return _coneAngle;
}

void LightEntityClass::SetSpotLightConeAngle(const double& coneAngle) {
    _coneAngle = coneAngle;
}


