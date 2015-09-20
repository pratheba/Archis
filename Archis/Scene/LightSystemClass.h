//
//  LightSystemClass.h
//  Archis
//
//  Created by Pratheba Selvaraju on 8/27/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef __Archis__LightSystemClass__
#define __Archis__LightSystemClass__

#include <iostream>
#include "LightEntityClass.h"

class LightSystemClass {
public:
    
    static LightSystemClass& GetInstance();
    static void ReleaseInstance();
    
    const LightEntityClass& GetCurrentLight();
    void SetCurrentLight(const LightEntityClass& currentCamera);
    
    double GetCurrentLightIntensity();
    
private:
    LightSystemClass();
    ~LightSystemClass(){}
    
    static LightSystemClass* _lightSystem;
    static LightEntityClass* _currentLight;
};

#endif /* defined(__Archis__LightSystemClass__) */
