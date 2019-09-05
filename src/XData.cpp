#include "XData.h"

#include <glm/gtc/matrix_transform.hpp>
namespace xdata {



    xdata::mat4 GenIdentityMat()
    {
        return glm::identity<mat4>();
    }

    xdata::quat GenIdentityQuat()
    {
        return glm::identity<quat>();
    }

}