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

#include <clarus/vision/images.hpp>

#include <clarus/vision/colors.hpp>
#include <clarus/vision/filters.hpp>

#include <iostream>

cv::Mat images::absdiff(const cv::Mat &a, const cv::Mat &b) {
    cv::Mat c;
    cv::absdiff(a, b, c);
    return c;
}

cv::Mat images::convert(const cv::Mat &image, int type) {
    cv::Mat converted;
    image.convertTo(converted, type);
    return converted;
}

cv::Mat images::scale(const cv::Mat &image, const cv::Size &size, int interpolation) {
    cv::Mat scaled;
    cv::resize(image, scaled, size, 0, 0, interpolation);
    return scaled;
}

cv::Mat images::load(const std::string &path) {
    cv::Mat image = cv::imread(path);
    if (image.empty()) {
        throw std::ios_base::failure("Could not load \"" + path + "\": file not found");
    }

    return image;
}

cv::Mat images::normalize(const cv::Mat &image) {
    CHANNEL_WISE(images::normalize, image);

    cv::Mat normed;
    cv::normalize(image, normed, 0, 255, cv::NORM_MINMAX, CV_8U);
    return normed;
}

cv::Mat images::load(const std::string &folder, const std::string &name, const std::string &ext) {
    return load(folder + '/' + name + '.' + ext);
}

void images::save(const cv::Mat &data, const std::string &path, bool normalize) {
    cv::Mat pixels = (normalize ? colors::discrete(data) : data);

    cv::imwrite(path, pixels);
}

void images::save(
    const cv::Mat &data,
    const std::string &folder,
    const std::string &name,
    const std::string &ext,
    bool normalize
) {
    save(data, folder + '/' + name + '.' + ext, normalize);
}
