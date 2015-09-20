//
//  Parser.cpp
//  Archis
//
//  Created by Pratheba Selvaraju on 8/23/15.
//  Copyright (c) 2015 Pratheba Selvaraju. All rights reserved.
//

#include "Parser.h"

std::vector<std::string>Parser::SceneComponents;

Parser::Parser() {
    SceneComponents.push_back("Camera");
    SceneComponents.push_back("Film");
    SceneComponents.push_back("LightGroup");
    SceneComponents.push_back("LightSource");
}

void Parser::ParseLuxRenderScene(const std::string& luxRenderFileName, CameraSystemClass& cameraSystemClass, LightSystemClass& lightSystemClass) {
    
    CameraEntityClass* camEntity = new CameraEntityClass();
    
    EntityClass::EntityLookAtVector* lookAtVector = new EntityClass::EntityLookAtVector();
    
    lookAtVector->_eyeVector = Eigen::Vector3d(1.460001, 1.980000, 5.657866);
    lookAtVector->_directionVector = Eigen::Vector3d(1.414863, 1.671967, 4.707562);
    lookAtVector->_upVector = Eigen::Vector3d(0.063852, 0.948439, -0.310461);
    
//    lookAtVector->_eyeVector = Eigen::Vector3d(0.000000, 0.000000, 5.000000);
//    lookAtVector->_directionVector = Eigen::Vector3d(0.000000, 0.000000, 4.000000);
//    lookAtVector->_upVector = Eigen::Vector3d(0.000000, 1.000000, 0.000000);
    
    camEntity->SetEntityLookAtVector(*lookAtVector);
    
    camEntity->SetFieldOfView(49.134342077604479);
    camEntity->SetFocalLength(Point2D<double>(0.035,0.035));
//    //camEntity->SetPixelWidth(1);
//    //camEntity->SetPixelHeight(1);
    camEntity->SetPosition(Eigen::Vector3d(1.460001, 1.980000, 5.657866));
    camEntity->SetRotation(Point3D<double>(-0.3157657980918884, 0.02314339205622673, -0.07478136569261551));
    
//    camEntity->SetFieldOfView(49.134342077604479);
//   // camEntity->SetPixelWidth(1);
//   // camEntity->SetPixelHeight(1);
//    camEntity->SetPosition(Eigen::Vector3d(0.000000, 0.000000, 5.000000));
//    camEntity->SetRotation(Point3D<double>(0.0, -0.0, 0.0));
    
    const ImageSystemClass& imageSystemClass = ImageSystemClass::GetInstance();
    long width = imageSystemClass.GetCurrentImageWidth();
    long height = imageSystemClass.GetCurrentImageHeight();
    camEntity->SetPrincipalPoint(Point2D<double>(width/2,height/2));
    
    cameraSystemClass.SetCurrentCamera(*camEntity);
    
    
    //-----------------------
    
    LightEntityClass* lightEntity = new LightEntityClass();
    lightEntity->SetLightGain(100);
    lightEntity->SetLightPower(0);
    lightEntity->SetLightEfficacy(0);
    lightEntity->SetSpotLightInnerConeAngle(0);
    lightEntity->SetSpotLightOuterConeAngle(22.5);
    Eigen::Matrix4d lightTransMatrix ;
    
    lightTransMatrix.col(0) = Eigen::Vector4d(0.006568121723831, 0.002261585788801, -0.007193399593234, 0.000000000000000);
    lightTransMatrix.col(1) = Eigen::Vector4d(-0.006568201817572, 0.006402075756341, -0.003984473180026, 0.000000000000000);
    lightTransMatrix.col(2) = Eigen::Vector4d(0.029632598161697, 0.058733433485031, 0.045522455126047, 0.000000000000000);
    lightTransMatrix.col(3) = Eigen::Vector4d(1.732566714286804, 1.937185287475586, 2.554601192474365, 1.000000000000000);
    lightEntity->SetLightTransformation(lightTransMatrix);
    
    lightSystemClass.SetCurrentLight(*lightEntity);
}

void Parser::ParsePLYFile(const std::string& plyFileName, GeometrySystemClass& geometryClass) {
    
    GeometryEntityClass* geomEntity = new GeometryEntityClass();
    geomEntity->SetGeometryType(PLANE);
    
    std::vector<Point3D<double>> vertices;
    
    vertices.push_back(Point3D<double>(0,0,2));
    vertices.push_back(Point3D<double>(2,0,2));
    vertices.push_back(Point3D<double>(2,2,2));
    vertices.push_back(Point3D<double>(0,2,2));
    
//    vertices.push_back(Point3D<double>(-1,-1,0));
//    vertices.push_back(Point3D<double>(1,-1,0));
//    vertices.push_back(Point3D<double>(1,1,0));
//    vertices.push_back(Point3D<double>(-1,1,0));
    
    std::vector<Eigen::Vector3d> vertexNormals;
    vertexNormals.push_back(Eigen::Vector3d(0,0,1));
    vertexNormals.push_back(Eigen::Vector3d(0,0,1));
    vertexNormals.push_back(Eigen::Vector3d(0,0,1));
    vertexNormals.push_back(Eigen::Vector3d(0,0,1));
    
    geomEntity->SetVertices(vertices);
    geomEntity->SetVertexNormal(vertexNormals);
    
    geometryClass.SetCurrentGeometry(*geomEntity);
}

void Parser::ParseImageFile(const std::string& imageFileName, ImageSystemClass& imageSystemClass) {
    //std::string fileName = "/Users/prathebaselvaraju/4-Projects/Archis/Input/light-xyz-gray/light-xyz-gray.exr";
    std::string fileName = "/Users/prathebaselvaraju/4-Projects/Archis/Input/light-xyz-gray-intensity-100/light-xyz-gray-intensity-100.exr";
    imageSystemClass = ImageSystemClass::GetInstance(fileName);
}

