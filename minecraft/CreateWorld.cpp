#include "CreateWorld.hpp"

CreateWorld* CreateWorld::mInstance{nullptr};

static std::random_device r;
static std::default_random_engine e1(r());
static std::uniform_int_distribution<int> uniformAltitude(-1, 1);
static std::uniform_int_distribution<int> uniformSnow(-5, 3);
static std::uniform_int_distribution<int> uniformSnowRegion(-SNOW_RADIUS, SNOW_RADIUS);
static std::uniform_int_distribution<int> uniformValue(0, WORLD_WIDTH - 1);

CreateWorld::CreateWorld() {

}

CreateWorld::~CreateWorld() {
    delete mInstance;
}

CreateWorld *CreateWorld::getInstance() {
    if (mInstance == nullptr) {
        mInstance = new CreateWorld();
    }
    return mInstance;
}

//初始化地图容器
void CreateWorld::initMapVec()
{
    //(Y坐标(Z坐标(X坐标)))
    mapData.resize(mWorldWidth, std::vector<std::vector<CubeType>>(mWorldHeight, std::vector<CubeType>(mWorldWidth, CubeType::Null)));
    plantData.resize(mWorldWidth, std::vector<std::vector<CubeType>>(mWorldHeight, std::vector<CubeType>(mWorldWidth, CubeType::Null)));
    leafData.resize(mWorldWidth, std::vector<std::vector<CubeType>>(mWorldHeight, std::vector<CubeType>(mWorldWidth, CubeType::Null)));
    waterSurfaceData.resize(mWorldWidth, std::vector<std::vector<CubeType>>(mWorldHeight, std::vector<CubeType>(mWorldWidth, CubeType::Null)));
}

//海拔生成
void CreateWorld::randInitAltitude() {
    //reserve函数用于预分配内存，确保在向vector中插入元素时不会频繁地重新分配内存
    beforeAltitudeVec.reserve(mWorldWidth);

    int rand_num = 0;
    int num_old = 0;
    int altitude = mWorldHeight / 2;

    for (int i = 0; i < mWorldWidth; i++) {
        num_old = rand_num;
        rand_num = uniformAltitude(e1);

        if (rand_num == -1){
            if (num_old == 1) {
                rand_num = 0;
            }
        }else if (rand_num == 1){
            if (num_old == -1) {
                rand_num = 0;
            }
        }

        altitude += rand_num;

        if (altitude <= 0 || altitude >= 200) {
            altitude = 100;
        }
        beforeAltitudeVec.push_back(mWorldHeight / 2);
    }
}

void CreateWorld::randLineAltitude() {
    int rand_num = 0;
    int altitude = 0;
    double avg1 = 0;
    double avg2 = 0;
    for (int i = 1; i < mWorldWidth - 1; i++) {
        rand_num = uniformAltitude(e1);
        avg1 = double(beforeAltitudeVec[i] + beforeAltitudeVec[i - 1] + beforeAltitudeVec[i + 1]) / 3;
        avg2 = double(avg1 + rand_num + beforeAltitudeVec[i - 1] + beforeAltitudeVec[i + 1]) / 3;

        if (avg1 == avg2) {
            altitude = avg2;
        }else if (avg1 > avg2) {
            altitude = avg2;
        }else if (avg1 < avg2) {
            altitude = avg2 + 1;
        }

        if (altitude <= 0 || altitude >= 200) {
            altitude = 100;
        }

        beforeAltitudeVec[i] = altitude;
    }
}

void CreateWorld::randAllAltitude() {
    for (int x = 0; x < mWorldWidth; x++) {
        altitudeMapVec.push_back(beforeAltitudeVec);
        randLineAltitude();
    }
}

