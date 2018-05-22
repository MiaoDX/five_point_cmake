Standalone CMake project for [Nghia Ho's 5 points algorithm](http://nghiaho.com/?p=1675).

It was once one subset of [MiaoDX/pose_estimation](https://github.com/MiaoDX/pose_estimation), for better isolation consideration, here it is.

I personally like using more mature libs for (almost) everything, but it seems that all existing relative pose estimation codes, both in OpenCV and OpenGV fails one way or another. So, at last we come back to this implement. To make this easier to integrate in various scenarios, I make it cmake-friendly and with one almost bare python binding (only with OpenCV).

[Notes from Nghia_Ho website](Notes_of_Nghia_Ho.md)

## USAGE

We use `with_opencv` as example, since it also has python binding, things can be a little complicated.

``` vi
git clone https://github.com/MiaoDX/five_point_cmake.git
cd five_point_cmake
git submodule init # add pybind11 for python binding
git submodule update

cd with_opencv
mkdir build
cd build
cmake-gui ..

<THEN specify PYHTON EXECUTABLE in GUI>
<Configure, Generate, Open Project and BUILD>
```

We will get the corresponding `.pyd` (for WIN32) and `.lib` (for Linux) files in folder `with_opencv/build/python/Release`. Copy that to the python (the one we build for) `Lib\site-packages` folder, then we are done.

Try with `python python/classes_cv.py` for the testing.