// ------------------------------------------------------
// Project OthoFlow
// OrthoFlow – From Raw Data to Pure Features
//  
// OrthoFlow Load Parameters
// 
// Developer: Guenter Faes, eigennet@faes.de
// GitHub: https://github.com/SuprenumDE/xxxx
// License: MIT
//
// Version 0.0.03, 27.01.2026
// ------------------------------------------------------



#pragma once
#include "FlowParams.h"

class ParamLoader {
public:
    static FlowParams from_cli(int argc, char** argv);
};

