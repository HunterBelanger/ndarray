# NPArray: NumPy style arrays for C++
A common scenario which arrises in scientific and high performance computing
is the need to conduct calculations or simulations in faster, lower level
languages such as C++, and then load results into a higher level language such
as python to conduct analysis on the results. Depending on the type and amount
of data, it can be difficult and time consuming to write this data to a file in
a specific format, making sure it is consisten, then writing a second python
implementation to reliably read such data. If storing as raw text however, this
can lead to long load times, and large data files.

Currently, there are no small and complete C++ libraries to complete this task.
The xtensor library is a very good option, and I do recommend it. However, it
is a large library which is likely much more than many need, especially in 
smaller projects. Also, being header-only it can lead to long compile times,
and large executables. If you are working on a larger project and need more
NumPy magic than this code provides, I do recommend that you check it out
[here](https://github.com/xtensor-stack/xtensor).

To fill this niche, I have written the NPArray template library, a minimal
implementation of NumPy style arrays for C++. These arrays can be
multi-dimensional, and reshaped like NumPy arrays. Indexing can be done either
with a vector, or as variadic parammeters, both using the () operator. Access
to the data using the linear index is also permitted via the [] operator.

Due to the ```.npy``` file format only supporting certain types, this library
only allows templates of the following type:

* ```NPArray<char>```
* ```NPArray<unsigned char>```
* ```NPArray<uint16_t>```
* ```NPArray<uint32_t>```
* ```NPArray<uint64_t>```
* ```NPArray<int16_t>```
* ```NPArray<int32_t>```
* ```NPArray<int64_t>```
* ```NPArray<float>```
* ```NPArray<double>```

Support for complex types will come in the future.

## Usage
To be written soon...

## Install
This library should be included in a project, and built using CMake. Place the
directory containing this repository into the directory for your project, and
in your CMakeList.txt, add the line ```add_subdirectory(path/to/nparray)```.
Then be sure to link your executable to the ```nparray``` target as well.

Alternatively, you may also build nparray on its own, making the
```libnparray.a``` static library, which you can manually link to.
