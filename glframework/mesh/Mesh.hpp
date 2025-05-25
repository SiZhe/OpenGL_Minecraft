#pragma once
#include "../Object.hpp"
#include "../Geometry.hpp"
#include "../material/Material.hpp"

class Mesh : public Object {
public:
    Mesh();
    Mesh(Geometry* geometry, Material* material);
    ~Mesh();

    Geometry* mGeometry{nullptr};
    Material* mMaterial{nullptr};
};



