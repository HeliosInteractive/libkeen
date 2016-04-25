# libkeen
Keen IO write-only library with caching support

## Building
You need [CMake](https://cmake.org/) to build this project. Currently only Windows is supported and Visual Studio 2015 has been used to develop and test this library. Everything needed to build this library is checked in. Dependencies are:

 - [standalone ASIO](http://think-async.com/) - for thread pool and IO operations
 - [SQLite3](http://www.sqlite.org/) - for caching failed events
 - [LibCURL](https://curl.haxx.se/libcurl/) - for network IO
 - [Catch](https://github.com/philsquared/Catch) - for unit tests

To build the library:

```BASH
cd <libkeen root>
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## Unit Tests
Tests can be invoked via [CTest](https://cmake.org/cmake/help/v3.0/manual/ctest.1.html). In Visual Studio for example, you may run tests by right clicking on `RUN_TESTS` target and building it. Make sure you have `libkeen.dll` under `tests/` so shared library test scenarios can run.

## Language bindings

 - [Unity3D C#](bindings/unity)
 - [Node.JS JS](bindings/node)
