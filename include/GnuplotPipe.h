// ------------------------------------------------------
// Project OthoFlow
// OrthoFlow – From Raw Data to Pure Features
//  
// OrthoFlow GNUPlot-Pipe 
// 
// Developer: Guenter Faes, eigennet@faes.de
// GitHub: https://github.com/SuprenumDE/xxxx
// License: MIT
//
// Version 0.0.01, 26.03.2026
// ------------------------------------------------------



#pragma once
#include <cstdio>
#include <cstdarg>

class GnuplotPipe {
    FILE* gp = nullptr;

public:
    GnuplotPipe() {
        gp = _popen("gnuplot -persistent", "w");
    }

    ~GnuplotPipe() {
        if (gp) _pclose(gp);
    }

    void cmd(const char* fmt, ...) {
        if (!gp) return;
        va_list args;
        va_start(args, fmt);
        vfprintf(gp, fmt, args);
        va_end(args);
        fprintf(gp, "\n");
    }
};
#pragma once
