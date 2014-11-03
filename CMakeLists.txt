cmake_minimum_required(VERSION 2.8)

project(clarus CXX)

include_directories(
    "include"
)

find_package(Boost REQUIRED COMPONENTS filesystem system)
find_package(OpenCV 2.4.8 REQUIRED)

add_library(clarus_core
    "include/clarus/core.hpp"
    "include/clarus/core/bag.hpp"
    "include/clarus/core/context.hpp"
    "include/clarus/core/index.hpp"
    "include/clarus/core/list.hpp"
    "include/clarus/core/list_iterator_const.hpp"
    "include/clarus/core/list_iterator.hpp"
    "include/clarus/core/locker.hpp"
    "include/clarus/core/looper.hpp"
    "include/clarus/core/loop.hpp"
    "include/clarus/core/math.hpp"
    "include/clarus/core/tuple.hpp"
    "include/clarus/core/types.hpp"
    "src/clarus/core/bag.cpp"
    "src/clarus/core/context.cpp"
#    "src/clarus/core/list.cpp"
    "src/clarus/core/locker.cpp"
    "src/clarus/core/loop.cpp"
    "src/clarus/core/looper.cpp"
    "src/clarus/core/math.cpp"
    "src/clarus/core/types.cpp"
)

add_library(clarus_io
    "include/clarus/io.hpp"
    "include/clarus/io/camera.hpp"
    "include/clarus/io/command.hpp"
    "include/clarus/io/console.hpp"
    "include/clarus/io/gnuplot.hpp"
    "include/clarus/io/http.hpp"
    "include/clarus/io/microphone.hpp"
    "include/clarus/io/speech.hpp"
    "include/clarus/io/video.hpp"
    "include/clarus/io/viewer.hpp"
    "src/clarus/io/camera.cpp"
    "src/clarus/io/command.cpp"
    "src/clarus/io/console.cpp"
    "src/clarus/io/gnuplot.cpp"
    "src/clarus/io/http.cpp"
    "src/clarus/io/microphone.cpp"
    "src/clarus/io/speech.cpp"
    "src/clarus/io/video.cpp"
    "src/clarus/io/viewer.cpp"
)

add_library(clarus_model
    "include/clarus/model.hpp"
    "include/clarus/model/ballot_discrete.hpp"
    "include/clarus/model/ballot.hpp"
    "include/clarus/model/ballot_weighted.hpp"
    "include/clarus/model/cloud.hpp"
    "include/clarus/model/cluster.hpp"
    "include/clarus/model/dbscan.hpp"
    "include/clarus/model/line2d.hpp"
    "include/clarus/model/munkres.hpp"
    "include/clarus/model/point.hpp"
    "include/clarus/model/quadtree.hpp"
    "include/clarus/model/ransac.hpp"
    "src/clarus/model/cloud.cpp"
    "src/clarus/model/cluster.cpp"
    "src/clarus/model/dbscan.cpp"
    "src/clarus/model/line2d.cpp"
    "src/clarus/model/munkres.cpp"
    "src/clarus/model/point.cpp"
)

add_library(clarus_vgram
    "include/clarus/vgram.hpp"
    "include/clarus/vgram/bitstring.hpp"
    "include/clarus/vgram/intstring.hpp"
    "include/clarus/vgram/layer.hpp"
    "include/clarus/vgram/network.hpp"
    "include/clarus/vgram/neuron.hpp"
    "include/clarus/vgram/output.hpp"
    "src/clarus/vgram/intstring.cpp"
)

