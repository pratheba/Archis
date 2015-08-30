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
    camEntity->SetEntityLookAtVector(*lookAtVector);
    
    const CameraEntityClass& cam = cameraSystemClass.GetCurrentCamera();
    
    camEntity->SetFieldOfView(49.134342077604479);
    camEntity->SetPixelWidth(0.5);
    camEntity->SetPixelHeight(0.5);
    camEntity->SetPosition(Eigen::Vector3d(1.460001, 1.980000, 5.657866));
    camEntity->SetRotation(Point3D<double>(-0.3157657980918884, 0.02314339205622673, -0.07478136569261551));
    cam.SetPosition(Eigen::Vector3d(1.460001, 1.980000, 5.657866));
    
    const ImageSystemClass& imageSystemClass = ImageSystemClass::GetInstance();
    long width = imageSystemClass.GetCurrentImageWidth();
    long height = imageSystemClass.GetCurrentImageHeight();
    camEntity->SetPrincipalPoint(Point2D<double>(width/2,height/2));
    
    cameraSystemClass.SetCurrentCamera(*camEntity);
    
    //-----------------------
    
    LightEntityClass* lightEntity = new LightEntityClass();
    lightEntity->SetLightGain(100);
    lightEntity->SetLightPower(100);
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
    std::string fileName = "/Users/prathebaselvaraju/4-Projects/Archis/Input/light-xyz-gray/light-xyz-gray.exr";
    imageSystemClass = ImageSystemClass::GetInstance(fileName);
}

    /*
# Main Scene File
    
    Renderer "sampler"
    
    Sampler "metropolis"
    "float largemutationprob" [0.400000005960464]
    "bool noiseaware" ["false"]
    
    Accelerator "qbvh"
    
    SurfaceIntegrator "bidirectional"
    "integer eyedepth" [16]
    "integer lightdepth" [16]
    "integer lightraycount" [1]
    "string lightpathstrategy" ["auto"]
    "string lightstrategy" ["auto"]
    
    VolumeIntegrator "multi"
    
    PixelFilter "blackmanharris"
    "float xwidth" [2.000000000000000]
    "float ywidth" [2.000000000000000]
    
    LookAt 1.460001 1.980000 5.657866 1.414863 1.671967 4.707562 0.063852 0.948439 -0.310461
    
    Camera "perspective"
    "float fov" [49.134342077604479]
    "float screenwindow" [-1.000000000000000 1.000000000000000 -0.562500000000000 0.562500000000000]
    "bool autofocus" ["false"]
    "float shutteropen" [0.000000000000000]
    "float shutterclose" [0.041666666666667]
    
    Film "fleximage"
    "integer xresolution" [960]
    "integer yresolution" [540]
    "float gamma" [2.200000000000000]
    "float colorspace_white" [0.314275000000000 0.329411000000000]
    "float colorspace_red" [0.630000000000000 0.340000000000000]
    "float colorspace_green" [0.310000000000000 0.595000000000000]
    "float colorspace_blue" [0.155000000000000 0.070000000000000]
    "string filename" ["light-xyz.Scene.00001"]
    "bool write_resume_flm" ["false"]
    "bool restart_resume_flm" ["false"]
    "bool write_flm_direct" ["false"]
    "bool write_exr_halftype" ["false"]
    "bool write_exr_applyimaging" ["true"]
    "bool write_exr_ZBuf" ["false"]
    "string write_exr_compressiontype" ["PIZ (lossless)"]
    "string write_exr_zbuf_normalizationtype" ["None"]
    "bool write_exr" ["false"]
    "string write_exr_channels" ["RGB"]
    "bool write_png" ["true"]
    "string write_png_channels" ["RGB"]
    "bool write_png_16bit" ["false"]
    "bool write_tga" ["false"]
    "string write_tga_channels" ["RGB"]
    "string ldr_clamp_method" ["cut"]
    "integer displayinterval" [10]
    "integer writeinterval" [180]
    "integer flmwriteinterval" [900]
    "integer outlierrejection_k" [2]
    "integer tilecount" [0]
    "string tonemapkernel" ["autolinear"]
    
    WorldBegin
    
#Include "light-xyz/Scene/00001/LuxRender-Materials.lxm"
    
#Include "light-xyz/Scene/00001/LuxRender-Geometry.lxo"
    
    TransformBegin #  "Spot"
    
    Transform [0.006568121723831 0.002261585788801 -0.007193399593234 0.000000000000000 -0.006568201817572 0.006402075756341 -0.003984473180026 0.000000000000000 0.029632598161697 0.058733433485031 0.045522455126047 0.000000000000000 1.732566714286804 1.937185287475586 2.554601192474365 1.000000000000000]
    
    LightGroup "default"
    
    LightSource "spot"
    "float gain" [100.000000000000000]
    "float importance" [10.000000000000000]
    "color L" [1.00000000 1.00000000 1.00000000]
    "float power" [100.000000000000000]
    "float efficacy" [10.000000000000000]
    "point from" [0.000000000000000 0.000000000000000 0.000000000000000]
    "point to" [0.000000000000000 0.000000000000000 -1.000000000000000]
    "float coneangle" [22.500000626119540]
    "float conedeltaangle" [22.500000626119540]
    
    TransformEnd # ""
    WorldEnd
*/
    
