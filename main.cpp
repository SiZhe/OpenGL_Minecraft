#include <iostream>
#include <format>
#include <cassert>
#include <memory>
#include <thread>
#include <vector>

#include "wrapper/CheckError.hpp"

#include "application/Application.hpp"
#include "application/camera/camera.hpp"
#include "application/camera/orthoCamera.hpp"
#include "application/camera/perspectiveCamera.hpp"
#include "application/camera/trackBallCameraControl.hpp"
#include "application/camera/gameCameraControl.hpp"
#include "application/camera/McGameCameraControl.hpp"
#include "application/AssimpLoader.hpp"

#include "glframework/Core.hpp"
#include "glframework/Shader.hpp"
#include "glframework/Texture.hpp"
#include "glframework/Scene.hpp"
#include "glframework/Geometry.hpp"
#include "glframework/material/Material.hpp"
#include "glframework/material/PhongMaterial.hpp"
#include "glframework/material/WhiteMaterial.hpp"
#include "glframework/material/DepthMaterial.hpp"
#include "glframework/material/OpacityMaskMaterial.hpp"
#include "glframework/material/ScreenMaterial.hpp"
#include "glframework/material/CubeMaterial.hpp"
#include "glframework/material/PhongEnvMaterial.hpp"
#include "glframework/material/SphereMaterial.hpp"
#include "glframework/material/mc/McCubeMaterial.hpp"
#include "glframework/material/mc/McPlantMaterial.hpp"
#include "glframework/material/mc/McEliminateMaterial.hpp"

#include "glframework/mesh/Mesh.hpp"
#include "glframework/mesh/InstancedMesh.hpp"
#include "glframework/mesh/mc/CubeMesh.hpp"
#include "glframework/mesh/mc/LeafMesh.hpp"
#include "glframework/mesh/mc/PlantMesh.hpp"
#include "glframework/mesh/mc/PlayerMesh.hpp"
#include "glframework/mesh/mc/EliminateMesh.hpp"
#include "glframework/mesh/mc/CrossMesh.hpp"

#include "glframework/renderer/Renderer.hpp"
#include "glframework/light/Light.hpp"
#include "glframework/light/AmbientLight.hpp"
#include "glframework/light/DirectionLight.hpp"
#include "glframework/light/SpotLight.hpp"

#include "glframework/framebuffer/FrameBuffer.hpp"

#include "minecraft/Painter.hpp"
#include "minecraft/CreateWorld.hpp"
#include "minecraft/EliminateAnima.hpp"
#include "minecraft/Bag.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "minecraft/Bag.hpp"
#include "minecraft/EquipSolt.hpp"

const auto& cameraControl = mcControl;
static int WIDTH = 850;
static int HEIGHT = 850;

//imgui
static clock_t fpsStart, fpsEnd;
static int fps {0};
static int currentSkyboxIndex {1};

std::vector<Mesh*> skyBoxMeshes;
Renderer* renderer{nullptr};
//std::vector<Mesh*> meshes;

Scene* sceneInScreen{nullptr};
Scene* sceneOffScreen{nullptr};
Painter* painter{nullptr};
PlayerMesh* player1{nullptr};
CrossMesh* cross{nullptr};

FrameBuffer* frameBuffer {nullptr};

DirectionLight* directionLight{nullptr};
std::vector<std::shared_ptr<PointLight>> pointLights;
SpotLight* spotLight{nullptr};
AmbientLight* ambientLight{nullptr};

glm::mat4 transform{glm::mat4(1.0f)};
PerspectiveCamera* camera{nullptr};
//TrackBallCameraControl* cameraControl{nullptr};
//GameCameraControl* cameraControl{nullptr};
//McGameCameraControl* cameraControl{nullptr};

glm::vec3 backgroundColor{0.0f, 0.0f, 0.0f};

void OnReSize(int width, int height) {
    // 更新帧缓冲区大小
    if (frameBuffer) {
        frameBuffer->resize(width, height);
    }
    glViewport(0, 0, width, height);
    //std::cout<<std::format("OnResize called") << std::endl;
}

void OnKey(int key, int scancode, int action, int mods) {
    //std::cout<<std::format("OnKey called") << std::endl;
    cameraControl->onKey(key,action,mods);
}

