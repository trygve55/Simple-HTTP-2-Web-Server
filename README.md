# Simple-HTTP-2-Web-Server

This is a simple HTTP2(h2c) web server library for Linux.


No support for TLS yet.
No support for UDP communication.

Functions:

ServerLib(int port), Create a server object with specified port number. 

int startServer(); Binds the socket and starts the server.


Testing:
You can test the server with nghttp or similar programs. There is a shellscript (./serverTesting/get.sh) to run a HTTP2 get request to the server.