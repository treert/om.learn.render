#include "XSoftRender.h"

XSoftRender *XSoftRender::_instance = nullptr;

XSoftRender* XSoftRender::Instance()
{
    if (_instance == nullptr) {
        _instance = new XSoftRender();
    }
    return _instance;
}
