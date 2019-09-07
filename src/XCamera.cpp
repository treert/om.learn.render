#include "XCamera.h"
#include "XCommon.h"

using namespace xdata;

XCamera::XCamera():XGameObject()
{
    fov = 45;
    _near = 1;
    _far = 100;

    MovementSpeed = 2.5f;
    MouseSensitivity = 0.1f;
}

xdata::mat4 XCamera::GetViewMat()
{
    mat4 mat = glm::lookAt(pos, pos + GetForwardDir(), GetUpDir());
    return mat;
}

xdata::mat4 XCamera::GetProjectMat()
{
    float aspect = (float)WIDTH / (float)HEIGHT;

    mat4 mat;

    //mat = glm::ortho<float>(-1, 1, -1, 1, 0, _far);
    mat = glm::perspective(glm::radians(fov), aspect, _near, _far);

    return mat;
}
