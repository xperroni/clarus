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

#ifndef CLARUS_IO_VIDEO_HPP
#define CLARUS_IO_VIDEO_HPP

#include <boost/smart_ptr.hpp>

#include <opencv2/opencv.hpp>

namespace clarus {
    class Video;
}

class clarus::Video {
    /** \brief \c VideoCapture object used to replay a video. */
    boost::shared_ptr<cv::VideoCapture> video;

    /** Last frame retrieved from a video. */
    cv::Mat frame;

    /** Whether there are still frames to be read. */
    bool left;

public:
    /**
    \brief Default constructor.
    */
    Video();

    /**
    \brief Creates a new video replay object and opens the video file at the given path.
    */
    Video(const std::string &path);

    /**
    \brief Returns the next video frame.
    */
    cv::Mat operator () ();

    /**
    \brief Opens the video file at the given path.
    */
    void open(const std::string &path);

    /**
    \brief Closes the current video file, if opened. Do nothing otherwise.
    */
    void close();

    /**
    \brief Returns the current video frame.
    */
    cv::Mat grab() const;

    /**
    \brief Returns whether there are more frames to be read.
    */
    bool more() const;

    /**
    \brief Returns the next video frame.
    */
    cv::Mat next();
};

#endif
