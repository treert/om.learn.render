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
    XLight *light;

    int stat_triangle_num = 0;
    int stat_pixel_num = 0;

public:
private:
    xdata::mat4 mat_project;
    xdata::mat4 mat_view;
    xdata::mat4 mat_model;
    xdata::mat4 mat_world2model;// mat_model的逆矩阵
    xdata::mat4 mat_mvp;

    xdata::vec3 m_light_dir;

public:
    void Render(XGameObject *scene);
    void Render(XGameObject *go, const xdata::mat4 &parent_world_mat);

    void RenderMesh(XMesh *mesh);
    xdata::V2F VectexProcess(xdata::V2F in);
    void HierarchicalTileTraversal(const xdata::R_in &tri, int l, int r, int b, int t);
    void RenderTriangle(xdata::R_in in);
    void Raterize(xdata::R_in in);
    void FragmentProcess(xdata::V2F v2f);
};