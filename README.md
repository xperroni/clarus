Clarus
======

Clarus is a library for machine learning, geared towards computer vision problems. It is developed in C++ 03 on top of [boost](http://www.boost.org/) and [OpenCV](http://opencv.org/). It is divided in several modules:

* `core`: multi-threading facilities, the `Bag` heterogeneous collection, and some other conveniences;
* `io`: facilities for interfacing with sensors (such as cameras) and displaying data;
* `model`: classes and algorithms dealing with high-level (usually geometric) models and concepts;
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

**2015-01-26**

Another small incremental update. The function `operator ()` has been implemented on `clarus::List`, enabling easy construction of subranges, for example:

    List<int> p = (List<int>(), 0, 1, 2, 3, 4, 5, 6, 7, 8, 9);
    List<int> q = p(2, 5); // makes q = [2, 3, 4]

Two changes have been made to the `fourier` functions. First, `fourier::correlate()` now includes a `clip` argument (set to `true` by default) which causes the last `(kernel.height)` rows and `(kernel.width)` columns of the cross-correlation map to be discarded before return. Therefore the only cross-correlation responses returned are those where the kernel was entirely "over" the data matrix. This is much more appropriate for when the cross-correlation map is being used to search for a small image (the kernel) over a larger one (the data matrix).

Second, function `fourier::convolve()` had a bug in the element-wise multiplication step, which was fixed.

**2015-01-12**

A rather underwhelming update, mostly released to support other projects I'm working on and wanted to publish -- the visual processing library [Cight](https://github.com/xperroni/Cight) and its demo application [Dejavu](https://github.com/xperroni/Dejavu).

The `core` module now includes `clarus::max()` and `clarus::argmax()` functions for OpenCV matrices, to save me from looking up the documentation of `cv::minMaxLoc()` every time I want to find the maximum value of a matrix. I also added `clarus::min()` and `clarus::argmin()` just for the sake of consistency, even though strangely I never seem to need finding the minimum of anything. Similarly I've also created a `clarus::pow()` function for element-wise matrix exponentiation.

The list class `clarus::List` now provides an `empty()` method (to match the one provided by OpenCV's `cv::Mat` class) and an implementation of the comma operator to support single-line instantiation and filling. For example, to return an integer list containing four arguments, instead of:

    List<int> l;
    l.append(1);
    l.append(2);
    l.append(3);
    l.append(4);
    return l;

Now you can just write:

    return (List<int>(), 1, 2, 3, 4);

The parentheses around the expression are optional but in my opinion they help clarify what is going on.

On a side note, increasingly I feel the library is in need of an overhaul that addresses problems such as the tension between Clarus and OpenCV basic types (e.g. cv::Point vs. clarus::Point), style inconsistencies (e.g. the `vgram` module's letter case style, or not everything being under the `clarus` namespace) and the hodgepodge of implemented and sometimes semi-abandoned algorithms, especially in the `vision` module. I am also seriously thinking of getting rid of the multithreading classes; I might look into it after porting my working projects to ROS, which I intend to do sometime after February.

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
