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

#ifndef CLARUS_IO_CAMERA_HPP
#define CLARUS_IO_CAMERA_HPP

#include <boost/noncopyable.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/thread.hpp>

#include <opencv2/opencv.hpp>

#include <string>

namespace clarus {
    class Camera;
}

class clarus::Camera {
    /** \brief Internal camera structures. */
    class Driver: public boost::noncopyable {
        /** \brief Camera object manipulated in the background thread. */
        cv::VideoCapture camera;

        /** \brief Flag used to signal when the background thread should terminate. */
        bool running;

        /** \brief Wait between frame captures, in microseconds. */
        double fps;

        /** \brief Counter used to tell apart one frame from the next. */
        uint64_t generation;

        /** \brief Background thread running the camera frame acquisition loop. */
        boost::shared_ptr<boost::thread> thread;

        /** \brief Mutex used to control thread access to this state. */
        boost::shared_mutex lock;

        /** \brief Condition variable used to signal frame updates. */
        boost::condition_variable_any updated;

        /** \brief Last read camera frame. */
        cv::Mat frame;

        /** \brief Window title for the display option (if NULL, captured frames are not displayed). */
        boost::shared_ptr<std::string> displayTitle;

        /** \brief Video recorder (if NULL, video is not recorded). */
        boost::shared_ptr<cv::VideoWriter> videoRecorder;

        /** \brief Camera loop function executed in a background thread. */
        void loop();

    public:
        /** \brief Smart pointer type alias. */
        typedef boost::shared_ptr<Driver> P;

        /**
        \brief Creates a new state for the camera of given index, with given frames per second parameter.
        */
        Driver(int index, double fps);

        /**
        \brief Object destructor.
        */
        ~Driver();

        void display(const std::string &title);

        void hide();

        void record(const std::string &path, int fourcc);

        void stop();

        cv::Mat grab();

        cv::Mat next();

        cv::Size size();

        double get(int id);

        void set(int id, double value);
    };

    /** Driver object used to control the camera hardware. */
    Driver::P driver;

public:
    /**
    \brief Default constructor.
    */
    Camera();

    /**
    \brief Creates and opens a camera of given index, with given FPS parameter.
    */
    Camera(int index, double fps = 20.0);

    /**
    \brief Object destructor. Enforces polymorphism. Do not remove.
    */
    virtual ~Camera();

    void open(int index = 0, double fps = 20.0);

    void close();

    bool opened() const;

    void display(const std::string &title = std::string("Camera"));

    void hide();

    void record(const std::string &path, int fourcc = CV_FOURCC('M','P','E','G'));

    void stop();

    cv::Mat grab();

    cv::Mat next();

    cv::Size size();

    double get(int id);

    void set(int id, double value);
};

#endif
