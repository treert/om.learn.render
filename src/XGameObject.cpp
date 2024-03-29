#include "XGameObject.h"
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/rotate_normalized_axis.hpp"

using namespace xdata;

XGameObject::XGameObject()
{
    parent = nullptr;
    scale = vec3(1);
    pos = vec3(0);
    eulers = vec3(0);
}

xdata::vec3 XGameObject::GetForwardDir()
{
    xdata::vec4 dir(0, 0, 1, 0);
    dir = GetMatToWorld()*dir;
    return glm::normalize(dir);
}

xdata::vec3 XGameObject::GetUpDir()
{
    xdata::vec4 dir(0, 1, 0, 0);
    dir = GetMatToWorld()*dir;
    return glm::normalize(dir);
}

xdata::vec3 XGameObject::GetRightDir()
{
    xdata::vec4 dir(1, 0, 0, 0);
    dir = GetMatToWorld()*dir;
    return glm::normalize(dir);
}

xdata::mat4 XGameObject::GetMatToParent()
{
    // M = TRS
    mat4 t = glm::translate(mat4(1), pos);
    mat4 r = glm::mat4_cast(quat(glm::radians(eulers)));
    mat4 s = glm::scale(mat4(1), scale);
    return t * r * s ;
}

xdata::mat4 XGameObject::GetMatToWorld()
{
    auto mat = GetMatToParent();
    XGameObject *p = this->parent;
    while (p)
    {
        mat = p->GetMatToParent()*mat;
        p = p->parent;
    }
    return mat;
}

void XGameObject::Move(const xdata::vec3 &motion)
{
    pos += motion;
}

void XGameObject::Move(float x, float y, float z)
{
    Move(vec3(x, y, z));
}

void XGameObject::Rotate(float x, float y, float z)
{
    eulers += vec3(x, y, z);
}

void XGameObject::Rotate(const xdata::vec3 &axis, float angle)
{
    quat qua = quat(glm::radians(eulers));
    qua = glm::rotateNormalizedAxis(qua, glm::radians(angle), glm::normalize(axis));
    eulers = glm::degrees(glm::eulerAngles(qua));
}
