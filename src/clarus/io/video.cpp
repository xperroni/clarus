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

#include <clarus/io/video.hpp>
using clarus::Video;

Video::Video() {
    // Nothing to do.
}

Video::Video(const std::string &path) {
    open(path);
}

cv::Mat Video::operator () () {
    return next();
}

void Video::open(const std::string &path) {
    video.reset(new cv::VideoCapture(path));
    next();
}

void Video::close() {
    video.reset();
}

cv::Mat Video::grab() {
    return frame.clone();
}

bool Video::more() {
    return left;
}

cv::Mat Video::next() {
    cv::Mat previous = frame.clone();

    cv::Mat latest;
    left = video->read(latest);
    if (left) {
        frame = latest;
    }

    return previous;
}
