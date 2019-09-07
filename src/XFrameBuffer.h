#pragma once
#include <limits>

#include <glad/glad.h>

#include "XCommon.h"
#include "XData.h"

#define rep(i,n) for (int i=0;i<(n);++i)


class XFrameBuffer
{
public:
    float color_buffer[HEIGHT][WIDTH][4];
    float depth_buffer[HEIGHT][WIDTH];

    unsigned int ID;

    XFrameBuffer() {
        ID = 0;
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        ID = texture;
    }

    float GetDepth(int x, int y)
    {
        return depth_buffer[x][y];
    }


    void SetDepth(int x, int y, float depth)
    {
        depth_buffer[x][y] = depth;
    }

    void SetColor(int x, int y, const xdata::Color &color) {
        SetColor(x, y, color.r, color.g, color.b, color.a);
    }

    void SetColor(int x, int y, float r, float g, float b, float a = 0)
    {
        color_buffer[x][y][0] = r;
        color_buffer[x][y][1] = g;
        color_buffer[x][y][2] = b;
        color_buffer[x][y][3] = a;
    }


    void UseInOpenGL()
    {
        GLenum pos = GL_TEXTURE0;
        glActiveTexture(pos);
        glBindTexture(GL_TEXTURE_2D, ID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WIDTH, HEIGHT, 0, GL_RGBA, GL_FLOAT, &color_buffer[0][0][0]);
    }


    void Clear(float r, float g, float b, float a = 0)
    {
        rep(x, HEIGHT )
            rep(y, WIDTH)
        {
            depth_buffer[x][y] = std::numeric_limits<float>::max();
            color_buffer[x][y][0] = r;
            color_buffer[x][y][1] = g;
            color_buffer[x][y][2] = b;
            color_buffer[x][y][3] = a;
        }
    }

};
