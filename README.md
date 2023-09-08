# miniRPC
Mini RPC using Websocket as connect tool. This idea comes from my work experience of exploring the WASM technology.
1, websocket as the network protocol to communicate the native server. (next step convert to WASM)
2, using multiplexing IO to process network message (JSON format)
3, provide minimum rpc server and client library for user to use this framework

blueprint:
1, not only for websocket, plan for TCP, HTTP
2, support Linux and Mac
3, support stateful and stateless FAAS
4, support multiple message format : JSON, protocol, XML
