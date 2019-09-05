#include "XGameObject.h"

using namespace xdata;

xdata::mat4 XGameObject::GetMatToParent()
{
    return xdata::GenIdentityMat();
}

void XGameObject::Move(const xdata::vec3 &motion)
{

}

void XGameObject::Rotate(float x, float y, float z)
{

}

void XGameObject::Rotate(const xdata::vec3 &axis, float angle)
{

}
