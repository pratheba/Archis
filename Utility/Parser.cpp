//
//  Parser.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 8/23/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "Parser.h"
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

Parser::Parser() {}

bool Parser::CheckIfEndOfModule(std::string& line) {
    boost::algorithm::trim(line);
    std::stringstream token(line);
    
    if (line[0] == '#') {
        std::string stringAfterHash = line.substr(1);
        boost::algorithm::trim(stringAfterHash);
        if(boost::iequals(stringAfterHash, "endModule"))
            return true;
    }
    return false;
}

void Parser::ParseImageModule(std::ifstream& sceneFileName,ImageSystemClass& imageSystemClass) {
    std::string line;
    std::string key, value;
    while (std::getline(sceneFileName,line)) {
        boost::algorithm::trim(line);
        if (line.empty()) {
            continue;
        }
        if(CheckIfEndOfModule(line))
            break;
        
        std::stringstream token(line);
        while(token >> std::skipws && std::getline(token, key, '=') && (token >> std::skipws >> value)) {
            std::cout << key << "=" << value << std::endl;
            boost::algorithm::trim(key);
            boost::algorithm::trim(value);
        }
        // We do this because when we first create a dummy instance
        imageSystemClass.ReleaseInstance();
        imageSystemClass = ImageSystemClass::GetInstance(value);
    }
}

void Parser::ParseCameraModule(std::ifstream& sceneFileName, CameraSystemClass& cameraSystemClass) {
    CameraEntityClass* camEntity = new CameraEntityClass();
    EntityClass::EntityLookAtVector* lookAtVector = new EntityClass::EntityLookAtVector();
    
    std::string line, key, value;
    std::vector<double> vectorValues;
    
    while (std::getline(sceneFileName,line)) {
        boost::algorithm::trim(line);
        if (line.empty()) {
            continue;
        }
        if(CheckIfEndOfModule(line)) {
            break;
        }
        
        if (line[0] == '#') {
            continue;
        }
        
        std::stringstream token(line);
        while(std::getline(token, key, '=')) {
            boost::algorithm::trim(key);
            
            vectorValues.clear();
            while(std::getline(token, value, ',')) {
                boost::algorithm::trim(value);
                vectorValues.push_back(atof(value.c_str()));
            }
            if (vectorValues.size() <= 0) {
                std::cout << "Field Of View does not contain all values to process." << std::endl;
                return;
            }
            
            if(boost::iequals(key, "eyeVector")) {
                if (vectorValues.size() != 3) {
                    std::cout << "Eye Vector does not contain all values to process." << std::endl;
                    return;
                }
                lookAtVector->_eyeVector = Eigen::Map<Eigen::Vector3d>(&vectorValues[0], vectorValues.size());
            }
            else if(boost::iequals(key,"directionVector")) {
                if (vectorValues.size() != 3) {
                    std::cout << "Eye Vector does not contain all values to process." << std::endl;
                    return;
                }
                lookAtVector->_directionVector = Eigen::Map<Eigen::Vector3d>(&vectorValues[0], vectorValues.size());
            }
            else if(boost::iequals(key,"upVector")) {
                if (vectorValues.size() != 3) {
                    std::cout << "Eye Vector does not contain all values to process." << std::endl;
                    return;
                }
                lookAtVector->_upVector = Eigen::Map<Eigen::Vector3d>(&vectorValues[0], vectorValues.size());
            }
            else if(boost::iequals(key, "FocalLength")) {
                camEntity->SetFocalLength(Point2D<double>(vectorValues[0],vectorValues[0]));
            }
            else if(boost::iequals(key,"FieldOfView")) {
                camEntity->SetFieldOfView(vectorValues[0]);
            }
            else if(boost::iequals(key,"pixelWidth")) {
                camEntity->SetPixelWidth(vectorValues[0]);
            }
            else if(boost::iequals(key,"pixelHeight")) {
                camEntity->SetPixelHeight(vectorValues[0]);
            }
            else if(boost::iequals(key,"rotationEuler")) {
                if (vectorValues.size() != 3) {
                    std::cout << "Rotation euler angle does not contain all values to process." << std::endl;
                    return;
                }
                camEntity->SetRotation(Point3D<double>(vectorValues));
            }
        }
    }
    
    camEntity->SetEntityLookAtVector(*lookAtVector);
    
    const ImageSystemClass& imageSystemClass = ImageSystemClass::GetInstance();
    long width = imageSystemClass.GetCurrentImageWidth();
    long height = imageSystemClass.GetCurrentImageHeight();
    camEntity->SetPrincipalPoint(Point2D<double>(height/2,width/2));
    
    cameraSystemClass.SetCurrentCamera(*camEntity);
    //delete camEntity;
    delete lookAtVector;
}

