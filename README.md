Clarus
======

Clarus is a library for machine learning, geared towards computer vision problems. It is developed in C++ 03 on top of [boost](http://www.boost.org/) and [OpenCV](http://opencv.org/). It is divided in several modules:

* `core`: multi-threading facilities, the `Bag` heterogeneous collection, and some other conveniences;
* `model`: classes and algorithms dealing with high-level (usually geometric) models and concepts;
* `io`: facilities for interfacing with sensors (such as cameras) and displaying data;
* `vgram`: an API for working with [VG-RAM Weightless Neural Networks](https://github.com/xperroni/Yamabiko/tree/master/2013-2);
* `vision`: operations on images.

Build & Install
---------------

Clarus is built using [cmake](http://www.cmake.org/). To build the project as a set of static libraries open a terminal window and type the commands below:

    $ cd $CLARUS_SOURCE/build/release
    $ cmake -DCMAKE_BUILD_TYPE=Release ../..
    $ make -j2
    $ sudo make install

If you'd rather have shared libraries, change the `cmake` command to:

    $ cmake -DBUILD_SHARED_LIBS:BOOL=ON -DCMAKE_BUILD_TYPE=Release ../..

Under Bash-compatible environments, the scripts `build.sh` and `clean.sh` can be used to quickly build the project and clean the compilation environment.

Version History
---------------

**2014-11-03**

After procrastinating for a long time I finally found the will to bite the bullet and move Clarus to a cross-platform build system. I ended up not using autotools though, going for [cmake](http://www.cmake.org/) instead. Along the way I documented some classes using [Doxygen](http://www.doxygen.org) notation (though there's still a long way to go), and also changed the project's file hierarchy, dividing source files between source and header trees. In fact I'm not convinced this last change was a good idea -- for one it makes harder to switch between a source file and its header, now that I dumped Code::Blocks for Kate -- so I may revisit this decision later on.

TO DO
-----

* Provide a script to generate system documentation from Doxygen-annotated files;
* Complete the in-source API documentation;
* Move the code base to C++ 11, when support is widespread enough.

Trivia
------

The library's name comes from the [Phidippus clarus spider](http://en.wikipedia.org/wiki/Phidippus_clarus), which was found to have a [huge brain](http://news.nationalgeographic.com/news/2011/12/111219-spiders-big-brains-bodies-legs-webs-animals-science/) relative to its body size. It is my hope that, like the spider, Clarus the library won't look like much at first sight, but reveal a deeper side over time.
