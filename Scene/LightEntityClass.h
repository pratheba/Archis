//
//  LightEntityClass.h
//  Archis
//
//  Created by Pratheba Selvaraju on 8/27/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef __Archis__LightEntityClass__
#define __Archis__LightEntityClass__

#include <iostream>
#include "EntityClass.h"

enum LIGHTTYPE {
    POINT  = 0,
    SPOT = 1,
    DIRECTIONAL = 2
};

class LightEntityClass : public EntityClass {
public:
    LightEntityClass();
    ~LightEntityClass(){}
    
    LIGHTTYPE GetLightType();
    void SetLightType(const LIGHTTYPE& lightType);
    
    double GetLightIntensity() const;
    void SetLightIntensity(const double& lightIntensity);
    
    double GetLightgain()const;
    void SetLightGain(const double& lightGain);
    
    double GetLightPower()const;
    void SetLightPower(const double& lightPower);
    
    double GetLightEfficacy() const;
    void SetLightEfficacy(const double& lightEfficacy);
    
    double GetGammaCorrection() const;
    void SetGammaCorrection(const double& gammaCorrection);
    
    double GetSpotLightInnerConeAngle()const;
    void SetSpotLightInnerConeAngle(const double& innerconeAngle);
    
    double GetSpotLightOuterConeAngle()const;
    void SetSpotLightOuterConeAngle(const double& outerconeAngle);
    
private:
    LIGHTTYPE _lightType;
    double _lightGain;
    double _lightPower;
    double _lightEfficacy;
    double _gammaCorrection;
    double _innerconeAngle;
    double _outerconeAngle;
};


#endif /* defined(__Archis__LightEntityClass__) */