void Parser::ParseLightModule(std::ifstream& sceneFileName,LightSystemClass& lightSystemClass) {
    LightEntityClass* lightEntity = new LightEntityClass();
    
    std::string line, key, value;
    std::vector<double> vectorValues;
    
    while (std::getline(sceneFileName,line)) {
        boost::algorithm::trim(line);
        if (line.empty()) {
            continue;
        }
        if(CheckIfEndOfModule(line)) {
            break;
        }
        
        if (line[0] == '#') {
            continue;
        }
        
        std::stringstream token(line);
        while(std::getline(token, key, '=')) {
            boost::algorithm::trim(key);
            
            vectorValues.clear();
            while(std::getline(token, value, ',')) {
                boost::algorithm::trim(value);
                vectorValues.push_back(atof(value.c_str()));
            }
            
            if(vectorValues.size() <= 0) {
                std::cout << "Input file error!!! " << key << " does not contain all values" << std::endl;
                continue;
            }
            
            if(boost::iequals(key,"gain")) {
                lightEntity->SetLightGain(vectorValues[0]);
            }
            else if(boost::iequals(key,"Power")) {
                lightEntity->SetLightPower(vectorValues[0]);
            }
            else if(boost::iequals(key,"Efficacy")) {
                lightEntity->SetLightEfficacy(vectorValues[0]);
            }
            else if(boost::iequals(key,"Gamma")) {
                lightEntity->SetGammaCorrection(vectorValues[0]);
            }
            else if(boost::iequals(key,"OuterConeAngle")) {
                lightEntity->SetSpotLightOuterConeAngle(vectorValues[0]);
            }
            else if(boost::iequals(key,"InnerConeAngle")) {
                lightEntity->SetSpotLightInnerConeAngle(vectorValues[0]);
            }
            else if(boost::iequals(key, "lightTransMatrix")) {
                
                if (vectorValues.size() != 16) {
                    std::cout << "Eye Vector does not contain all values to process." << std::endl;
                    return;
                }
                
                Eigen::VectorXd vector = Eigen::Map<Eigen::VectorXd>(&vectorValues[0], vectorValues.size());
                Eigen::Matrix4d lightTransMatrix(vector.data());
                lightTransMatrix.transpose();
                lightEntity->SetLightTransformation(lightTransMatrix);
                
            }
        }
    }
    // We delete the light entity when we delete the system class
    lightSystemClass.SetCurrentLight(*lightEntity);
}

void Parser::ParseGeometryModule(std::ifstream& sceneFileName, GeometrySystemClass& geometrySystemClass) {
    GeometryEntityClass* geomEntity = new GeometryEntityClass();
    
    std::string line, key, value;
    std::vector<double> vectorValues;
    
    while (std::getline(sceneFileName,line)) {
        boost::algorithm::trim(line);
        if (line.empty()) {
            continue;
        }
        if(CheckIfEndOfModule(line)) {
            break;
        }
        
        if (line[0] == '#') {
            continue;
        }
        
        std::stringstream token(line);
        while(std::getline(token, key, '=')) {
            boost::algorithm::trim(key);
            
            vectorValues.clear();
            while(std::getline(token, value, ',')) {
                boost::algorithm::trim(value);
                vectorValues.push_back(atof(value.c_str()));
            }
            
            if(vectorValues.size() <= 0) {
                std::cout << "Input file error!!! " << key << " does not contain all values" << std::endl;
                continue;
            }
            
            if(boost::iequals(key,"Type")) {
                geomEntity->SetGeometryType(value);
            }
            else if(boost::iequals(key,"NumberOfVertices")) {
                geomEntity->SetNumberOfVertices(vectorValues[0]);
            }
            else if(boost::iequals(key,"vertices")) {
                std::vector<Point3D<double>> vertices;
                for(int index=0; index < vectorValues.size(); ) {
                    vertices.push_back(Point3D<double>(vectorValues[index],vectorValues[index+1],vectorValues[index+2]));
                    index = index+3;
                }
                geomEntity->SetVertices(vertices);
            }
            else if(boost::iequals(key,"normals")) {
                std::vector<Eigen::Vector3d> normals;
                for(int index=0; index < vectorValues.size(); ) {
                    normals.push_back(Eigen::Vector3d(vectorValues[index],vectorValues[index+1],vectorValues[index+2]));
                    index = index+3;
                }
                geomEntity->SetVertexNormal(normals);
            }
        }
    }
    geometrySystemClass.SetCurrentGeometry(*geomEntity);
}

