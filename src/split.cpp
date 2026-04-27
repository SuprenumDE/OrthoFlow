// ------------------------------------------------------
// Project OthoFlow
// OrthoFlow – From Raw Data to Pure Features
// 
// Separating the label column from the features
// 
// Developer: Guenter Faes, eigennet@faes.de
// GitHub: https://github.com/SuprenumDE/xxxx
// License: MIT
//
// Version 0.0.01, 25.02.2026
// ------------------------------------------------------

#include "split.h"
#include <stdexcept>

FeatureLabelSplit Splitter::split(const Eigen::MatrixXd& data, size_t label_column) const
{
    if (label_column >= data.cols()) {
        throw std::runtime_error("Splitter: Label column index out of range.");
    }

    const size_t rows = data.rows();
    const size_t cols = data.cols();

    FeatureLabelSplit result;
    result.y = data.col(label_column);

    result.X.resize(rows, cols - 1);

    size_t dst_col = 0;
    for (size_t src_col = 0; src_col < cols; ++src_col) {
        if (src_col == label_column) continue;
        result.X.col(dst_col++) = data.col(src_col);
    }

    return result;
}
