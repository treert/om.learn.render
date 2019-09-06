#include "XCamera.h"
#include "XCommon.h"

using namespace xdata;

XCamera::XCamera():XGameObject()
{
    fov = 45;
    _near = 1;
    _far = 100;
}

xdata::mat4 XCamera::GetViewMat()
{
    mat4 mat = glm::lookAt(pos, pos + GetForwardDir(), GetUpDir());
    return mat;
}

xdata::mat4 XCamera::GetProjectMat()
{
    return glm::perspective(glm::radians(fov), (float)WIDTH / (float)HEIGHT, _near, _far);
}
