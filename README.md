# Simple-HTTP-2-Web-Server

This is a simple HTTP2(h2c) web server library for Linux.
This library uses only the c++ standard template library and some C functions.

Included:
HTTP2 Settings sent by clients and servers upon connection.
Sending HTML files.
Some basic header decoding.

Useful methods:
ServerLib(int port), Create a server object with specified port number. 
int startServer(); Binds the socket and starts the server.

Needed in the future:
No support for TLS yet.
No support for UDP communication.
Huffman encoding is incomplete.

Testing:
Test the server with nghttp or similar programs. There is a shellscript (./serverTesting/get.sh) to run a HTTP2 get request to the server.