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

* also you can try use the docker to intall the libraries to your local host.

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
* run miniRPC program in docker. fisrt you just need get the docker/dockerfile, second build your docker image and run, all the source code and environment will be set automatically.

for example (the dockerfile is based on the example library, user can change to own library):
```
cd docker
docker build --tag minirpc:v1.0 .
docker run -it minirpc:v1.0
cd /usr/src/miniRPC
```

* run miniRPC program in kubernetes. please follow [k8s/README.md](https://github.com/wolfplain/miniRPC/blob/main/k8s/README.md)

## Authors

[@david xie](www.linkedin.com/in/wei-xie-make-possible)

## Roadmap
1, Using WebSocket as connect tool;  
2, Using event framework to process connect messages (JSON format);  
3, Support MAC and Linux;  
4, Support k8s, docker deployment;  
5, Support more protocol, TCP, HTTP;  
6, Support stateful and stateless FAAS, provide storage features;  
7, Support protocol buffers, xml message format;  
8, Support WebAssembly;  


## Version History
* 0.1
    * Initial Release

## License

This project is licensed under the Apache-2.0 license License - see the LICENSE.md file for details