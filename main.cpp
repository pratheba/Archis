//
//  main.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 9/15/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "InitializerClass.hpp"

int main(int argc, const char * argv[]) {
    InitializerClass* initClass = new InitializerClass();
    initClass->ParseResouceFileAndRunSpotLightParameterEstimation("Resource.txt");
    
    return 0;
    
};