void CreateWorld::avgAltitude() {
    int avg = 0;
    for (int x = 0; x < mWorldWidth; x++) {
        for (int z = mWorldWidth; z > 0; z--) {
            avg = 0;
            int count = 5;

            if (x + 2 < mWorldWidth && z - 2 > 0 && z + 2 < mWorldWidth) {
                avg += altitudeMapVec[x + 2][z - 2];
                avg += altitudeMapVec[x + 2][z - 1];
                avg += altitudeMapVec[x + 2][z + 0];
                avg += altitudeMapVec[x + 2][z + 1];
                avg += altitudeMapVec[x + 2][z + 2];
            } else {
                count -= 1;
            }

            if (x + 1 < mWorldWidth && z - 2 > 0 && z + 2 < mWorldWidth) {
                avg += altitudeMapVec[x + 1][z - 2];
                avg += altitudeMapVec[x + 1][z - 1];
                avg += altitudeMapVec[x + 1][z + 0];
                avg += altitudeMapVec[x + 1][z + 1];
                avg += altitudeMapVec[x + 1][z + 2];
            } else {
                count -= 1;
            }

            if (z - 2 > 0 && z + 2 < mWorldWidth) {
                avg += altitudeMapVec[x + 0][z - 2];
                avg += altitudeMapVec[x + 0][z - 1];
                avg += altitudeMapVec[x + 0][z + 0];
                avg += altitudeMapVec[x + 0][z + 1];
                avg += altitudeMapVec[x + 0][z + 2];
            } else {
                count -= 1;
            }

            if (x - 1 > 0 && z - 2 > 0 && z + 2 < mWorldWidth) {
                avg += altitudeMapVec[x - 1][z - 2];
                avg += altitudeMapVec[x - 1][z - 1];
                avg += altitudeMapVec[x - 1][z + 0];
                avg += altitudeMapVec[x - 1][z + 1];
                avg += altitudeMapVec[x - 1][z + 2];
            } else {
                count -= 1;
            }

            if (x - 2 > 0 && z - 2 > 0 && z + 2 < mWorldWidth) {
                avg += altitudeMapVec[x - 2][z - 2];
                avg += altitudeMapVec[x - 2][z - 1];
                avg += altitudeMapVec[x - 2][z + 0];
                avg += altitudeMapVec[x - 2][z + 1];
                avg += altitudeMapVec[x - 2][z + 2];
            } else {
                count -= 1;
            }

            if (count > 0) {
                avg = avg / (count * 5);
                altitudeMapVec[x][z] = avg;
            }
        }
    }
}

void CreateWorld::fillingGrasslandCube() {
    //根据海拔数据填充土块和草地
    for (int x = 0; x < mWorldWidth; x++) {
        for (int z = 0; z < mWorldWidth; z++) {
            mapData[x][altitudeMapVec[x][z]][z] = CubeType::GrassBlock;
            for (int y = 0; y < altitudeMapVec[x][z]; y++)
                mapData[x][y][z] = CubeType::Dirt;
        }
    }
}

// 随机生成雪地
void CreateWorld::randSnowCubeMap() {
    int centerX = uniformValue(e1);
	int centerZ = uniformValue(e1);

	for (int z = centerZ - mSnowRadius; z < centerZ + mSnowRadius; z++) {
		for (int x = centerX - mSnowRadius; x < centerX + mSnowRadius; x++) {
			if (x > 1 && z > 1 && x < mWorldWidth - 1 && z < mWorldWidth - 1) {
				int xx = x - (centerX - mSnowRadius);
				int zz = z - (centerZ - mSnowRadius);
				if (xx / mSnowRadius == 0 && zz / mSnowRadius == 0) {
					if (xx % mSnowRadius > (mSnowRadius - mSnowRadius / 4 - zz % mSnowRadius)) {
						int rand = uniformSnow(e1);
						if (rand == -5) {
						    mapData[x - 1][altitudeMapVec[x - 1][z]][z] = CubeType::Snow;
						} else if (rand == 3) {
						    mapData[x][altitudeMapVec[x][z - 1]][z - 1] = CubeType::Snow;
						}
						mapData[x][altitudeMapVec[x][z]][z] = CubeType::Snow;
					}
				} else if (xx / mSnowRadius == 1 && zz / mSnowRadius == 0) {
					if (xx % mSnowRadius < mSnowRadius / 4 + zz % mSnowRadius) {
						int rand = uniformSnow(e1);
						if (rand == -5) {
						    mapData[x + 1][altitudeMapVec[x + 1][z]][z] = CubeType::Snow;
						} else if (rand == 3) {
						    mapData[x][altitudeMapVec[x][z - 1]][z - 1] = CubeType::Snow;
						}
						mapData[x][altitudeMapVec[x][z]][z] = CubeType::Snow;
					}
				}
				else if (xx / mSnowRadius == 0 && zz / mSnowRadius == 1) {
					if (xx % mSnowRadius > zz% mSnowRadius - mSnowRadius / 4) {
						int rand = uniformSnow(e1);
						if (rand == -5) {
						    mapData[x - 1][altitudeMapVec[x - 1][z]][z] = CubeType::Snow;
						} else if (rand == 3) {
						    mapData[x][altitudeMapVec[x][z + 1]][z + 1] = CubeType::Snow;
						}
						mapData[x][altitudeMapVec[x][z]][z] = CubeType::Snow;
					}
				}
				else if (xx / mSnowRadius == 1 && zz / mSnowRadius == 1) {
					if (xx % mSnowRadius < (mSnowRadius + mSnowRadius / 4 - zz % mSnowRadius)) {
						int rand = uniformSnow(e1);
						if (rand == -5) {
						    mapData[x + 1][altitudeMapVec[x + 1][z]][z] = CubeType::Snow;
						} else if (rand == 3) {
						    mapData[x][altitudeMapVec[x][z - 1]][z + 1] = CubeType::Snow;
						}
						mapData[x][altitudeMapVec[x][z]][z] = CubeType::Snow;
					}
				}
			}
		}
	}
}

