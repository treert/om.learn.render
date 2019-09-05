#pragma once
#include <limits>

#include <glad/glad.h>

#define rep(i,n) for (int i=0;i<(n);++i)


class IFrameBuffer
{
public:
    virtual float GetDepth(int x,int y) = 0;
    virtual void SetDepth(int x, int y, float depth) = 0;
    virtual void SetColor(int x, int y, float r, float g, float b, float a = 0) = 0;
    virtual void Clear(float r, float g, float b, float a = 0) = 0;

    virtual void UseInOpenGL() = 0;
};

template<int W, int H>
class XFrameBuffer:public IFrameBuffer
{
public:
    float color_buffer[W][H][4];
    float depth_buffer[W][H];

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

    virtual float GetDepth(int x, int y) override
    {
        return depth_buffer[x][y];
    }


    virtual void SetDepth(int x, int y, float depth) override
    {
        depth_buffer[x][y] = depth;
    }


    virtual void SetColor(int x, int y, float r, float g, float b, float a = 0) override
    {
        color_buffer[x][y][0] = r;
        color_buffer[x][y][1] = g;
        color_buffer[x][y][2] = b;
        color_buffer[x][y][3] = a;
    }


    virtual void UseInOpenGL() override
    {
        GLenum pos = GL_TEXTURE0;
        glActiveTexture(pos);
        glBindTexture(GL_TEXTURE_2D, ID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, W, H, 0, GL_RGBA, GL_FLOAT, &color_buffer[0][0][0]);
    }


    virtual void Clear(float r, float g, float b, float a = 0) override
    {
        rep(x, W)
            rep(y, H) 
        {
            depth_buffer[x][y] = std::numeric_limits<float>::max();
            color_buffer[x][y][0] = r;
            color_buffer[x][y][1] = g;
            color_buffer[x][y][2] = b;
            color_buffer[x][y][3] = a;
        }
    }

};
