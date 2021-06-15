#include <algorithm>
#include<math.h>

#include "XSoftRender.h"
#include "XGameObject.h"
using namespace xdata;

#define feach(e,s) for(auto e = (s).begin(); e != (s).end(); ++e)
#define rep(i,n) for (int i=0;i<(n);++i)
#define rep2(i,a,b) for (int i=(a);i<(b);++i)

XSoftRender *XSoftRender::_instance = nullptr;

XSoftRender& XSoftRender::Instance()
{
    if (_instance == nullptr) {
        _instance = new XSoftRender();
    }
    return *_instance;
}

void XSoftRender::Render(XGameObject *scene)
{
    assert(scene->parent == nullptr);
    stat_triangle_num = 0;
    stat_pixel_num = 0;

    frame_buffer.Clear(0.2f, 0.3f, 0.3f, 1.0f);
    
    mat_project = camera.GetProjectMat();
    mat_view = camera.GetViewMat();

    m_light_dir = -light->GetForwardDir();

    auto mat = mat4(1);

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
        mat_world2model = glm::inverse(mat);
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
        v2f.normal = mesh->normals[i];
        v2f = VectexProcess(v2f);
        verts.push_back(v2f);
    }
    stat_triangle_num += verts.size() / 3;
    for (int i = 0; i + 2 < verts.size(); i+=3) {
        R_in tri;
        tri.v[0] = verts[i];
        tri.v[1] = verts[i + 1];
        tri.v[2] = verts[i + 2];
        
        RenderTriangle(tri);
    }
}


void XSoftRender::RenderTriangle(xdata::R_in in)
{
    // Triangle Setup
    in.Setup(WIDTH,HEIGHT);
    if (in.TestWholeTriangle() == false) {
        return;// 过滤掉
    }
    // Triangle Travel
    Raterize(in);
}

void XSoftRender::HierarchicalTileTraversal(const xdata::R_in &tri, int l, int r, int b, int t) {
    bool test = tri.TestRectInTriangle(l, r-1, b, t-1);
    if (test == false) return;
    if (r - l >= 4) {
        int ml = (l + r) / 2;
        if (t - b >= 4) {
            int mb = (t + b) / 2;
            HierarchicalTileTraversal(tri, l, ml, b, mb);
            HierarchicalTileTraversal(tri, l, ml, mb, t);
            HierarchicalTileTraversal(tri, ml, r, b, mb);
            HierarchicalTileTraversal(tri, ml, r, mb, t);
        }
        else {
            HierarchicalTileTraversal(tri, l, ml, b, t);
            HierarchicalTileTraversal(tri, ml, r, b, t);
        }
    }
    else if (t - b >= 4) {
        int mb = (t + b) / 2;
        HierarchicalTileTraversal(tri, l, r, b, mb);
        HierarchicalTileTraversal(tri, l, r, mb, t);
    }
    else {
        V2F frag;
        rep2(i, l,r)
            rep2(k, b, t) {
            if (tri.TestAndInterpolate(i, k, frag)) {
                FragmentProcess(frag);
            }
        }
    }
}

float fmin(float a, float b, float c) {
    return std::min(a, std::min(b, c));
}

float fmax(float a, float b, float c) {
    return std::max(a, std::max(b, c));
}

void XSoftRender::Raterize(xdata::R_in in)
{
    // hierarchical tile traversal
    // 算个简单的包围
    float minx = fmin(in.v[0].pos.x, in.v[1].pos.x, in.v[2].pos.x);
    float miny = fmin(in.v[0].pos.y, in.v[1].pos.y, in.v[2].pos.y);
    float maxx = fmax(in.v[0].pos.x, in.v[1].pos.x, in.v[2].pos.x);
    float maxy = fmax(in.v[0].pos.y, in.v[1].pos.y, in.v[2].pos.y);

    int l = std::max(0, (int)floor(minx));
    int r = std::min(WIDTH, (int)ceil(maxx+1));
    int b = std::max(0, (int)floor(miny));
    int t = std::min(HEIGHT, (int)ceil(maxy+1));

    HierarchicalTileTraversal(in, l, r, b, t);

}


xdata::V2F XSoftRender::VectexProcess(xdata::V2F in)
{
    in.pos = mat_mvp * in.pos;
    in.world_pos = mat_model * in.pos;
    in.normal = vec4(in.normal, 0) * mat_world2model;// 法线变换到世界

    // in.normal = mat_model * vec4(in.normal, 0);
    in.normal = glm::normalize(in.normal);
    return in;
}

void XSoftRender::FragmentProcess(xdata::V2F v2f)
{
    // test depth
    auto depth = frame_buffer.GetDepth(v2f.row, v2f.col);
    if (depth < v2f.pos.z) {
        return;
    }
    else 
    {
        frame_buffer.SetDepth(v2f.row, v2f.col, v2f.pos.z);
    }
    stat_pixel_num++;
    // calc color
    // Half Lambert & Blinn-Phong
    Color color = Color(1, 0, 0, 1);
    {
        vec3 color_ambient = light->ambient;
        vec3 color_light = light->color;
        vec3 color_specular = light->specular;
        vec3 color_albedo = Color(1, 1, 1, 1);

        vec3 ambient = color_ambient * 0.1f;

        vec3 normal = glm::normalize(v2f.normal);
        vec3 light_dir = m_light_dir;
        
        vec3 diffuse = color_light * color_albedo * (glm::dot(normal, light_dir)*0.5f + 0.5f);

        vec3 viewDir = glm::normalize(camera.pos - v2f.world_pos);
        vec3 halfDir = glm::normalize(light_dir + viewDir);

        vec3 specular = color_light * color_specular * (float) pow(std::max(0.0f, glm::dot(normal, halfDir)), 32);
        // vec3 specular = color_light * color_specular * glm::clamp(glm::dot(normal, halfDir)*100 - 97,0.0f,1.0f);

        color = vec4(ambient + diffuse + specular, 1);
        // color = vec4(ambient + diffuse, 1);
    }
    // Merge
    frame_buffer.SetColor(v2f.row, v2f.col, color);
}
