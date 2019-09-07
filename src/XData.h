#pragma once
#include <memory>

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

// һЩ���ݽṹ

namespace xdata {
    typedef typename glm::vec2 vec2;
    typedef typename glm::vec3 vec3;
    typedef typename glm::vec4 vec4;
    typedef typename glm::quat quat;
    typedef typename glm::mat4 mat4;

    typedef typename glm::vec4 Color;


    // ���ν׶ε�������������ش���׶ε����룬Ҳ�ǹ�դ���׶ε����
    struct V2F
    {
        float one;

        vec4 pos;

        vec2 uv;
        vec3 normal;

        vec3 world_pos;

        int row, col;// ��Ļ��������


        V2F() {
            one = 1;
        }

        void HomogeneousDivision() {
            // ��Щ��Ϊ�˺����ֵ��׼����
            one /= pos.w;
            uv /= pos.w;
            normal /= pos.w;
            world_pos /= pos.w;

            pos /= pos.w;
        }

        void ScreenMap(int with, int height) {
            pos.x = (pos.x + 1)*with / 2;
            pos.y = (pos.y + 1)*height / 2;
        }

    };

    // ��դ������
    struct R_in
    {
        V2F v[3];

        float A2;// �������������*2
        vec3 E0, E1, E2;

        void Setup(int with, int height) {
            // ��γ���
            v[0].HomogeneousDivision();
            v[1].HomogeneousDivision();
            v[2].HomogeneousDivision();

            // ��Ļӳ��
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

        bool TestAndInterpolate(int col, int row, V2F&out) const{
            float x = col + 0.5f, y = row + 0.5f;
            vec3 t_p = vec3(x, y, 1);
            float f0 = glm::dot(E0, t_p)*v[0].one;
            float f1 = glm::dot(E1, t_p)*v[1].one;
            float f2 = glm::dot(E2, t_p)*v[2].one;
            if (f0 < 0 || f1 < 0 || f2 < 0) return false;// ������������

            float u0 = f0 / (f0 + f1 + f2);
            float u1 = f1 / (f0 + f1 + f2);
            float u2 = f2 / (f0 + f1 + f2);

            // interpolate
#define lerp(name) out.name = v[0].name*u0 + v[1].name*u1 + v[2].name*u2
            lerp(pos);// ���ֵ
            lerp(one);
            lerp(uv);
            lerp(normal);
            lerp(world_pos);
#undef lerp
            // �õ������Ĳ�ֵ
            out.uv /= out.one;
            out.normal /= out.one;
            out.world_pos /= out.one;

            out.row = row;
            out.col = col;

            return true;
        }

        bool TestRectInTriangle(int l, int r, int b, int t) const {
            vec3 point[2][2] = {
                {
                    vec3(l+0.5f,b + 0.5f,1),
                    vec3(r + 0.5f,b + 0.5f,1),
                },
                {
                    vec3(l + 0.5f,t + 0.5f,1),
                    vec3(r + 0.5f,t + 0.5f,1),
                }
            };
            return _TestRectInHalfPlane(E0, point)
                && _TestRectInHalfPlane(E1, point)
                && _TestRectInHalfPlane(E2, point);
        }

        bool _TestRectInHalfPlane(const vec3 &plane, const vec3 point[2][2] ) const {
            int i = plane.y < 0 ? 0 : 1;
            int k = plane.x < 0 ? 0 : 1;

            float e = glm::dot(plane, point[i][k]);
            return e >= 0;
        }
    };
}