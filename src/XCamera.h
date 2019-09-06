#pragma once

#include "XData.h"
#include "XGameObject.h"

class XCamera:public XGameObject
{
public:
    XCamera();

    float _near;// near and far is used by define
    float _far;
    float fov;


    xdata::mat4 GetViewMat();
    xdata::mat4 GetProjectMat();
};