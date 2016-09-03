cmake_minimum_required(VERSION 2.8.3)
project(clarus)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)
find_package(Boost REQUIRED COMPONENTS filesystem system)
find_package(OpenCV 2.4.10 REQUIRED)

# Add FFTW support
list(APPEND FFTW3_LIBS fftw3)
add_library(fftw3 SHARED IMPORTED)
set_target_properties(fftw3 PROPERTIES IMPORTED_LOCATION
  "/usr/lib/x86_64-linux-gnu/libfftw3.so"
)

# Add threaded FFTW support
find_package(Threads)
list(APPEND FFTW3_LIBS fftw3_threads)
add_library(fftw3_threads SHARED IMPORTED)
set_target_properties(fftw3_threads PROPERTIES IMPORTED_LOCATION
  "/usr/lib/x86_64-linux-gnu/libfftw3_threads.so"
)

## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend and a run_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependencies might have been
##     pulled in transitively but can be declared for certainty nonetheless:
##     * add a build_depend tag for "message_generation"
##     * add a run_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   std_msgs  # Or other packages containing msgs
# )

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if you package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
  INCLUDE_DIRS include
  # Mind the order of the libraries, must be listed in inverse order of dependencies
  LIBRARIES clarus_io clarus_vgram clarus_vision clarus_model clarus_fftw clarus_core
  DEPENDS OpenCV boost
#    CATKIN_DEPENDS other_catkin_pkg
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
# include_directories(include)
# TODO: Check names of system library include directories (OpenCV, boost)
include_directories(
  ${Boost_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  ${boost_INCLUDE_DIRS}
  "include/"
)

## Declare a cpp library
# add_library(clarus
#   src/${PROJECT_NAME}/clarus.cpp
# )

add_library(clarus_core
  "src/clarus/core/bag.cpp"
#  "src/clarus/core/list.cpp"
  "src/clarus/core/math.cpp"
  "src/clarus/core/matrix.cpp"
  "src/clarus/core/types.cpp"
)

add_library(clarus_fftw
  "src/clarus/fftw/buffer.cpp"
  "src/clarus/fftw/correlate.cpp"
  "src/clarus/fftw/cosine_search.cpp"
  "src/clarus/fftw/plan.cpp"
  "src/clarus/fftw/signal.cpp"
  "src/clarus/fftw/signal_domain_c.cpp"
  "src/clarus/fftw/signal_domain_r.cpp"
  "src/clarus/fftw/signals.cpp"
)

add_library(clarus_io
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
  "src/clarus/model/cloud.cpp"
  "src/clarus/model/cluster.cpp"
  "src/clarus/model/dbscan.cpp"
  "src/clarus/model/line2d.cpp"
  "src/clarus/model/munkres.cpp"
  "src/clarus/model/point.cpp"
)

add_library(clarus_vgram
  "src/clarus/vgram/intstring.cpp"
)

add_library(clarus_vision
  "src/clarus/vision/bayer.cpp"
  "src/clarus/vision/colors.cpp"
  "src/clarus/vision/cvmat.cpp"
  "src/clarus/vision/depths.cpp"
  "src/clarus/vision/dither.cpp"
  "src/clarus/vision/filters.cpp"
  "src/clarus/vision/fourier.cpp"
  "src/clarus/vision/gaussian.cpp"
  "src/clarus/vision/images.cpp"
  "src/clarus/vision/integral.cpp"
  "src/clarus/vision/logpolar.cpp"
  "src/clarus/vision/minchinton.cpp"
  "src/clarus/vision/segment.cpp"
  "src/clarus/vision/sparse.cpp"
#  "src/clarus/vision/surfer.cpp"
)

## Declare a cpp executable
# add_executable(clarus_node src/clarus_node.cpp)

## Add cmake target dependencies of the executable/library
## as an example, message headers may need to be generated before nodes
# add_dependencies(clarus_node clarus_generate_messages_cpp)

## Specify libraries to link a library or executable target against
# target_link_libraries(clarus_node
#   ${catkin_LIBRARIES}
#   ${Boost_LIBRARIES}
#   ${OpenCV_LIBRARIES}
#   ${boost_LIBRARIES}
# )

target_link_libraries(clarus_fftw
 ${FFTW3_LIBS}
)

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# install(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables and/or libraries for installation
# install(TARGETS clarus clarus_node
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

install(
  TARGETS
    clarus_core
    clarus_fftw
    clarus_io
    clarus_model
    clarus_vgram
    clarus_vision
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)

## Mark cpp header files for installation
install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.hpp"
  PATTERN ".svn" EXCLUDE
)

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_clarus.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
