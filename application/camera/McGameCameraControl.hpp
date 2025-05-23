#pragma once
#include "cameraControl.hpp"
#include "../../minecraft/CreateWorld.hpp"

#define mcControl McGameCameraControl::getInstance()

// 第一人 称
#define EYE_HEIGHT 1

//玩家状态
enum class PlayerStatus
{
    Normal,		//默认
    JumpUp,		//跳跃
    Decline		//下降
};

class McGameCameraControl :public CameraControl {
public:
    static McGameCameraControl* getInstance();
    ~McGameCameraControl() override;

    void onKey(int button, int action, int mods) override;
    void onMouse(int button, int action, double xpos, double ypos) override;
    void onScroll(double yoffset) override;
    void onCursor(double xpos, double ypos) override;

    //人物出现时的所在的海拔
    void eyeInit(int altitude);
    // 有关人物所在海拔的处理函数(跳跃,下降)
    void eyeHeight();
    // 更新指示方块的坐标
    void upCubeFrame();
    // 放置
    void mouseLeft();
    // 消除
    void mouseRight();
    // 人物移动
    void cameraMoveKey();
    // 人物跳跃
    void cameraJumpKey();
    // 碰撞检测
    inline bool isCollision(float x, float y, float z);
    // 却换手持方块
    void takeCube();

    void pitch(float angle);
    void yaw(float angle);
    void update() override;

public:
    // 身体角度
    float mBodyRadians = -180.0f;
    // 视角角度
    float mEyeRadians = 0.0f;
    // 视角海拔
    float mEyeAltitude = 0;
    glm::vec3 mEyePos = glm::vec3(WORLD_WIDTH / 2, mEyeAltitude, WORLD_WIDTH / 2);

    // 放置动画
    bool placeAnimation = false;
    // 手持方块
    CubeType mNowTakeCube = CubeType::Dirt;
    // 人物状态
    PlayerStatus mPlayerStatus = PlayerStatus::Normal;
    // 经过时间
    float elapseTime = 0.0f;
    // 上次时间
    float oldTime = 1.0f;

    float mMoveSpeed = 5.0f;
    float mPlaceSpeed = 0.5f;

    // 跳跃
    float gravity {20.0f};
    float mJumpHeight { EYE_HEIGHT * 1.0f};
    float mJumpTimer {0.0f}; // 跳跃计时器
    float mInitialJumpY {0.0f};
    float mJumpPeakY{0.0f};
    float mJumpInitialVelocity {0.0f};
    float mJumpTotalTime{0.0f};

    // esc停止标志
    bool isGameSTOP = false;
    // 时候换方块
    //bool isChangeCube = false;
    //  滚轮切换方块
    int mScrollValue = 1;

    // 破坏方块
    bool mIsBreaking = false;         // 是否正在破坏方块

    // 是否正在放置
    bool mIsPlacing = false;
    glm::vec3 mBreakingBlockPos;     // 当前正在破坏的方块位置

    //展示书包
    bool isBag = false;
private:
    McGameCameraControl();
    static McGameCameraControl* mInstance;
};