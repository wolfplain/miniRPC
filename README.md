# Project Title

miniRPC

## Description

miniRPC is a remote protocol call service using Websocket as the network protocol.


## Getting Started

### Dependencies

* Websocket doesn't depend on any open source, it's a good learning template for slef develop Websocket server.
* Using Json format as the message format, the server uses rapidjson to process Json message.
* Using GoogleTest as the unit test framework.

### Installing

* go into the miniRPC directory, and run ./build.sh. it will automatically create a build directory.
  all of the headers and libraries will be installed into this directory. 
* Any modifications needed to be made to files/folders

### Executing program

* go into the miniRPC directory, and run ./build.sh. it will automatically create a build directory.
  all of the headers and libraries will be installed into this directory.

```
cd ./miniRPC
./build.sh
```

* run program
```
./build/tests/tests/ut-miniRPC --gtest_filter=*
```

## Authors

[@david xie](www.linkedin.com/in/wei-xie-make-possible)


## Version History
* 0.1
    * Initial Release

## License

This project is licensed under the Apache-2.0 license License - see the LICENSE.md file for details
