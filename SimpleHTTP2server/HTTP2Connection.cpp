#include "HTTP2Connection.hpp"
#include "HTTP2Frame.hpp"
#include "HTTP2Stream.hpp"

unsigned int HTTP2Connection::connectionIdIncrement = 0;

HTTP2Connection::HTTP2Connection(int socket, char *buffer, WebBinder *webBinder) {
    connectionId = connectionIdIncrement++;
    
    this->socket = socket;
    this->reciveBuffer = buffer;
    this->sendBuffer = new char[BUFFERSIZE];
    this->concurrentStreams = 1;
    this->webBinder = webBinder;

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
    unsigned int payloadSize = 36;
    char payload[payloadSize];
    payloadSize = settings.buildPayload(payload);
    settingsFrame.setPayload(payload, payloadSize);
    sendFrame(settingsFrame);
    //Sending setting frame end

    //reads client upgrade header (MAGIC) start
    read(socket, buffer, 24);
    //reads client upgrade header (MAGIC) end

    //reciveFrames start
    int bytesRead;
    unsigned int lastOKIdentifier = -1;
    bool terminate = 0;
    
    //declaring reused variables
    unsigned int streamDependency, padLength, weight;
    bool streamDependencyExclusive;
    
    while(!terminate) {
        bytesRead = read(socket, buffer, BUFFERSIZE);

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
                case HTTP2Frame::FrameIDs.HEADERS: 
                    std::cout << "Received HEADERS frame" << std::endl;
                    proccessHeaderFrame(frame);
                    break;
                case HTTP2Frame::FrameIDs.PRIORITY: {
                    std::cout << "Received PRIORITY frame" << std::endl;
                    
                    char *receivedPayload = frame.getPayload();
                    
                    streamDependencyExclusive = (receivedPayload[0] & 0x80) >> 7;
                    streamDependency = 0;
                    streamDependency += (receivedPayload[0] << 24) & 0xE;
                    streamDependency += receivedPayload[1] << 16;
                    streamDependency += receivedPayload[2] << 8;
                    streamDependency += receivedPayload[3];
                    weight = receivedPayload[4];
                    
                    if (frame.getStreamIdentifier() == 0) {
                        std::cout << "PRIORITY frame with StreamIdentifier == 0 received. PROTOCOL_ERROR" << std::endl;
                        connectionError(0, ErrorCodes.PROTOCOL_ERROR);
                    } if (frame.getLength() > 40) {
                        streamError(frame.getStreamIdentifier(), ErrorCodes.FRAME_SIZE_ERROR);
                    } else {
                        setStreamWeight(frame.getStreamIdentifier(), weight);
                    }
                    
                    break;
                }
                case HTTP2Frame::FrameIDs.RST_STREAM:
                    std::cout << "Received RST_STREAM frame" << std::endl;
                    
                    //std::cout << ""
                    if (frame.getStreamIdentifier() == 0) {
                        connectionError(0, ErrorCodes.PROTOCOL_ERROR);
                    } else if (getStream(frame.getStreamIdentifier()).getStage() == HTTP2Stream::States.idle) {
                        connectionError(0, ErrorCodes.PROTOCOL_ERROR);
                    } else if (frame.getLength() > 32) {
                        connectionError(0, ErrorCodes.FRAME_SIZE_ERROR);
                    } else {
                        setStreamState(frame.getStreamIdentifier(), HTTP2Stream::States.closed);
                    }
                    
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

void HTTP2Connection::proccessHeaderFrame(HTTP2Frame frame) {
    //streams[]
    char *receivedPayload = frame.getPayload();
    unsigned int iterator = 0;
    
    int streamDependency = -1, padLength = -1, weight = -1;
    bool streamDependencyExclusive = false;
    
    //Padded flag
    if (frame.getFlag(3)) {
        std::cout << "Padded flag" << std::endl;
        padLength = receivedPayload[iterator++];
    }
    
    //Priority flag
    if (frame.getFlag(5)) {
        std::cout << "Priority flag" << std::endl;
        
        streamDependency = 0;
        streamDependencyExclusive = (receivedPayload[iterator] & 0x80) >> 7;
        streamDependency += (receivedPayload[iterator] << 24) & 0xE;
        streamDependency += receivedPayload[iterator + 1] << 16;
        streamDependency += receivedPayload[iterator + 2] << 8;
        streamDependency += receivedPayload[iterator + 3];
        weight = receivedPayload[iterator + 4];
        iterator += 5;
        
        if (frame.getStreamIdentifier() == 0 ) 
            return connectionError(0, ErrorCodes.PROTOCOL_ERROR);
        
        setStreamWeight(streamDependency, weight);    
    }
    
    //End headers flag
    if (frame.getFlag(2)) std::cout << "End header flag" << std::endl;
    
    //End stream flag
    if (frame.getFlag(0)) {
        std::cout << "End stream flag" << std::endl;
        setStreamState(streamDependency, 6);
    }
    
    //Decode header block fragment
    //pointer = receivedPayload[iterator]
    //length = frame.length() - iterator
    
    Header header = hpack.decodeHTTP2Header(&receivedPayload[iterator], frame.getLength() - iterator);
    
    std::cout << std::endl << std::endl << "Decoded HTTP2 frame:" << std::endl << header.getHTTP2_0();
    
    //Sending data frame start
    HTTP2Frame dataFrame;
    
    dataFrame.setType(0);
    dataFrame.setFlags(0x01);
    dataFrame.setStreamIdentifier(streamDependency);
    std::string payload("Test");
    dataFrame.setPayload(payload.c_str(), 4);
    sendFrame(dataFrame);
    //Sending data frame end
}

void HTTP2Connection::streamError(int streamIdentifier, unsigned int errorCode) {
    
    //prepare payload
    char payload[4];
    
    payload[0] = (errorCode >> 24) & 0xFF;
    payload[1] = (errorCode >> 16) & 0xFF;
    payload[2] = (errorCode >> 8) & 0xFF;
    payload[3] = (errorCode) & 0xFF;

    //send frame
    HTTP2Frame RSTFrame;
    RSTFrame.setType(HTTP2Frame::FrameIDs.RST_STREAM);
    RSTFrame.setStreamIdentifier(streamIdentifier);
    RSTFrame.setPayload(payload, 4);
    sendFrame(RSTFrame);
}

void HTTP2Connection::connectionError(unsigned int lastOKID, unsigned int errorCode) {
    
    connectionError(lastOKID, errorCode, new char[0], 0);
}

void HTTP2Connection::connectionError(unsigned int lastOKID, unsigned int errorCode, char *additionalDebugData, unsigned int additionalDebugDataLength) {
    
    //prepare payload
    char payload[8 + additionalDebugDataLength];
    
    payload[0] = (lastOKID >> 24) & 0xEF;
    payload[1] = (lastOKID >> 16) & 0xFF;
    payload[2] = (lastOKID >> 8) & 0xFF;
    payload[3] = (lastOKID) & 0xFF;
    payload[4] = (errorCode >> 24) & 0xFF;
    payload[5] = (errorCode >> 16) & 0xFF;
    payload[6] = (errorCode >> 8) & 0xFF;
    payload[7] = (errorCode) & 0xFF;
    
    for (unsigned int  i = 0; i < additionalDebugDataLength;i++) payload[8 + i] = additionalDebugData[i];
    
    //send frame
    HTTP2Frame goAwayFrame;
    goAwayFrame.setType(7);
    goAwayFrame.setStreamIdentifier(lastOKID);
    goAwayFrame.setPayload(payload, 8 + additionalDebugDataLength);
    sendFrame(goAwayFrame);
    
    close(socket);
}

ssize_t HTTP2Connection::sendFrame(HTTP2Frame frame) {
    frame.getFrame(sendBuffer);
    std::cout << "Sending frame: " << frame.debugFrame(sendBuffer) << " Size: " << std::dec << frame.getSize() << std::endl;
    return write(socket, sendBuffer, frame.getSize());
}

void HTTP2Connection::setStreamState(int streamIdentifier, unsigned int state) {
    if (streams[streamIdentifier].getStreamIdentifier() == -1) streams[streamIdentifier].setStreamIdentifier(streamIdentifier);
    streams[streamIdentifier].setState(state);
}

void HTTP2Connection::setStreamWeight(int streamIdentifier, char state) {
    if (streams[streamIdentifier].getStreamIdentifier() == -1) streams[streamIdentifier].setStreamIdentifier(streamIdentifier);
    streams[streamIdentifier].setState(state);
}

HTTP2Stream HTTP2Connection::getStream(int streamIdentifier) {
    if (streams[streamIdentifier].getStreamIdentifier() == -1) streams[streamIdentifier].setStreamIdentifier(streamIdentifier);
    return streams[streamIdentifier];
}