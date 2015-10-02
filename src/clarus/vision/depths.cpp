/*
Copyright (c) Helio Perroni Filho <xperroni@gmail.com>

This file is part of Clarus.

Clarus is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Clarus is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Clarus. If not, see <http://www.gnu.org/licenses/>.
*/

#include <clarus/vision/depths.hpp>

#include <clarus/core/list.hpp>
using clarus::List;

#include <clarus/vision/colors.hpp>
#include <clarus/vision/images.hpp>

#include <fstream>

typedef List<depths::Depth> DepthRow;

typedef List<DepthRow> DepthMap;

cv::Mat depths::load(const std::string &path) {
    DepthMap values;
    std::ifstream file(path.c_str());
    for (char c1 = '\0';;) {
        file >> c1;
        if (c1 == ']') {
            break;
        }

        DepthRow &row = values.append();
        for (char c2 = '\0';;) {
            file >> c2;
            if (c2 == ']') {
                break;
            }

            Depth value = -1;
            file >> value;

            row.append(value);
        }
    }

    int rows = values.size();
    int cols = values[0].size();

    cv::Mat depths(rows, cols, CV_32F, -1.0);
    for (int i = 0; i < rows; i++) {
        const DepthRow &row = values[i];
        for (int j = 0; j < cols; j++) {
            depths.at<Depth>(i, j) = row[j];
        }
    }


    return depths;
}

void depths::save(const cv::Mat &depths, const std::string &path) {
    std::ofstream file(path.c_str());
    save(depths, file);
}

void depths::save(const cv::Mat &depths, std::ostream &out) {
    int rows = depths.rows;
    int cols = depths.cols;

    out << "[";
    for (int i = 0;;) {
        out << "[";
        for (int j = 0;;) {
            out << depths.at<Depth>(i, j);
            if (++j < cols) {
                out << ", ";
            }
            else {
                break;
            }
        }

        if (++i < rows) {
            out << "]," << std::endl;
        }
        else {
            out << "]]" << std::endl;
            return;
        }
    }
}

cv::Mat depths::bgr(const cv::Mat &depths) {
    cv::Mat grays;
    depths.convertTo(grays, CV_64F);
    grays += 1.0;

    cv::log(grays, grays);
    cv::normalize(grays, grays, 0, 255, CV_MINMAX, CV_8U);
    return colors::colormap(grays, cv::COLORMAP_JET);
}

cv::Mat depths::bgr(const cv::Mat &depths, const cv::Size &scale) {
    cv::Mat colored = bgr(depths);
    if (scale.width > 0 && scale.height > 0) {
        colored = images::scale(colored, scale, cv::INTER_NEAREST);
    }

    return colored;
}
