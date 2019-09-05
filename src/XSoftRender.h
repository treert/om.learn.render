#pragma once

#include "XFrameBuffer.h"

class XSoftRender
{
private:
    static XSoftRender *_instance;
public:
    static XSoftRender* Instance();

    XSoftRender() {
        frame_buffer = new XFrameBuffer<1280, 720>();
    }

    IFrameBuffer *frame_buffer;
    
};