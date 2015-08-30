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
    
    double GetLightgain()const;
    void SetLightGain(const double& lightGain);
    
    double GetLightPower()const;
    void SetLightPower(const double& lightPower);
    
    double GetSpotLightConeAngle()const;
    void SetSpotLightConeAngle(const double& coneAngle);
    
private:
    LIGHTTYPE _lightType;
    double _lightGain;
    double _lightPower;
    double _coneAngle;
};


#endif /* defined(__Archis__LightEntityClass__) */
