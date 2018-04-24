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
    settingsFrame.getFrame(frame);
    std::cout << settingsFrame.debugFrame(frame) << std::endl;
    write(socket, frame, settingsFrame.getSize());
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
                case 0:
                    std::cout << "Received DATA frame" << std::endl;
                    break;
                case 1: {
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
                    if(frame.getLength() > 0 && (frame.getFlags() & 0x01) == 1) {
                        connectionError(socket, lastOKIdentifier);
                        terminate = 1;
                        std::cout << "Payload length > 0, but ACK is set. TCP terminated." << std::endl;
                    }


                    if(frame.getFlags() != 0x01) {

                        HTTP2ConnectionSettings settings(frame.getPayload(), frame.getLength()); // TODO how to get settings from frame?

                        //Sending setting frame start
                        HTTP2Frame settingsACK;

                        settingsACK.setType(4);
                        settingsACK.setFlags(0x01);
                        settingsACK.setStreamIdentifier(0);
                        sendFrame(settingsACK);
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
