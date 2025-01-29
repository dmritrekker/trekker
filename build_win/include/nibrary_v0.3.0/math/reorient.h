#pragma once

#include "base/nibr.h"
#include <string>
#include <functional>

namespace NIBR 
{

typedef enum {
    ORDEROFDIRECTIONS_NOTSET = -1,
    XYZ,XYz,XyZ,Xyz,xYZ,xYz,xyZ,xyz,
    XZY,XZy,XzY,Xzy,xZY,xZy,xzY,xzy,
    YXZ,YXz,YxZ,Yxz,yXZ,yXz,yxZ,yxz,
    YZX,YZx,YzX,Yzx,yZX,yZx,yzX,yzx,
    ZYX,ZYx,ZyX,Zyx,zYX,zYx,zyX,zyx,
    ZXY,ZXy,ZxY,Zxy,zXY,zXy,zxY,zxy
} OrderOfDirections;

OrderOfDirections convertOrderOfDirections(std::string ood);

std::function<void(float *)> reorientFun(OrderOfDirections ood);
std::function<void(float *)> reorientFun(std::string ood);

}