//
//  GeometrySystemClass.h
//  Archis
//
//  Created by Pratheba Selvaraju on 8/27/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef __Archis__GeometrySystemClass__
#define __Archis__GeometrySystemClass__

#include <iostream>
#include "GeometryEntityClass.h"

class GeometrySystemClass {
public:
    static GeometrySystemClass& GetInstance();
    static void ReleaseInstance();
    
    const GeometryEntityClass& GetCurrentGeometry()const;
    void SetCurrentGeometry(const GeometryEntityClass& currentGeometry)const;
        
private:
    GeometrySystemClass();
    ~GeometrySystemClass(){}
    
    static GeometrySystemClass* _geometrySystem;
    static GeometryEntityClass* _currentGeometry;
};

#endif /* defined(__Archis__GeometrySystemClass__) */
