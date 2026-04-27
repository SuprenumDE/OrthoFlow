// ------------------------------------------------------
// Project OthoFlow
// OrthoFlow – From Raw Data to Pure Features
//  
// OrthoFlow Plot Utility-Functions 
// 
// Developer: Guenter Faes, eigennet@faes.de
// GitHub: https://github.com/SuprenumDE/xxxx
// License: MIT
//
// Version 0.0.02, 27.03.2026
// ------------------------------------------------------


#pragma once
#include "GnuplotPipe.h"
#include <Eigen/Dense>
#include <string>
#include <algorithm>

inline void plot_scree(const Eigen::VectorXd& s,
    const std::string& out_path,
    int k_selected,
    double svd_threshold,
    double variance_explained) {

    GnuplotPipe gp;

    gp.cmd("set terminal pngcairo size 900,600");
    gp.cmd("set output '%s'", out_path.c_str());

    gp.cmd("set title 'Cumulative Variance Scree Plot' font ',16' offset 0,-1");
    gp.cmd("set xlabel 'Component' font ',12'");
    gp.cmd("set ylabel 'Cumulative variance' font ',12'");
    gp.cmd("set grid");

    // --- Dynamische Y-Achse bestimmen ---
    double total = s.array().square().sum();
    if (total == 0.0) return; // Schutz gegen Division durch 0

    double cum = 0.0;
    double min_y = 1.0;

    for (int i = 0; i < s.size(); ++i) {
        cum += s(i) * s(i);
        double y = cum / total;
        min_y = std::min(min_y, y);
    }

    // Untere Grenze dynamisch bestimmen
    double y_lower = std::min(min_y, svd_threshold) - 0.05;
    if (y_lower < 0.0) y_lower = 0.0;

    gp.cmd("set yrange [%f:1.0]", y_lower);
    gp.cmd("set format y '%%.2f'");

    // X-Achse dynamisch
    gp.cmd("set xrange [1:%d]", s.size() + 1);
    gp.cmd("set xtics 1");
    gp.cmd("set mxtics 2");

    // Modern line style
    gp.cmd("set style line 1 lc rgb '#5e3c99' lt 1 lw 2 pt 7 ps 1.2");

    // Threshold lines
    gp.cmd("set arrow from 1,%f to %d,%f nohead lc rgb '#d73027' lw 2 dt 2",
        svd_threshold, s.size(), svd_threshold);

    gp.cmd("set arrow from %d,%f to %d,1.0 nohead lc rgb '#4575b4' lw 2 dt 2",
        k_selected, y_lower, k_selected);

    // Intersection point
    gp.cmd("set object circle at %d,%f size 0.03 fc rgb '#000000' fs solid",
        k_selected, variance_explained);

    // Labels
    gp.cmd("set label 'k = %d' at %d, graph 0.05 center font ',12'",
        k_selected, k_selected);

    gp.cmd("set label 'Threshold = %.2f' at graph 0.02, %f left font ',12'",
        svd_threshold, svd_threshold);

    // Plot
    gp.cmd("plot '-' using 1:2 with steps ls 1 notitle");


    // Daten streamen
    cum = 0.0;
    for (int i = 0; i < s.size(); ++i) {
        cum += s(i) * s(i);
        gp.cmd("%d %f", i + 1, cum / total);
    }

    gp.cmd("e");
}

