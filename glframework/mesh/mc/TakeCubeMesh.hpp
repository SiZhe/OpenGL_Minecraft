#pragma once
#include "../Mesh.hpp"
#include "../../application/camera/camera.hpp"

class TakeCubeMesh : public Mesh{
public:
    TakeCubeMesh(const std::string& name);
    ~TakeCubeMesh();

    void init();
    void upDataShaderFir(float eyeRadians, float bodyRadians, float limbRadians, float placeValue, float takeValue);

	std::string mName;

private:
    std::string face[6] = { "/right.png",
	                        "/left.png",
	                        "/top.png",
	                        "/bottom.png",
	                        "/front.png",
	                        "/back.png" };

    std::vector<std::string> mDiffPath ;
};
