#pragma once

#include<vector>

#include "XData.h"
#include "XFrameBuffer.h"
#include "XCamera.h"
#include "XLight.h"
#include "XMesh.h";

class XGameObject;

// 管理了整个渲染状态，uniform类的变量也在其中
class XSoftRender
{
private:
    static XSoftRender *_instance;
public:
    static XSoftRender& Instance();

    XFrameBuffer frame_buffer;

    XCamera camera;
    XLight light;

private:
    xdata::mat4 mat_project;
    xdata::mat4 mat_view;
    xdata::mat4 mat_model;
    xdata::mat4 mat_mvp;

    xdata::vec3 m_light_dir;
    xdata::vec3 m_light_color;
    xdata::vec3 m_view_dir;

public:
    void Test();

    void Render(XGameObject *scene);
    void Render(XGameObject *go, const xdata::mat4 &parent_world_mat);

    void RenderMesh(XMesh *mesh);
    xdata::V2F VectexProcess(xdata::V2F in);
    void RenderTriangle(xdata::R_in in);
    void Raterize(xdata::R_in in, std::vector<xdata::V2F> &out);
    void FragmentProcess(xdata::V2F v2f);
};