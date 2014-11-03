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

#include <clarus/io/camera.hpp>
using clarus::Camera;
using std::string;

#include <boost/date_time/posix_time/posix_time.hpp>
namespace pt = boost::posix_time;

#include <boost/function.hpp>

#include <unistd.h>

#include <map>
#include <stdexcept>

Camera::Driver::Driver(int index, double _fps):
    camera(index),
    running(true),
    fps(_fps),
    generation(0)
{
    while (!camera.isOpened()) {
        sleep(1);
    }

    // Hack to avoid the first few frames to be the same.
    for (int i = 0; i < 10; i++) {
        camera.read(frame);
    }

    // Start background thread.
    thread.reset(new boost::thread(&Driver::loop, this));
}

Camera::Driver::~Driver() {
    running = false;
    thread->join();
}

void Camera::Driver::loop() {
    double gap = 1000000.0 / fps;
    pt::ptime last = pt::microsec_clock::local_time();
    for (pt::ptime current = last; running; current = pt::microsec_clock::local_time()) {
        pt::time_duration td = current - last;
        double remaining = gap - td.total_microseconds();
        if (remaining > 0) {
            usleep(remaining);
        }

        last = current;

        /* Enable critical section to the end of this scope. */ {
            boost::unique_lock<boost::shared_mutex> locked(lock);

            camera.read(frame);

            cv::VideoWriter *recorder = videoRecorder.get();
            if (recorder != NULL) {
                recorder->write(frame);
            }

            std::string *title = displayTitle.get();
            if (title != NULL) {
                cv::imshow(*title, frame);
                cv::waitKey(1);
            }

            generation++;
        }

        updated.notify_all();
    }
}

void Camera::Driver::display(const std::string &title) {
    boost::unique_lock<boost::shared_mutex> locked(lock);
    displayTitle.reset(new std::string(title));
}

void Camera::Driver::hide() {
    boost::unique_lock<boost::shared_mutex> locked(lock);
    cv::destroyWindow(*displayTitle);
    displayTitle.reset();
}

void Camera::Driver::record(const std::string &path, int fourcc) {
    boost::unique_lock<boost::shared_mutex> locked(lock);
    videoRecorder.reset(new cv::VideoWriter(path, fourcc, fps, size()));
}

void Camera::Driver::stop() {
    boost::unique_lock<boost::shared_mutex> locked(lock);
    videoRecorder.reset();
}

cv::Mat Camera::Driver::grab() {
    boost::shared_lock<boost::shared_mutex> locked(lock);
    return frame.clone();
}

cv::Mat Camera::Driver::next() {
    boost::shared_lock<boost::shared_mutex> locked(lock);
    uint64_t current = generation;
    while (generation == current) {
        updated.wait(locked);
    }

    return frame.clone();
}

cv::Size Camera::Driver::size() {
    int width = camera.get(CV_CAP_PROP_FRAME_WIDTH);
    int height = camera.get(CV_CAP_PROP_FRAME_HEIGHT);
    return cv::Size(width, height);
}

double Camera::Driver::get(int id) {
    return camera.get(id);
}

void Camera::Driver::set(int id, double value) {
    camera.set(id, value);
}

Camera::Camera() {
    // Nothing to do.
}

Camera::Camera(int index, double fps) {
    open(index, fps);
}

Camera::~Camera() {
    close();
}

void Camera::open(int index, double fps) {
    driver.reset(new Driver(index, fps));
}

void Camera::close() {
    driver.reset();
}

bool Camera::opened() {
    return (driver.get() != NULL);
}

void Camera::display(const string &title) {
    if (!opened()) {
        throw std::runtime_error("Cannot open display on a closed camera");
    }

    driver->display(title);
}

void Camera::hide() {
    if (!opened()) {
        return;
    }

    driver->hide();
}

void Camera::record(const std::string &path, int fourcc) {
    if (!opened()) {
        throw std::runtime_error("Cannot record on a closed camera");
    }

    driver->record(path, fourcc);
}

void Camera::stop() {
    if (!opened()) {
        return;
    }

    driver->stop();
}

cv::Mat Camera::grab() {
    if (!opened()) {
        throw std::runtime_error("Cannot grab frames on a closed camera");
    }

    return driver->grab();
}

cv::Mat Camera::next() {
    if (!opened()) {
        throw std::runtime_error("Cannot grab frames on a closed camera");
    }

    return driver->next();
}

cv::Size Camera::size() {
    if (!opened()) {
        throw std::runtime_error("Cannot get field of view size from a closed camera");
    }

    return driver->size();
}


double Camera::get(int id) {
    if (!opened()) {
        throw std::runtime_error("Cannot get property value from a closed camera");
    }

    return driver->get(id);
}

void Camera::set(int id, double value) {
    if (!opened()) {
        throw std::runtime_error("Cannot set property value to a closed camera");
    }

    driver->set(id, value);
}
