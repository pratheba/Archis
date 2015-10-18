//
//  InitializerClass.hpp
//  Archis
//
//  Created by Pratheba Selvaraju on 10/16/15.
//  Copyright © 2015 Pratheba Selvaraju. All rights reserved.
//

#ifndef InitializerClass_hpp
#define InitializerClass_hpp

#include <iostream>

class InitializerClass {
public:
    InitializerClass();
    ~InitializerClass();
    
    void ParseResouceFileAndRunSpotLightParameterEstimation(const std::string& resourceFileName);
    
};

#endif /* InitializerClass_hpp */
