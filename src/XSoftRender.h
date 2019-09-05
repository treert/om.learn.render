#pragma once

#include<vector>

#include "XData.h"
#include "XFrameBuffer.h"

class XCamera;
class XGameObject;

class XSoftRender
{
private:
    static XSoftRender *_instance;
public:
    static XSoftRender& Instance();

    XFrameBuffer frame_buffer;

public:
    // äÖÈ¾µÄ¸÷¸ö½×¶Î
    void Render(const XCamera &camera, const XGameObject &scene);
    xdata::V2F VectexProcess(xdata::V2F in);
    void Raterize(xdata::R_in in, std::vector<xdata::V2F> &out);
    xdata::color FragmentProcess(xdata::V2F v2f);
};