void OnMouse(int button, int action, int mods) {
    //std::cout<<std::format("OnMouse called") << std::endl;
    double xpos, ypos;
    glApp->getCursorPos(xpos, ypos);
    cameraControl->onMouse(button,action,xpos,ypos);
}

void OnCursor(double xpos, double ypos) {
    // 检查IMGUI是否想要捕获滚轮事件
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) {
        return;  // IMGUI正在使用滚轮，不传递给主窗口
    }
    //std::cout<<std::format("OnCursor called") << std::endl;
    cameraControl->onCursor(xpos,ypos);
}

void OnScroll(double yoffset) {
    // 检查IMGUI是否想要捕获滚轮事件
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) {
        return;  // IMGUI正在使用滚轮，不传递给主窗口
    }
    //std::cout<<std::format("OnScroll called") << std::endl;
    cameraControl->onScroll(yoffset);
}

void prepareCamera() {
    camera = new PerspectiveCamera(glm::radians(90.0f), static_cast<float>(glApp->getWidth()/glApp->getHeight()),0.01f, 100.0f);
    //camera = new OrthoCamera(-2.0f,2.0f,-2.0f,2.0f,2.0f,-2.0f);
    //camera->setPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    //cameraControl = new TrackBallCameraControl();
    //cameraControl = new GameCameraControl();
    //cameraControl = new McGameCameraControl();
    cameraControl->setCamera(camera);
}

void initAll() {
    mcWD->initMapVec();

    painter->init();

    mcEliminateA->init();

    cross->init();

    mcBag->init();

    mcEquipS->init();
}

void loadMap() {
    mcWD->createCubeMap();
    mcWD->createPlantMap();
    mcWD->createTreeMap();
}

void drawPlayer() {
    // 玩家1
    player1->eyeRadians = -cameraControl->mEyeRadians;
    player1->bodyRadians = -cameraControl->mBodyRadians + 90;
    player1->placeAnimation(cameraControl->placeAnimation);
    player1->takeCubeAnimation(cameraControl->mNowTakeCube);
    player1->draw({ cameraControl->mEyePos.x, cameraControl->mEyePos.y - EYE_HEIGHT, cameraControl->mEyePos.z}, true);
}

void drawBegin(){
    for (auto& sky : skyBoxMeshes) {
        sceneOffScreen->removeChild(sky);
    }
    painter->removeFromScene(sceneOffScreen);
    mcEliminateA->removeFromScene(sceneOffScreen);
    if (mcControl->isBag == true || mcControl->mKeyStates[GLFW_KEY_B]|| mcControl->mKeyStates[GLFW_KEY_ESCAPE]) {
        mcBag->removeFromScene(sceneOffScreen);
    }
    mcEquipS->removeFromScene(sceneOffScreen);

    fpsStart = clock();
    float nowTime = glfwGetTime();
    cameraControl->elapseTime = nowTime - cameraControl->oldTime;
    cameraControl->oldTime = nowTime;
}

void drawEnd() {
    static int fpsNum = 0;
    static double timeLong = 0;

    fpsEnd = clock();

    double endtime = (double(fpsEnd) - fpsStart) / CLOCKS_PER_SEC;

    timeLong += endtime;
    if (timeLong >= 1.0)
    {
        timeLong = 0;
        fps = fpsNum;
        fpsNum = 0;
    }
    fpsNum++;
}

void drawMain() {
    // 清理:实例绘制需要clear
    painter->clearAll();
    mcEliminateA->clearAll();

    // 地图 (根据视角)
    painter->drawMap(cameraControl->mEyePos);
    painter->upDataCube();
    //painter->upDataLeaf();
    painter->upDataPlant();
    painter->addScene(sceneOffScreen);

    // 人物
    drawPlayer();
    // 方块掉落
    mcEliminateA->draw();
    mcEliminateA->updateEliminateCube();
    mcEliminateA->addScene(sceneOffScreen);

    sceneOffScreen->addChild(skyBoxMeshes[currentSkyboxIndex]);

    // 背包
    if (mcControl->isBag == true) {
        mcBag->addScene(sceneOffScreen);
    }

    // 装备栏
    mcEquipS->addScene(sceneOffScreen);

    // 视角控制
    cameraControl->eyeHeight();
}

