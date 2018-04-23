# Simple-HTTP-2-Web-Server

This is a simple HTTP2(h2c) web server. It does not support TLS yet.

Functions:

ServerLib(int port), Create a server object with port number "port". 

int startServer(); Binds the socket and starts the server.


Testing:
You can test the server with nghttp or similar programs. There is a shellscript (./serverTesting/get.sh) to run a HTTP2 get request to the server.