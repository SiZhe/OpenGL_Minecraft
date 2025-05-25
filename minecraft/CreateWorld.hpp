#pragma once
#include "../glframework/Core.hpp"

//地图长宽高
#define WORLD_WIDTH	1200
#define WORLD_HEIGHT 200
//雪地面积
#define SNOW_RADIUS 400
//水域
#define WATER_PERCENTILE 11   // 水域覆盖百分比（%）
#define WATER_OFFSET_RANGE 0  // 水位随机偏移范围（± 格数）w

#define mcWD CreateWorld::getInstance()

enum class CubeType
{
    // 基础方块
    Null,               // 空
    Dirt,               // 泥土
    Sand,               // 沙子
    Stone,              // 石头
    Bricks,             // 砖块
    OakPlanks,          // 橡木板
    Bedrock,            // 基岩
    Glowstone,          // 发光石
    //GroundBrick,        // 地砖
    StoneBrick,         // 石砖

    //地图
    GrassBlock,         // 草方块
    OakLog,             // 橡木原木
    Snow,               // 雪地

    //树叶
    LeaveGreen,		    //绿树叶
    LeaveRed,		    //红树叶
    LeaveYellow,		//黄树叶
    LeaveWhite,		    //白树叶

    SandStone,          // 沙岩
    Ice,                // 冰
    BirchPlanks,        // 桦木板
    BirchLog,           // 桦木原木

    // 块
    BlockOfDiamond,     // 钻石块
    BlockOfGold,        // 金块
    BlockOfEmerald,     // 翡翠块
    BlockOfLapis,       // 青金石块
    BlockOfIron,        // 铁块
    BlockOfQuartz,      // 石英块
    BlockOfRedstone,    // 红石块

    // 矿石
    DiamondOre,         // 钻石矿石
    EmeraldOre,         // 翡翠矿石
    CoalOre,            // 煤矿石
    GoldOre,            // 金矿石
    IronOre,            // 铁矿石
    LapisLazuliOre,     // 青金石矿石
    RedstoneOre,        // 红石矿石

    // 羊毛
    //LightBlueWool,      // 浅蓝色羊毛
    //LimeWool,           // 莱姆色羊毛
    //MagentaWool,        // 品红色羊毛
    //OrangeWool,         // 橙色羊毛
    PinkWool,           // 粉色羊毛
    RedWool,            // 红色羊毛
    YellowWool,         // 黄色羊毛
    WhiteWool,          // 白色羊毛
    BlackWool,          // 黑色羊毛

    // 杂物
    Bookshelf,          // 书架
    CarvedPumpkin,      // 雕刻南瓜
    CraftingTable,      // 工作台
    Furnace,            // 炉子
    Glass,              // 玻璃
    HayBale,            // 干草块
    Melon,              // 西瓜
    TNT,                // TNT炸药

    //植物
    Mushroom,		    //蘑菇
    RedMushroom,		//红蘑菇
    Flower,			    //小花
    Grass_1,			//草1
    Grass_2,			//草2
    Grass_3,			//草3

    WaterSurface,        //水体表面
    WaterBelow,          //默认水体
};