// 随机分布小型植物
void CreateWorld::createPlantMap() {
    int x = 0;
    int z = 0;

    for (int i = 0; i < mWorldWidth * 20; i++){
        x = uniformValue(e1);
        z = uniformValue(e1);
        // 跳过水域和雪地
        if (mapData[x][altitudeMapVec[x][z]][z] == CubeType::Snow ||
            mapData[x][altitudeMapVec[x][z]][z] == CubeType::WaterBelow) {
            continue;
        }
        switch (uniformValue(e1) % 6){
            case 0:
                plantData[x][altitudeMapVec[x][z] + 1][z] = CubeType::Grass_1;
                break;
            case 1:
                plantData[x][altitudeMapVec[x][z] + 1][z] = CubeType::Grass_2;
                break;
            case 2:
                plantData[x][altitudeMapVec[x][z] + 1][z] = CubeType::Grass_3;
                break;
            case 3:
                plantData[x][altitudeMapVec[x][z] + 1][z] = CubeType::Mushroom;
                break;
            case 4:
                plantData[x][altitudeMapVec[x][z] + 1][z] = CubeType::RedMushroom;
                break;
            case 5:
                plantData[x][altitudeMapVec[x][z] + 1][z] = CubeType::Flower;
                break;
            default:
                break;
        }
    }
}

//检测周围是否有树木
bool CreateWorld::isAroundTree(int x, int z)
{
    for (int xx = x - 4; xx <= x + 4; xx++) {
        for (int zz = z - 4; zz <= z + 4; zz++) {
            if (xx >= 0 && xx < mWorldWidth && zz >= 0 && zz < mWorldWidth) {
                if (mapData[xx][altitudeMapVec[xx][zz]][zz] == CubeType::OakLog) {
                    return true;
                }
            }
        }
    }
    return false;
}

