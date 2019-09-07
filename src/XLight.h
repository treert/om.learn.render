#pragma once

#include "XData.h"
#include "XGameObject.h"

class XLight:public XGameObject
{
public:
    xdata::Color color;
    xdata::Color ambient;
    xdata::vec3 specular;

    XLight():XGameObject() {
        color = xdata::Color(1, 1, 1, 1)*0.8f;
        ambient = xdata::Color(0, 0, 0, 1);
        specular = color;
        //specular = xdata::Color(0, 1, 0, 1);
    }
};