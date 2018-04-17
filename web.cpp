#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string>
#define PORT 8080
int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    char buffer[1024] = {0};
    int addrlen = sizeof(address);
    
    std::string http = "HTTP/1.1 200 OK\r\n" ;
    http.append("Content-Type: text/html\r\n");
    http.append("\r\n");
    http.append("<HTML><BODY>\r\n");
    http.append("<H1> Hilsen. Du har koblet deg opp til min enkle web-tjener </h1>\r\n");
    http.append("</BODY></HTML>\r\n");
    
    std::cout << http << std::endl;
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, 
                                 sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    std::cout << "server started" << std::endl;
    
    while (1) {
        
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                           (socklen_t*)&addrlen))<0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        
        std::cout << "connected" << std::endl;
            
        read(new_socket, buffer, 1024);
        std::cout << std::string(buffer) << std::endl;

        write(new_socket, http.c_str(), http.size());
        std::cout << "sent"<< std::endl;
        
        close(new_socket);
    }
    
    return 0;
}