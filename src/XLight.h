#pragma once

#include "XData.h"
#include "XGameObject.h"

class XLight:public XGameObject
{
public:
    xdata::Color color;

    XLight():XGameObject() {
        color = xdata::Color(1, 1, 1, 1);
    }
};