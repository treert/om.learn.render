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

    typedef typename glm::vec4 Color;

    mat4 GenIdentityMat();
    quat GenIdentityQuat();


    // 几何阶段的输入输出，像素处理阶段的输入，也是光栅化阶段的输出
    struct V2F
    {
        float one;

        vec4 pos;

        vec2 uv;
        vec3 normal;

        int row, col;// 屏幕像素坐标


        V2F() {
            one = 1;
        }

        bool HomogeneousDivision() {
            // 这些是为了后面插值做准备的
            one /= pos.w;
            uv /= pos.w;
            normal /= pos.w;

            pos /= pos.w;

            return pos.x >= -1 && pos.x <= 1 && pos.y >= -1 && pos.y <= 1 && pos.z >= -1 && pos.z <= 1;
        }

        void ScreenMap(int with, int height) {
            pos.x = (pos.x + 1)*with / 2;
            pos.y = (pos.y + 1)*height / 2;
        }

    };

    // 光栅的输入
    struct R_in
    {
        V2F v[3];

        bool valid = false;
        float A2;// 三角形向量面积*2
        vec3 E0, E1, E2;

        void Setup(int with, int height) {
            // 齐次除法
            valid = false;
            valid = valid || v[0].HomogeneousDivision();
            valid = valid || v[1].HomogeneousDivision();
            valid = valid || v[2].HomogeneousDivision();
            if (valid == false) return;// 不在像机里

            // 屏幕映射
            v[0].ScreenMap(with, height);
            v[1].ScreenMap(with, height);
            v[2].ScreenMap(with, height);

            // e(x,y) = ax + by + c
            E0.x = -(v[2].pos.y - v[1].pos.y);
            E0.y = v[2].pos.x - v[1].pos.x;
            E0.z = -E0.x*v[1].pos.x - E0.y*v[1].pos.y;
            E1.x = -(v[0].pos.y - v[2].pos.y);
            E1.y = v[0].pos.x - v[2].pos.x;
            E1.z = -E1.x*v[2].pos.x - E1.y*v[2].pos.y;
            E2.x = -(v[1].pos.y - v[0].pos.y);
            E2.y = v[1].pos.x - v[0].pos.x;
            E2.z = -E2.x*v[0].pos.x - E2.y*v[0].pos.y;

            A2 = E0.z + E1.z + E2.z;
           
        }

        bool TestAndInterpolate(int i, int k, V2F&out) {
            float x = i + 0.5f, y = k + 0.5f;
            vec3 t_p = vec3(x, y, 1);
            float f0 = glm::dot(E0, t_p)*v[0].one;
            float f1 = glm::dot(E1, t_p)*v[1].one;
            float f2 = glm::dot(E2, t_p)*v[2].one;
            if (f0 < 0 || f1 < 0 || f2 < 0) return false;// 不再三角形里

            float u0 = f0 / (f0 + f1 + f2);
            float u1 = f1 / (f0 + f1 + f2);
            float u2 = f2 / (f0 + f1 + f2);

            // interpolate
#define lerp(name) out.name = v[0].name*u0 + v[1].name*u1 + v[2].name*u2
            lerp(pos);// 深度值
            lerp(one);
            lerp(uv);
            lerp(normal);
#undef lerp
            // 得到期望的插值
            out.uv /= out.one;
            out.normal /= out.one;

            out.row = i;
            out.col = k;

            return true;
        }
    };
}