#include "ServerLib.hpp"
#include "ReadFile.cpp"
#include "HeaderParser.cpp"
#include "HTTP2Frame.cpp"
#include "Settings.hpp"

ServerLib::ServerLib(int port) : port(port) {
  // Temporary
  thread_action = [this](int socket, std::string *res) {
      char buffer[1024] = {0};
      std::cout << "Client Connected" << std::endl;
        
      read(socket, buffer, 1024);
      
      Header header = ParseHeader(buffer);
      
      std::cout << header.getHeaderline("upgrade") << std::endl;
      
      //if (forceHTTP2 || header.getHeaderline("upgrade").compare("h2c")) {
      if (header.getHeaderline("upgrade").compare("h2c")) {
          //Upgrade connection to HTTP2(h2c)
          handleHTTP2Request(socket, buffer);
      } else {
          //Respond as HTTP 1.1
          string html = read_htmlfile("www/test.html");
          write(socket, html.c_str(), html.length());
          std::cout << "Response sent" << std::endl;
          
          close(socket);
      }
      return 0;
  };
}

int ServerLib::startServer() {
  
  // Creating socket file descriptor
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
      perror("socket failed");
      return -1;
  }
    
  // Forcefully attaching socket to the port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
  {
      perror("setsockopt");
      return -2;
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);
    
  // Forcefully attaching socket to the port 8080
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
      perror("bind failed");
      return -3;
  }
  if (listen(server_fd, 3) < 0) {
      perror("listen");
      return -41;
  }
  
  std::cout << "Server online" << std::endl;
  while (1) {
      handleRequest();
  }
  
  return 0;
}

int ServerLib::handleRequest() {
    int new_socket;
    
    if (thread_action) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            return -1;
        }

        std::thread (thread_action, new_socket, &defaultResponse).detach();// Execute the function upon connection.
      }
    return 0;
}

int ServerLib::handleHTTP2Request(int socket, char buffer[1024]) {
    
    string upgradeHeader = "HTTP/1.1 101 Switching Protocols \r\n";
    upgradeHeader.append("Connection: Upgrade\r\n");
    upgradeHeader.append("Upgrade: h2c \r\n\r\n");
    
    write(socket, upgradeHeader.c_str(), upgradeHeader.length());
    
    std::cout << "Upgraded connection to HTTP2(h2c)" << std::endl;
    
    //Sending setting frame start
    HTTP2Frame settingsFrame;
    
    settingsFrame.setType(4);
    char payload[0];
    settingsFrame.setPayload(payload, 0);
    char frame[1024]= {0};
    settingsFrame.getFrame(frame);
    std::cout << settingsFrame.debugFrame(frame)<< std::endl;
    write(socket, frame, settingsFrame.getSize());
    //Sending setting frame end
    
    //reads client upgrade header
    read(socket, buffer, 24);
    
    int bytesRead;
    unsigned int lastOKIdentifier = -1;
    bool terminate = 0;
    while (!terminate) {
        bytesRead = read(socket, buffer, 1024);
        
        if (bytesRead > 0) {
            std::cout << "Bytes received: " << bytesRead << " ";
            for(int i=0; i< bytesRead; ++i)
                std::cout << std::hex << (int)buffer[i];
            std::cout << std::endl;
            
            vector<HTTP2Frame> receivedFrames = bufferToFrames(buffer, bytesRead);
            
            std::cout << "Frames received: " << receivedFrames.size() << std::endl;
            
            //Handle received frames
            for (auto frame: receivedFrames) {
                auto streamID = frame.getStreamIdentifier();
                if (frame.getLength() > 0 && (frame.getFlags() & 0x01) == 1) {
                    connectionError(socket, lastOKIdentifier);
                    terminate = 1;
                    std::cout << "Payload length > 0, but ACK is set. TCP terminated." << std::endl;
                }
                switch (frame.getType()) {
                // Use inheritance for frame types?
                case 0:
                    std::cout << "Received DATA frame" << std::endl;
                    break;
                case 1:
                    std::cout << "Received HEADERS frame" << std::endl;
                    break;
                case 2:
                    std::cout << "Received PRIORITY frame" << std::endl;
                    break;
                case 3:
                    std::cout << "Received RST_STREAM frame" << std::endl;
                    break;
                case 4:
                    /*
                     * SETTINGS_HEADER_TABLE_SIZE (0x1)
                     * SETTINGS_ENABLE_PUSH (0x2)
                     * SETTINGS_MAX_CONCURRENT_STREAMS (0x3)
                     * SETTINGS_INITIAL_WINDOW_SIZE (0x4)
                     * SETTINGS_MAX_FRAME_SIZE (0x5)
                     * SETTINGS_MAX_HEADER_LIST_SIZE (0x6)
                     */
                    std::cout << "Received SETTINGS frame" << std::endl;
                    if (frame.getFlags() != 0x01) {
                        
                        Settings settings(frame.getPayload());// TODO how to get settings from frame?
                        
                        //Sending setting frame start
                        HTTP2Frame settingsACK;
                        
                        settingsACK.setType(4);
                        settingsACK.setFlags(0x01);
                        settingsACK.setStreamIdentifier(0);
                        settingsACK.sendFrame(socket);
                        //Sending setting frame end
                    }
                    break;
                case 5:
                    std::cout << "Received PUSH_PROMISE frame" << std::endl;
                    break;
                case 6:
                    std::cout << "Received PING frame" << std::endl;
                    break;
                case 7:
                    std::cout << "GOAWAY frame received. No new streams allowed." << std::endl;
                    terminate = 1;
                    break;
                case 8:
                    std::cout << "Received WINDOW_UPDATE frame" << std::endl;
                    break;
                case 9:
                    std::cout << "Received CONTINUATION frame" << std::endl;
                    break;
                }
                lastOKIdentifier = streamID;
            }
        }
            //std::cout << buffer << std::endl;
        
        std::this_thread::sleep_for (std::chrono::seconds(1));
    }
    
    //close(socket);
    
    return 0;
}

int ServerLib::setDefaultResponse(std::string response) {
    defaultResponse = response;
    return 0;
}

void ServerLib::setDebug(bool debug) {
    debugFlag = debug;
}

/*void ServerLib::setForceHTTP2(bool forceHTTP2) {
    this -> forceHTTP2 = forceHTTP2;
}

int ServerLib::findHeaderEnd(char buffer[1024]) {
    return 0;
}*/

void ServerLib::connectionError(int socket, unsigned int lastOKID) {
    HTTP2Frame goAway;
    goAway.setType(7);
    goAway.setStreamIdentifier(lastOKID);
    goAway.sendFrame(socket);
    close(socket);
}
