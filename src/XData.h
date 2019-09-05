#pragma once
#include <memory>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

// 一些数据结构

namespace xdata {
    typedef typename glm::vec2 vec2;
    typedef typename glm::vec3 vec3;
    typedef typename glm::vec4 vec4;
    typedef typename glm::quat quat;
    typedef typename glm::mat4 mat4;

    typedef typename glm::vec4 color;

    mat4 GenIdentityMat();
    quat GenIdentityQuat();


    // 几何阶段的输入输出，像素处理阶段的输入，也是光栅化阶段的输出
    struct V2F
    {
        vec4 pos;
        vec2 uv;
        vec3 normal;

        static V2F Lerp(const V2F &a, const V2F &b, float u) 
        {
            V2F ret;
#define lerp(name) ret.name = a.name + u *(b.name - a.name)
            lerp(pos);
            lerp(uv);
            lerp(normal);
#undef lerp
            return ret;
        }
    };

    // 光栅的输入
    struct R_in
    {
        V2F v[3];
    };
}