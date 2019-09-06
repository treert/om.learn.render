#include "XSoftRender.h"
#include "XGameObject.h"
using namespace xdata;

#define feach(e,s) for(auto e = (s).begin(); e != (s).end(); ++e)
#define rep(i,n) for (int i=0;i<(n);++i)

XSoftRender *XSoftRender::_instance = nullptr;

XSoftRender& XSoftRender::Instance()
{
    if (_instance == nullptr) {
        _instance = new XSoftRender();
    }
    return *_instance;
}

void XSoftRender::Test()
{
    XGameObject *root = XGameObject::Create();
    root->mesh.reset(XMesh::CreateCube());
    root->Move(1, 2, 3);

    this->Render(root);
}

void XSoftRender::Render(XGameObject *scene)
{
    assert(scene->parent == nullptr);

    frame_buffer.Clear(0.2f, 0.3f, 0.3f, 1.0f);
    
    mat_project = camera.GetProjectMat();
    mat_view = camera.GetViewMat();

    m_light_color = light.color;
    m_light_dir = -light.GetForwardDir();
    m_view_dir = -camera.GetForwardDir();

    auto mat = GenIdentityMat();

    Render(scene, mat);

    frame_buffer.UseInOpenGL();
}

void XSoftRender::Render(XGameObject *go, const xdata::mat4 &parent_world_mat)
{
    mat4 mat = parent_world_mat * go->GetMatToParent();

    feach(it, go->childs) {
        Render(*it, mat);
    }
    if (go->mesh) {
        // render mesh
        mat_model = mat;
        mat_mvp = mat_project * mat_view*mat_model;
        RenderMesh(go->mesh.get());
    }
}

void XSoftRender::RenderMesh(XMesh *mesh)
{
    // 几何变换
    std::vector<V2F> verts;
    rep(i, mesh->vertices.size()) {
        V2F v2f;
        v2f.pos = vec4(mesh->vertices[i], 1);
        //v2f.uv = mesh->uvs[i];
        //v2f.normal = mesh->normals[i];
        v2f = VectexProcess(v2f);
        verts.push_back(v2f);
    }

    for (int i = 0; i + 2 < verts.size(); i+=3) {
        R_in tri;
        tri.v[0] = verts[i];
        tri.v[1] = verts[i + 1];
        tri.v[2] = verts[i + 2];
        
        RenderTriangle(tri);
    }
}

xdata::V2F XSoftRender::VectexProcess(xdata::V2F in)
{
    in.pos = mat_mvp * in.pos;
    return in;
}

void XSoftRender::RenderTriangle(xdata::R_in in)
{
    // Triangle Setup
    in.Setup(WIDTH,HEIGHT);
    //if (in.A2 <= 0 || in.valid == false) {
    //    return;// 过滤掉
    //}
    // Triangle Travel
    std::vector<xdata::V2F> frags;
    Raterize(in, frags);
    // Pixel
    feach(it, frags) {
        FragmentProcess(*it);
    }
}

void XSoftRender::Raterize(xdata::R_in in, std::vector<xdata::V2F> &out)
{
    V2F frag;
    // 先写个极端暴力的
    rep(i, WIDTH)
        rep(k, HEIGHT) {
        if (in.TestAndInterpolate(i, k, frag)) {
            FragmentProcess(frag);
        }
    }
}

void XSoftRender::FragmentProcess(xdata::V2F v2f)
{
    // test depth
    auto depth = frame_buffer.GetDepth(v2f.row, v2f.col);
    if (depth < v2f.pos.z) {
        return;
    }
    else {
        frame_buffer.SetDepth(v2f.row, v2f.col, v2f.pos.z);
    }
    // calc color
    Color color = Color(1, 0, 0, 1);
    // Merge
    frame_buffer.SetColor(v2f.row, v2f.col, color);
}
