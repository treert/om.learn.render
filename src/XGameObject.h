#pragma once

#include "XData.h"
#include<list>
#include <assert.h>

class XMesh;

class XGameObject
{
protected:
    XGameObject();
public:
    static XGameObject*Create() {
        return new XGameObject();
    }

    static void Destroy(XGameObject *go) {
        go->RemoveFromParent();
        // 递归删掉所有儿子
        
        for (auto it = go->childs.begin(); it != go->childs.begin();it++) {
            (*it)->parent = nullptr;// 避免遍历时修改list,反正最后统一删除掉。
            Destroy(*it);
        }
        delete go;
    }
public:
#pragma region tree
    XGameObject *parent;
    std::list<XGameObject*> childs;

    void SetParent(XGameObject *parent) 
    {
        // 安全检查，不能把儿子设置成父亲。
        auto *tmp = parent;
        while (tmp)
        {
            if (tmp == this) throw "will has circle";
            tmp = tmp->parent;
        }

        RemoveFromParent();

        this->parent = parent;
        parent->childs.push_back(this);
    }

    void RemoveFromParent() {
        if (parent) {
            for (auto iter = parent->childs.begin(); iter != parent->childs.end(); ++iter)
            {
                if (*iter == this) {
                    parent->childs.erase(iter);
                    break;
                }
            }
            parent = nullptr;
        }
    }

#pragma endregion

    std::shared_ptr<XMesh> mesh;

    xdata::vec3 pos;
    xdata::vec3 scale;
    xdata::vec3 eulers;
    // xdata::quat rotate;

    xdata::vec3 GetForwardDir();
    xdata::vec3 GetUpDir();
    xdata::vec3 GetRightDir();
    xdata::mat4 GetMatToParent();
    xdata::mat4 GetMatToWorld();
    void Move(const xdata::vec3 &motion);
    void Move(float x, float y, float z);
    void Rotate(float x, float y, float z);
    void Rotate(const xdata::vec3 &axis, float angle);
};