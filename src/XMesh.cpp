#include "XMesh.h"
#include<math.h>
using namespace xdata;

void GenSphere(XMesh *mesh, vec3 a1, vec3 a2, vec3 a3, int level) {
    if (level > 0) {
        // 继续细分
        level -= 1;
        auto b1 = (a1 + a3)*0.5f;
        auto b2 = (a1 + a2)*0.5f;
        auto b3 = (a2 + a3)*0.5f;
        GenSphere(mesh, a1, b2, b1, level);
        GenSphere(mesh, b1, b2, b3, level);
        GenSphere(mesh, b2, a2, b3, level);
        GenSphere(mesh, b1, b3, a3, level);
        return;
    }

    // 填充顶点和法线
    a1 = glm::normalize(a1);
    a2 = glm::normalize(a2);
    a3 = glm::normalize(a3);

    mesh->vertices.push_back(a1*0.5f);
    mesh->normals.push_back(a1);
    mesh->vertices.push_back(a2*0.5f);
    mesh->normals.push_back(a2);
    mesh->vertices.push_back(a3*0.5f);
    mesh->normals.push_back(a3);
}

XMesh* XMesh::CreateSphere(int level)
{
    XMesh* mesh = new XMesh();

    float sqr2 = sqrt(2);
    float sqr6 = sqrt(6);

    vec3 bps[4] = {
        vec3(0,0,3),
        vec3(-2 * sqr2,0,-1),
        vec3(sqr2,sqr6, -1),
        vec3(sqr2,-sqr6, -1),
    };
    GenSphere(mesh, bps[1], bps[2], bps[0], level);
    GenSphere(mesh, bps[1], bps[0], bps[3], level);
    GenSphere(mesh, bps[1], bps[3], bps[2], level);
    GenSphere(mesh, bps[2], bps[3], bps[0], level);

    return mesh;
}