void prepare() {
    renderer = new Renderer();
    sceneInScreen = new Scene();
    sceneOffScreen = new Scene();
    frameBuffer = new FrameBuffer(WIDTH,HEIGHT);
    //mc
    painter = new Painter();
    player1 = painter->playerMeshes[0].get();
    //准星，每帧都要渲染 不在render中
    cross = new CrossMesh("cross");

    std::vector<std::string> sky1 = {
        "../assets/textures/skybox/cube/sky1/right.jpg",
        "../assets/textures/skybox/cube/sky1/left.jpg",
        "../assets/textures/skybox/cube/sky1/top.jpg",
        "../assets/textures/skybox/cube/sky1/bottom.jpg",
        "../assets/textures/skybox/cube/sky1/front.jpg",
        "../assets/textures/skybox/cube/sky1/back.jpg",
    };

    std::vector<std::string> sky2 = {
        "../assets/textures/skybox/cube/sky2/right.jpg",
        "../assets/textures/skybox/cube/sky2/left.jpg",
        "../assets/textures/skybox/cube/sky2/top.jpg",
        "../assets/textures/skybox/cube/sky2/bottom.jpg",
        "../assets/textures/skybox/cube/sky2/front.jpg",
        "../assets/textures/skybox/cube/sky2/back.jpg",
    };

    std::vector<std::string> sky3 = {
        "../assets/textures/skybox/cube/sky3/right.jpg",
        "../assets/textures/skybox/cube/sky3/left.jpg",
        "../assets/textures/skybox/cube/sky3/top.jpg",
        "../assets/textures/skybox/cube/sky3/bottom.jpg",
        "../assets/textures/skybox/cube/sky3/front.jpg",
        "../assets/textures/skybox/cube/sky3/back.jpg",
    };

    //cube天空盒
    //depthFunc 为GL_LEQUAL
    Geometry* skybox = Geometry::CreateBox(100.0f,100.0f,100.0f);

    CubeMaterial* cubeSkyMat0 = new CubeMaterial();
    cubeSkyMat0->mCubeTexture = new Texture(sky1,0);
    Mesh* cubeSkyMesh0 = new Mesh(skybox,cubeSkyMat0);

    CubeMaterial* cubeSkyMat1 = new CubeMaterial();
    cubeSkyMat1->mCubeTexture = new Texture(sky2,0);
    Mesh* cubeSkyMesh1 = new Mesh(skybox,cubeSkyMat1);

    CubeMaterial* cubeSkyMat2 = new CubeMaterial();
    cubeSkyMat2->mCubeTexture = new Texture(sky3,0);
    Mesh* cubeSkyMesh2 = new Mesh(skybox,cubeSkyMat2);

    skyBoxMeshes.push_back(cubeSkyMesh0);
    skyBoxMeshes.push_back(cubeSkyMesh1);
    skyBoxMeshes.push_back(cubeSkyMesh2);

    initAll();
    loadMap();
    cameraControl->eyeInit(mcWD->getAltitude(WORLD_WIDTH / 2, WORLD_WIDTH / 2, WORLD_HEIGHT - 1));

    //painter->drawMap(cameraControl->mEyePos);
    //player1->draw({ cameraControl->mEyePos.x, cameraControl->mEyePos.y - EYE_HEIGHT, cameraControl->mEyePos.z}, true);
    //painter->upDataCube();
    //painter->upDataLeaf();
    //painter->upDataPlant();

    //加入场景初始化
    painter->addScene(sceneOffScreen);
    sceneOffScreen->addChild(cross);
    mcEliminateA->addScene(sceneOffScreen);

    Geometry* screen = Geometry::CreateScreenPlane();
    ScreenMaterial* screenMaterial = new ScreenMaterial();
    screenMaterial->mScreenTexture = frameBuffer->mColorAttachment;
    Mesh* screenMesh = new Mesh(screen,screenMaterial);
    sceneInScreen->addChild(screenMesh);

    directionLight = new DirectionLight(glm::vec3(1.0f, 1.0f, 1.0f),glm::vec3(-1.0f,-1.0f,-1.0f));
    directionLight->mSpecularIntensity = 0.1f;

    std::shared_ptr<PointLight> pointLight01 = std::make_shared<PointLight>(0.017f,0.07f,1.0f);
    pointLight01->setPosition(glm::vec3(0.0f, 0.0f, 1.5f));
    pointLight01->mSpecularIntensity = 1.0f;

    std::shared_ptr<PointLight> pointLight02 = std::make_shared<PointLight>(0.017f,0.07f,1.0f);
    pointLight02->setPosition(glm::vec3(0.0f,-1.5f,0.0f));
    pointLight02->setColor(glm::vec3(0.0f,1.0f,0.0f));
    pointLight02->mSpecularIntensity = 1.0f;

    pointLights.push_back(pointLight01);
    pointLights.push_back(pointLight02);

    spotLight = new SpotLight(glm::radians(25.0f),glm::radians(30.0f),glm::vec3(-1.0f,0.0f,0.0f));
    spotLight->setPosition(glm::vec3(1.0f, 0.0f, 0.0f));
    spotLight->mSpecularIntensity = 1.0f;

    ambientLight = new AmbientLight(glm::vec3(0.4f));
}