void Parser::ParseSceneFile(const std::string& fileName, SystemClass& systemClass) {
    std::string line;
    std::string key, value;
    std::ifstream sceneFileName(fileName);
    
    while (std::getline(sceneFileName,line)) {
        boost::algorithm::trim(line);
        if (line.empty()) {
            continue;
        }
        std::stringstream token(line);
        std::string tokenString = token.str();
        if (tokenString[0] == '#') {
            continue;
        }

        while(token >> std::skipws && std::getline(token, key, '=') && (token >> std::skipws >> value)) {
            std::cout << key << "=" << value << std::endl;
            boost::algorithm::trim(key);
            boost::algorithm::trim(value);
        }
        
        if(boost::iequals(key, "Module")) {
            
            if(boost::iequals(value, "Image")) {
                ParseImageModule(sceneFileName, systemClass._imageSystemClass);
            }
            else if(boost::iequals(value, "Camera")) {
                ParseCameraModule(sceneFileName, systemClass._cameraSystemClass);
            }
            else if(boost::iequals(value, "Light")) {
                ParseLightModule(sceneFileName, systemClass._lightSystemClass);
            }
            else if(boost::iequals(value, "Geometry")) {
                ParseGeometryModule(sceneFileName, systemClass._geometrySystemClass);
            }
        }
    }
}

void Parser::ParseResourceFile(const std::string& resourceFileName, SystemClass& systemClass) {
     ParseSceneFile(resourceFileName, systemClass);
}

std::vector<std::string> Parser::ParseResourceFileAndFilesToBeProcessed(const std::string& resourceFileName) {
    std::vector<std::string> resourceFiles;
    int numberOfFilesToRead = -1;
    std::string line;
    std::string key, value;
    
    boost::filesystem::path full_path(boost::filesystem::current_path());
    std::string path = std::string(full_path.c_str()) + "/../../Resource/" + resourceFileName;
    std::ifstream resourceFile(path);
    
    while (std::getline(resourceFile,line)) {
        boost::algorithm::trim(line);
        if (line.length() <= 0) {
            continue;
        }
        std::stringstream token(line);
        std::string tokenString = token.str();
        if (tokenString[0] == '#') {
            continue;
        }
        
        while(token >> std::skipws && std::getline(token, key, '=') && (token >> std::skipws >> value)) {
            boost::algorithm::trim(key);
            boost::algorithm::trim(value);
        }
        if(boost::iequals(key, "NumberOfFiles"))
            numberOfFilesToRead = atoi(value.c_str());
        
        
        if (numberOfFilesToRead > 0) {
            while (std::getline(resourceFile,line)) {
                boost::algorithm::trim(line);
                if (line.length() <= 0) {
                    continue;
                }
                std::stringstream token(line);
                std::string tokenString = token.str();
                if (tokenString[0] == '#') {
                    continue;
                }
                std::string path = std::string(full_path.c_str()) + "/../../Resource/"+ line;
                resourceFiles.push_back(path);
                if (resourceFiles.size() == numberOfFilesToRead) {
                    break;
                }
            }
            break;
        }
    }
    return resourceFiles;
}
