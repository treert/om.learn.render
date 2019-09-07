#include "XMesh.h"
using namespace xdata;

XMesh* XMesh::CreateCube()
{
    XMesh* mesh = new XMesh();
    mesh->vertices.push_back(vec3(0, 0, 0));
    mesh->vertices.push_back(vec3(1, 1, 0));
    mesh->vertices.push_back(vec3(0, 1, 0));
    return mesh;
}