void initIMGUI() {
    //创建imgui上下文
    ImGui::CreateContext();
    //选择一个主题
    ImGui::StyleColorsDark();

    //设置ImGui与GLFW和OPENGL的绑定
    ImGui_ImplGlfw_InitForOpenGL(glApp->getWindow(),true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void renderIMGUI() {
    //开启当前的IMGUI渲染
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    //决定当前的GUI上面有哪些控件
    ImGui::Begin("Minecraft Setting");

    //根据 FPS 值改变颜色（60+绿色，30~60 黄色，<30 红色）
    ImVec4 fpsColor;
    if (fps >= 60) {
        fpsColor = ImVec4(0.0f, 1.0f, 0.0f, 1.0f); // 绿色
    } else if (fps >= 30) {
        fpsColor = ImVec4(1.0f, 1.0f, 0.0f, 1.0f); // 黄色
    }else {
        fpsColor = ImVec4(1.0f, 0.0f, 0.0f, 1.0f); // 红色
    }

    ImGui::TextColored(fpsColor, "FPS: %d", fps); // 带颜色的文字

    //天空盒选择下拉菜单
    const char* skyboxNames[] = { "SkyStyle - Sunny", "SkyStyle - Night", "SkyStyle - Cloudy" };
    ImGui::Combo("Choose SkyStyle", &currentSkyboxIndex, skyboxNames, IM_ARRAYSIZE(skyboxNames));

    //控制移动速度
    ImGui::SliderFloat("Move Speed",&(mcControl->mMoveSpeed),0.0f,20.0f);
    ImGui::SliderFloat("Jump Height",&(mcControl->mJumpHeight),0.0f,10.0f);
    ImGui::SliderFloat("Cube Lifetime",&(mcEliminateA->dropCubeLifetime),0.0f,20.0f);

    ImGui::End();

    //执行UI渲染
    ImGui::Render();
    int display_w,display_h;
    glfwGetWindowSize(glApp->getWindow(),&display_w,&display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main() {

    if(!glApp->init(false,WIDTH, HEIGHT, "Welcome To Minecraft")){
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glApp->SetResizeCallback(OnReSize);
    glApp->SetKeyCallback(OnKey);
    glApp->SetCursorCallback(OnCursor);
    glApp->SetMouseCallback(OnMouse);
    glApp->SetScrollCallback(OnScroll);

    // 启用 VSync（锁定到显示器刷新率）
    glfwSwapInterval(1);  // 1 = 启用，0 = 禁用

    GL_CALL_ERROR(glViewport(0, 0,WIDTH, HEIGHT));
    //GL_CALL_ERROR(glClearColor(0.88f, 0.49f, 0.58f, 1.0f));
    GL_CALL_ERROR(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

    prepareCamera();
    prepare();

    initIMGUI();

    while(glApp->update()){
        cameraControl->update();

        drawBegin();
        drawMain();
        drawEnd();

        renderer->setBackgroundColor(backgroundColor);
        //renderer->render(sceneOffScreen,camera,directionLight,pointLights,spotLight,ambientLight,frameBuffer->mFbo);
        renderer->render(sceneOffScreen,camera,directionLight,pointLights,spotLight,ambientLight);

        renderIMGUI();
    }

    glApp->destroy();

    return 0;
}