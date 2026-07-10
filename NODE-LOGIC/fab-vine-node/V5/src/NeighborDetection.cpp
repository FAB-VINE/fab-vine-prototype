#include "NeighborDetection.h"

void NeighborDetection::begin(const uint32_t baud)
{

    // reset state arrays
    for (int face = 0; face < FACE_COUNT; face++) {
        lastSeen[face] = 0;
        hasNeighbor[face] = false;
    }

    Serial1.begin(baud, SERIAL_8N1, -1, D6);

    for (int i = 0; i < FACE_COUNT; ++i) {
        rxPorts[i]->begin(baud);
    }
}

void NeighborDetection::update(TimeInMillis currentTime)
{
    readIncomingData(currentTime);
    processFaceDetection(currentTime);
}

void NeighborDetection::sendBroadcastData(const uint8_t state)
{
    broadcastData.state = state;
    sendPackage(Serial1, broadcastData);
}

void NeighborDetection::setOnFaceConnectedCallback(FaceEventFunction callback)
{
    onFaceConnect = callback;
}

void NeighborDetection::setOnFaceDisconnectedCallback(FaceEventFunction callback)
{
    onFaceDisconnect = callback;
}

uint8_t NeighborDetection::countConnectedFaces()
{
    uint8_t connectedFaces = 0;
    for (uint8_t i = 0; i < FACE_COUNT; ++i) {
        if (hasNeighbor[i]) {
            connectedFaces++;
        }
    }
    return connectedFaces;
}

void NeighborDetection::readIncomingData(TimeInMillis currentTime)
{
    // RECEIVE
    for (uint8_t i = 0; i < FACE_COUNT; ++i) {
        // loop all faces
        Face face = (Face)i;
        // process serial buffer
        while (rxPorts[face]->available() > 0) {
            if (faceReceivers[face].processIncomingData(rxPorts[face]->read(), neighborData[face])) {
                lastSeen[face] = currentTime;
                if(hasNeighbor[face] == false && onFaceConnect){
                    onFaceConnect(face);
                }
                hasNeighbor[face] = true;
            }
        }
    }
}

void NeighborDetection::processFaceDetection(TimeInMillis currentTime)
{
    for (int i = 0; i < FACE_COUNT; ++i) {
        Face face = (Face)i;
        if (!hasNeighbor[face]) continue;

        if (currentTime - lastSeen[face] > neighborsTimeOut) {
            // invoke callback
            if(onFaceDisconnect) onFaceDisconnect(face);
            hasNeighbor[face] = false;
            lastSeen[face] = 0;
        }
    }

    // calculate grid pos
    if (hasNeighbor[FaceLeft]) {
        uint8_t leftNeighbors = neighborData[FaceLeft].neighborCount[FaceLeft];
        broadcastData.neighborCount[FaceLeft] = leftNeighbors > 0 ? leftNeighbors + 1 : 1;
    } else {
        broadcastData.neighborCount[FaceLeft] = 0;
    }
}
