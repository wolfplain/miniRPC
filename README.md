# Project Title

miniRPC

## Description

miniRPC is a remote procedure call service using Websocket as the network protocol.


## Getting Started

### Dependencies

* Websocket doesn't depend on any open source, it's a good learning template for self develop Websocket server.
* Using Json format as the message format, the server uses [rapidjson](https://github.com/miloyip/rapidjson/)  to process Json message.
* Using [googletest](https://code.google.com/p/googletest/) as the unit test framework.

### Installing

* all the source headers and libraries are in the `./build/miniRPC/include` and `./build/miniRPC/lib` directory.

### Executing program

* go into the miniRPC directory, and run ./build.sh. it will automatically create a build directory.
  all of the headers and libraries will be installed into this directory.

```
cd ./miniRPC
./build.sh
```

* run test program
```
./build/tests/tests/ut-miniRPC --gtest_filter=*
```

* run example program to check how to use minirpc. after execute example binary, the rpc sever will start, enjoy your test
```
cd example
mkdir build
cd build
cmake ..
make
./example
```
## Authors

[@david xie](www.linkedin.com/in/wei-xie-make-possible)


## Version History
* 0.1
    * Initial Release

## License

This project is licensed under the Apache-2.0 license License - see the LICENSE.md file for details