// 生成树木群和单棵树
void CreateWorld::createTreeMap() {
    int x = 0;
    int z = 0;
    for (int region = 0; region < (mWorldWidth / 25) * 2; region++) {
        int xx = uniformValue(e1) % (mWorldWidth - mWorldWidth / 4);
        int zz = uniformValue(e1) % (mWorldWidth - mWorldWidth / 4);
        for (int count = 0; count < mWorldWidth / 8; count++) {
            x = uniformValue(e1) % (mWorldWidth / 4) + xx;
            z = uniformValue(e1) % (mWorldWidth / 4) + zz;

            if (x >= mWorldWidth || z >= mWorldWidth)
                continue;

            // 跳过水域区域（地表高度低于水位线）
            if (altitudeMapVec[x][z] < mWaterLevel)
                continue;

            if (plantData[x][altitudeMapVec[x][z] + 1][z] == CubeType::Null) {
                if (!isAroundTree(x, z))
                {
                    int height = uniformValue(e1) % 3 + 5 + altitudeMapVec[x][z];

                    fillingLeave(x, z, height, CubeType::LeaveGreen);

                    for (int y = altitudeMapVec[x][z]; y < height; y++)
                    {
                        altitudeMapVec[x][z] += 1;
                        mapData[x][altitudeMapVec[x][z]][z] = CubeType::OakLog;
                    }
                }
            }
        }
    }

    int count = 0;
    do {
        int x = uniformValue(e1);
        int z = uniformValue(e1);

        // 跳过水域区域（地表高度低于水位线）
        if (altitudeMapVec[x][z] < mWaterLevel)
            continue;

        if (plantData[x][altitudeMapVec[x][z] + 1][z] == CubeType::Null) {
            if (mapData[x][altitudeMapVec[x][z]][z] != CubeType::Snow) {
                if (!isAroundTree(x, z)) {
                    int height = uniformValue(e1) % 3 + 5 + altitudeMapVec[x][z];

                    if (count % 2 == 0)
                        fillingLeave(x, z, height, CubeType::LeaveRed);
                    else
                        fillingLeave(x, z, height, CubeType::LeaveYellow);

                    for (int y = altitudeMapVec[x][z]; y < height; y++)
                    {
                        altitudeMapVec[x][z] += 1;
                        mapData[x][altitudeMapVec[x][z]][z] = CubeType::OakLog;
                    }
                    count++;
                }
            }
        }
    } while (count < mWorldWidth);
}

// 生成树叶
void CreateWorld::fillingLeave(int x, int z, int height, CubeType ct)
{
    if (x > 1 && x < mWorldWidth - 1 && z >1 && z < mWorldWidth) {
        if (mapData[x - 1][altitudeMapVec[x - 1][z + 0]][z + 0] == CubeType::Snow &&
            mapData[x + 1][altitudeMapVec[x + 1][z + 0]][z + 0] == CubeType::Snow &&
            mapData[x + 0][altitudeMapVec[x + 0][z - 1]][z - 1] == CubeType::Snow &&
            mapData[x + 0][altitudeMapVec[x + 0][z + 1]][z + 1] == CubeType::Snow) {
            ct = CubeType::LeaveWhite;
        }
    }

    for (int xx = x - 1; xx <= x + 1; xx++) {
        for (int zz = z - 1; zz <= z + 1; zz++) {
            if (xx >= 0 && xx < mWorldWidth && zz >= 0 && zz < mWorldWidth) {
                leafData[xx][height + 0][zz] = ct;
                if (xx == x || zz == z) {
                    leafData[xx][height + 1][zz] = ct;
                }
            }
        }
    }

    for (int xx = x - 2; xx <= x + 2; xx++) {
        for (int zz = z - 2; zz <= z + 2; zz++) {
            if (xx >= 0 && xx < mWorldWidth && zz >= 0 && zz < mWorldWidth) {
                leafData[xx][height - 1][zz] = ct;
                leafData[xx][height - 2][zz] = ct;
            }
        }
    }

    if (height - altitudeMapVec[x][z] >= 7) {
        for (int xx = x - 1; xx <= x + 1; xx++) {
            for (int zz = z - 1; zz <= z + 1; zz++) {
                if (xx >= 0 && xx < mWorldWidth && zz >= 0 && zz < mWorldWidth) {
                    leafData[xx][height - 3][zz] = ct;
                }
            }
        }
    }
}

void CreateWorld::createCubeMap(){
    //创建海拔
    randInitAltitude();
    randAllAltitude();
    avgAltitude();

    // 步骤2：计算地形统计特征
    calculateTerrainStats();

    // 步骤3：动态计算合理水位
    calculateWaterLevel();

    fillingGrasslandCube();
    randSnowCubeMap();

    // 步骤6：生成水域系统
    generateWater();
}

int CreateWorld::getAltitude(int x, int z, int y){
    for (int i = y; i > 0; i--) {
        if (mapData[x][i][z] != CubeType::Null) {
            return i + 1;
        }
    }
    return 0;
}

