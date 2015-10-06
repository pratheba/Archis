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

double LightEntityClass::GetSpotLightInnerConeAngle() const{
    return _innerconeAngle;
}

void LightEntityClass::SetSpotLightInnerConeAngle(const double& innerconeAngle) {
    _innerconeAngle = innerconeAngle;
}

double LightEntityClass::GetSpotLightOuterConeAngle() const{
    return _outerconeAngle;
}

void LightEntityClass::SetSpotLightOuterConeAngle(const double& outerconeAngle) {
    _outerconeAngle = outerconeAngle;
}

double LightEntityClass::GetLightEfficacy() const{
    return _lightEfficacy;
}

void LightEntityClass::SetLightEfficacy(const double& lightEfficacy) {
    _lightEfficacy = lightEfficacy;
}

double LightEntityClass::GetGammaCorrection() const {
    return _gammaCorrection;
}

void LightEntityClass::SetGammaCorrection(const double& gammaCorrection) {
    _gammaCorrection = gammaCorrection;
}


