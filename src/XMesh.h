#pragma once

#include <vector>

#include "XData.h"

class XMesh
{
public:
    std::vector<xdata::vec3> vertices;
    std::vector<xdata::vec2> uvs;
    std::vector<xdata::vec3> normals;

    static XMesh* CreateSphere(int level = 5);
};