#include "XSoftRender.h"
using namespace xdata;

XSoftRender *XSoftRender::_instance = nullptr;

XSoftRender& XSoftRender::Instance()
{
    if (_instance == nullptr) {
        _instance = new XSoftRender();
    }
    return *_instance;
}

xdata::V2F XSoftRender::VectexProcess(xdata::V2F in)
{
    return in;
}

void XSoftRender::Raterize(xdata::R_in in, std::vector<xdata::V2F> &out)
{

}

xdata::color XSoftRender::FragmentProcess(xdata::V2F v2f)
{
    return color(1, 0, 0, 1);
}