inline std::map<CubeType, int> CubeEliminateTimes = {
    // 基础方块
    {CubeType::Null, 0},               // 空
    {CubeType::Dirt, 4},               // 泥土
    {CubeType::Sand, 4},               // 沙子
    {CubeType::Stone, 8},              // 石头
    {CubeType::Bricks, 8},             // 砖块
    {CubeType::OakPlanks, 6},          // 橡木板
    {CubeType::Bedrock, 32},           // 基岩
    {CubeType::Glowstone, 8},          // 发光石
    {CubeType::StoneBrick, 8},         // 石砖

    // 地图
    {CubeType::GrassBlock, 4},         // 草方块
    {CubeType::OakLog, 6},             // 橡木原木
    {CubeType::Snow, 4},               // 雪地

    // 树叶
    {CubeType::LeaveGreen, 1},         // 绿树叶
    {CubeType::LeaveRed, 1},           // 红树叶
    {CubeType::LeaveYellow, 1},        // 黄树叶
    {CubeType::LeaveWhite, 1},         // 白树叶

    {CubeType::SandStone, 4},          // 沙岩
    {CubeType::Ice, 2},                // 冰
    {CubeType::BirchPlanks, 6},        // 桦木板
    {CubeType::BirchLog, 6},           // 桦木原木

    // 块
    {CubeType::BlockOfDiamond, 32},     // 钻石块
    {CubeType::BlockOfGold, 8},        // 金块
    {CubeType::BlockOfEmerald, 8},     // 翡翠块
    {CubeType::BlockOfLapis, 16},       // 青金石块
    {CubeType::BlockOfIron, 16},        // 铁块
    {CubeType::BlockOfQuartz, 6},      // 石英块
    {CubeType::BlockOfRedstone, 6},    // 红石块

    // 矿石
    {CubeType::DiamondOre, 32},         // 钻石矿石
    {CubeType::EmeraldOre, 8},         // 翡翠矿石
    {CubeType::CoalOre, 6},            // 煤矿石
    {CubeType::GoldOre, 8},            // 金矿石
    {CubeType::IronOre, 16},            // 铁矿石
    {CubeType::LapisLazuliOre, 24},     // 青金石矿石
    {CubeType::RedstoneOre, 8},        // 红石矿石

    // 羊毛
    {CubeType::PinkWool, 2},           // 粉色羊毛
    {CubeType::RedWool, 2},            // 红色羊毛
    {CubeType::YellowWool, 2},         // 黄色羊毛
    {CubeType::WhiteWool, 2},          // 白色羊毛
    {CubeType::BlackWool, 2},          // 黑色羊毛

    // 杂物
    {CubeType::Bookshelf, 2},          // 书架
    {CubeType::CarvedPumpkin, 2},      // 雕刻南瓜
    {CubeType::CraftingTable, 2},      // 工作台
    {CubeType::Furnace, 2},            // 炉子
    {CubeType::Glass, 2},              // 玻璃
    {CubeType::HayBale, 2},            // 干草块
    {CubeType::Melon, 2},              // 西瓜
    {CubeType::TNT, 0},                // TNT炸药

    // 植物
    {CubeType::Mushroom, 0},           // 蘑菇
    {CubeType::RedMushroom, 0},        // 红蘑菇
    {CubeType::Flower, 0},             // 小花
    {CubeType::Grass_1, 0},            // 草1
    {CubeType::Grass_2, 0},            // 草2
    {CubeType::Grass_3, 0},            // 草3
};


using vec3 = std::vector<std::vector<std::vector<CubeType>>>;

class CreateWorld {
public:
    static CreateWorld* getInstance();
    ~CreateWorld();

    //初始化地图容器
    void initMapVec();
    //创建方块
    void createCubeMap();
    //创建植物
    void createPlantMap();
    //创建树木
    void createTreeMap();
    //所在地图的海拔
    int getAltitude(int x, int z, int y);
    //方块位置数据
    vec3 mapData;
    //树木位置数据
    vec3 plantData;
    //树叶位置数据
    vec3 leafData;
    //水表面位置数据
    vec3 waterSurfaceData;

private:
    CreateWorld();
    static CreateWorld* mInstance;

    int mWorldWidth{WORLD_WIDTH};
    int mWorldHeight{WORLD_HEIGHT};
    int mSnowRadius{SNOW_RADIUS};

    //前海拔(创建随机地图时使用)
    std::vector<int> beforeAltitudeVec;
    //地图海拔(创建随机地图时使用)
    std::vector<std::vector<int>> altitudeMapVec;

    //地图第一行的海拔随机初始化
    void randInitAltitude();
    //地图中一行海拔数据的初始化
    void randLineAltitude();
    //随机初始化地图的所有海拔
    void randAllAltitude();
    //地图海拔平均化
    void avgAltitude();
    //填充草地
    void fillingGrasslandCube();
    //填充树叶
    void fillingLeave(int x, int z, int height, CubeType ct);
    //随机创建雪地
    void randSnowCubeMap();
    //周围是否存在树木
    bool isAroundTree(int x, int z);

    // 新增水域相关成员
    int mWaterLevel;         // 动态计算的水域高度
    int mMinAltitude;        // 地形最小海拔
    int mMaxAltitude;        // 地形最大海拔
    int mAvgAltitude;        // 地形平均海拔

    // 新增方法声明
    void calculateTerrainStats();  // 计算地形统计特征
    void calculateWaterLevel();    // 动态计算合理水位
    void generateWater();          // 生成水域方块
};