add_library(clarus_vision
    "include/clarus/vision.hpp"
    "include/clarus/vision/bayer.hpp"
    "include/clarus/vision/colors.hpp"
    "include/clarus/vision/cvmat.hpp"
    "include/clarus/vision/depths.hpp"
    "include/clarus/vision/dither.hpp"
    "include/clarus/vision/filters.hpp"
    "include/clarus/vision/fourier.hpp"
    "include/clarus/vision/gaussian.hpp"
    "include/clarus/vision/images.hpp"
    "include/clarus/vision/kernel.hpp"
    "include/clarus/vision/logpolar.hpp"
    "include/clarus/vision/minchinton.hpp"
    "include/clarus/vision/segment.hpp"
    "include/clarus/vision/sparse.hpp"
    "include/clarus/vision/surfer.hpp"
    "src/clarus/vision/bayer.cpp"
    "src/clarus/vision/colors.cpp"
    "src/clarus/vision/cvmat.cpp"
    "src/clarus/vision/depths.cpp"
    "src/clarus/vision/dither.cpp"
    "src/clarus/vision/filters.cpp"
    "src/clarus/vision/fourier.cpp"
    "src/clarus/vision/gaussian.cpp"
    "src/clarus/vision/images.cpp"
    "src/clarus/vision/logpolar.cpp"
    "src/clarus/vision/minchinton.cpp"
    "src/clarus/vision/segment.cpp"
    "src/clarus/vision/sparse.cpp"
    "src/clarus/vision/surfer.cpp"
)

install(
    TARGETS
        clarus_core
        clarus_io
        clarus_model
        clarus_vgram
        clarus_vision
    DESTINATION "/usr/local/lib"
)

install(
    FILES
        "include/clarus/core.hpp"
        "include/clarus/io.hpp"
        "include/clarus/model.hpp"
        "include/clarus/vgram.hpp"
        "include/clarus/vision.hpp"
    DESTINATION "/usr/local/include/clarus"
)

install(
    FILES
        "include/clarus/core/bag.hpp"
        "include/clarus/core/context.hpp"
        "include/clarus/core/index.hpp"
        "include/clarus/core/list.hpp"
        "include/clarus/core/list_iterator_const.hpp"
        "include/clarus/core/list_iterator.hpp"
        "include/clarus/core/locker.hpp"
        "include/clarus/core/looper.hpp"
        "include/clarus/core/loop.hpp"
        "include/clarus/core/math.hpp"
        "include/clarus/core/tuple.hpp"
        "include/clarus/core/types.hpp"
    DESTINATION "/usr/local/include/clarus/core"
)

install(
    FILES
        "include/clarus/io/camera.hpp"
        "include/clarus/io/command.hpp"
        "include/clarus/io/console.hpp"
        "include/clarus/io/gnuplot.hpp"
        "include/clarus/io/http.hpp"
        "include/clarus/io/microphone.hpp"
        "include/clarus/io/speech.hpp"
        "include/clarus/io/video.hpp"
        "include/clarus/io/viewer.hpp"
    DESTINATION "/usr/local/include/clarus/io"
)

install(
    FILES
        "include/clarus/model/ballot_discrete.hpp"
        "include/clarus/model/ballot.hpp"
        "include/clarus/model/ballot_weighted.hpp"
        "include/clarus/model/cloud.hpp"
        "include/clarus/model/cluster.hpp"
        "include/clarus/model/dbscan.hpp"
        "include/clarus/model/line2d.hpp"
        "include/clarus/model/munkres.hpp"
        "include/clarus/model/point.hpp"
        "include/clarus/model/quadtree.hpp"
        "include/clarus/model/ransac.hpp"
    DESTINATION "/usr/local/include/clarus/model"
)

install(
    FILES
        "include/clarus/vgram/bitstring.hpp"
        "include/clarus/vgram/intstring.hpp"
        "include/clarus/vgram/layer.hpp"
        "include/clarus/vgram/network.hpp"
        "include/clarus/vgram/neuron.hpp"
        "include/clarus/vgram/output.hpp"
    DESTINATION "/usr/local/include/clarus/vgram"
)

install(
    FILES
        "include/clarus/vision/bayer.hpp"
        "include/clarus/vision/colors.hpp"
        "include/clarus/vision/cvmat.hpp"
        "include/clarus/vision/depths.hpp"
        "include/clarus/vision/dither.hpp"
        "include/clarus/vision/filters.hpp"
        "include/clarus/vision/fourier.hpp"
        "include/clarus/vision/gaussian.hpp"
        "include/clarus/vision/images.hpp"
        "include/clarus/vision/kernel.hpp"
        "include/clarus/vision/logpolar.hpp"
        "include/clarus/vision/minchinton.hpp"
        "include/clarus/vision/segment.hpp"
        "include/clarus/vision/sparse.hpp"
        "include/clarus/vision/surfer.hpp"
    DESTINATION "/usr/local/include/clarus/vision"
)
