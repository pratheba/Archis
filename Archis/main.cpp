//
//  main.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 9/15/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "LightParameters/SpotLightParameterEstimationClass.h"

int main(int argc, const char * argv[]) {
    //    if (argc < 2) {
    //        std::cout << "No input arguments. Returning ... " << std::endl;
    //        return -1;
    //    }
    
    INPUTPARAM inputParameters("imageFile","luxfile","plyfile");
    std::string fileName = "/Users/prathebaselvaraju/4-Projects/Archis/Archis/Input/Light-gray-intensity100-gamma1/light-gray-intensity100-gamma1.exr";
    
    SpotLightParameterEstimationClass& spotLightParamEstClass = SpotLightParameterEstimationClass::GetInstance(fileName);
    spotLightParamEstClass.GetSpotLightExponentFromImage(inputParameters);
    
    return 0;
    
};
