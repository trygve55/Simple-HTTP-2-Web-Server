#include "HTTP2Connection.hpp"
#include "HTTP2Frame.hpp"
#include "HTTP2Stream.hpp"

unsigned int HTTP2Connection::connectionIdIncrement = 0;

HTTP2Connection::HTTP2Connection(int socket, char *buffer) {
    connectionId = connectionIdIncrement++;
    
    this->socket = socket;
    this->reciveBuffer = buffer;
    this->sendBuffer = new char[1024];
    this->concurrentStreams = 1;

    //Switching to HTTP2(h2c) start
    std::string upgradeHeader = "HTTP/1.1 101 Switching Protocols \r\n";
    upgradeHeader.append("Connection: Upgrade\r\n");
    upgradeHeader.append("Upgrade: h2c \r\n\r\n");
    write(socket, upgradeHeader.c_str(), upgradeHeader.length());
    std::cout << "Upgraded connection to HTTP2(h2c)" << std::endl;
    //Switching to HTTP2(h2c) end

    //Sending setting frame start
    HTTP2Frame settingsFrame;

    settingsFrame.setType(4);
    char payload[0];
    settingsFrame.setPayload(payload, 0);
    char frame[1024] = {0};
    sendFrame(frame);
    //Sending setting frame end

    //reads client upgrade header (MAGIC) start
    read(socket, buffer, 24);
    //reads client upgrade header (MAGIC) end

    //reciveFrames start
    int bytesRead;
    unsigned int lastOKIdentifier = -1;
    bool terminate = 0;
    while(!terminate) {
        bytesRead = read(socket, buffer, 1024);

        if(bytesRead > 0) {
            std::cout << "Bytes received: " << bytesRead << " ";
            for(int i = 0; i < bytesRead; ++i)
                std::cout << std::hex << (int)buffer[i];
            std::cout << std::endl;

            std::vector<HTTP2Frame> receivedFrames = HTTP2Frame::bufferToFrames(buffer, bytesRead);

            std::cout << "Frames received: " << receivedFrames.size() << std::endl;

            //Handle received frames
            for(auto frame : receivedFrames) {
                auto streamID = frame.getStreamIdentifier();

                std::cout << "Frame with stream id " << streamID << " proccessed." << std::endl;

                switch(frame.getType()) {
                // Use inheritance for frame types?
                case HTTP2Frame::FrameIDs.DATA:
                    std::cout << "Received DATA frame" << std::endl;
                    break;
                case HTTP2Frame::FrameIDs.HEADERS: {
                    std::cout << "Received HEADERS frame" << std::endl;

                    //Sending data frame start
                    HTTP2Frame dataFrame;

                    dataFrame.setType(0);
                    dataFrame.setFlags(0x01);
                    dataFrame.setStreamIdentifier(11);
                    std::string pay("Test");
                    dataFrame.setPayload(pay.c_str(), 4);
                    sendFrame(dataFrame);
                    //Sending data frame end

                    break;
                }
                case HTTP2Frame::FrameIDs.PRIORITY:
                    std::cout << "Received PRIORITY frame" << std::endl;
                    break;
                case HTTP2Frame::FrameIDs.RST_STREAM:
                    std::cout << "Received RST_STREAM frame" << std::endl;
                    break;
                case HTTP2Frame::FrameIDs.SETTINGS:
                    std::cout << "Received SETTINGS frame" << std::endl;
                    if(frame.getLength() > 0 && (frame.getFlags() & 0x01) == 1) {
                        connectionError(socket, lastOKIdentifier);
                        terminate = 1;
                        std::cout << "Payload length > 0, but ACK is set. TCP terminated." << std::endl;
                    }

                    if(frame.getFlags() != 0x01) {

                        HTTP2ConnectionSettings settings(frame.getPayload(), frame.getLength());

                        //Sending setting frame start
                        HTTP2Frame settingsACK;

                        settingsACK.setType(4);
                        settingsACK.setFlags(0x01);
                        settingsACK.setStreamIdentifier(0);
                        sendFrame(settingsACK);
                        //Sending setting frame end
                    }
                    break;
                case HTTP2Frame::FrameIDs.PUSH_PROMISE:
                    std::cout << "Received PUSH_PROMISE frame" << std::endl;
                    break;
                case HTTP2Frame::FrameIDs.PING:
                    std::cout << "Received PING frame" << std::endl;
                    break;
                case HTTP2Frame::FrameIDs.GOAWAY:
                    std::cout << "GOAWAY frame received. No new streams allowed." << std::endl;
                    terminate = 1;
                    break;
                case HTTP2Frame::FrameIDs.WINDOW_UPDATE:
                    std::cout << "Received WINDOW_UPDATE frame" << std::endl;
                    break;
                case HTTP2Frame::FrameIDs.CONTINUATION:
                    std::cout << "Received CONTINUATION frame" << std::endl;
                    break;
                }
                lastOKIdentifier = streamID;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    //close(socket);
}

void HTTP2Connection::connectionError(int socket, unsigned int lastOKID) {
    HTTP2Frame goAway;
    goAway.setType(7);
    goAway.setStreamIdentifier(lastOKID);
    sendFrame(goAway);
    close(socket);
}

ssize_t HTTP2Connection::sendFrame(HTTP2Frame frame) {
    frame.getFrame(sendBuffer);
    std::cout << "Sending frame: " << frame.debugFrame(sendBuffer)  << " Size: " << std::dec << frame.getSize() << std::endl;
    return write(socket, sendBuffer, frame.getSize());
}
