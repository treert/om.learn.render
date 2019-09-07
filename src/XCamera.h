#pragma once

#include "XData.h"
#include "XGameObject.h"


class XCamera:public XGameObject
{
public:
    XCamera();

    enum MovementDir{
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT
    };

    float _near;// near and far is used by define
    float _far;
    float fov;

    // Camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;


    xdata::mat4 GetViewMat();
    xdata::mat4 GetProjectMat();

    void ProcessKeyboard(MovementDir direction, float deltaTime) {
        float velocity = MovementSpeed * deltaTime;
        if (direction == FORWARD)
            pos += GetForwardDir() * velocity;
        if (direction == BACKWARD)
            pos -= GetForwardDir() * velocity;
        if (direction == LEFT)
            pos -= GetRightDir() * velocity;
        if (direction == RIGHT)
            pos += GetRightDir() * velocity;
    }

    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        eulers.y += xoffset;
        eulers.x += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (eulers.x > 89.0f)
                eulers.x = 89.0f;
            if (eulers.x < -89.0f)
                eulers.x = -89.0f;
        }

    }

    void ProcessMouseScroll(float yoffset)
    {
        fov -= yoffset;
        fov = glm::clamp(fov, 1.0f, 45.0f);
    }
};