// 新增：计算地形统计特征（最小/最大/平均海拔）
void CreateWorld::calculateTerrainStats() {
    int totalAltitude = 0;
    mMinAltitude = WORLD_HEIGHT;  // 初始化为世界最大高度
    mMaxAltitude = 0;

    for (int x = 0; x < mWorldWidth; x++) {
        for (int z = 0; z < mWorldWidth; z++) {
            int alt = altitudeMapVec[x][z];
            totalAltitude += alt;

            if (alt < mMinAltitude) mMinAltitude = alt;
            if (alt > mMaxAltitude) mMaxAltitude = alt;
        }
    }
    mAvgAltitude = totalAltitude / (mWorldWidth * mWorldWidth);
}

// 动态计算合理水位
void CreateWorld::calculateWaterLevel() {
    // 收集所有地形海拔数据w
    std::vector<int> allAltitudes;
    for (int x = 0; x < mWorldWidth; x++) {
        for (int z = 0; z < mWorldWidth; z++) {
            allAltitudes.push_back(altitudeMapVec[x][z]);
        }
    }

    // 计算WATER_PERCENTILE分位数（水域覆盖比例）
    std::sort(allAltitudes.begin(), allAltitudes.end());
    size_t percentileIndex = (allAltitudes.size() * WATER_PERCENTILE) / 100;
    int baseWaterLevel = allAltitudes[percentileIndex];

    // 添加随机偏移（±WATER_OFFSET_RANGE格）
    std::uniform_int_distribution<int> offsetDist(-WATER_OFFSET_RANGE, WATER_OFFSET_RANGE);
    mWaterLevel = baseWaterLevel + offsetDist(e1);

    // 限制水位范围（确保水域合理）
    mWaterLevel = std::clamp(mWaterLevel,
        mMinAltitude + 10,  // 最低不低于地形最低点+5格（避免太浅）
        mMaxAltitude - 10   // 最高不超过地形最高点-10格（避免淹没山顶）
    );
}

// 新增：生成水域方块（水面+水体）
void CreateWorld::generateWater() {
    for (int x = 0; x < mWorldWidth; x++) {
        for (int z = 0; z < mWorldWidth; z++) {
            int surfaceHeight = altitudeMapVec[x][z];  // 当前位置地表高度

            // 仅处理低于水位线的区域
            if (surfaceHeight < mWaterLevel ) {
                // 将地表高度处的方块替换为沙子（水体底部）
                mapData[x][surfaceHeight][z] = CubeType::Sand;

                // 检测当前区域是否为雪地（3x3范围内有雪方块）
                bool isSnowRegion = false;
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dz = -1; dz <= 1; dz++) {
                        int nx = x + dx;
                        int nz = z + dz;
                        if (nx >= 0 && nx < mWorldWidth && nz >= 0 && nz < mWorldWidth) {
                            if (mapData[nx][altitudeMapVec[nx][nz]][nz] == CubeType::Snow) {
                                isSnowRegion = true;
                                break;
                            }
                        }
                    }
                    if (isSnowRegion) break;
                }

                // 生成水面方块（仅在水位线高度）
                if (mWaterLevel < mWorldHeight) {  // 确保不超出世界高度
                    if (isSnowRegion) {
                        mapData[x][mWaterLevel][z] = CubeType::Ice;
                    }else {
                        waterSurfaceData[x][mWaterLevel][z] =CubeType::WaterSurface;
                    }
                }

                // 生成水体下方方块（地表到水位线之间）
                for (int y = surfaceHeight + 1; y < mWaterLevel ; y++) {
                    // 保留基岩（Bedrock）不被替换
                    if (mapData[x][y][z] != CubeType::Bedrock) {
                        mapData[x][y][z] = CubeType::Null;  // 清除原方块
                        if (isSnowRegion) {
                            mapData[x][y][z] = CubeType::Ice;
                        } else {
                            mapData[x][y][z] =CubeType::WaterBelow;
                        }
                        // waterSurfaceData[x][mWaterLevel][z] =
                        //     isSnowRegion ? CubeType::Ice : CubeType::WaterBelow;
                        // //mapData[x][y][z] = CubeType::WaterBelow;
                    }
                }
            }
        }
    }
}