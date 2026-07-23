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

void NeighborDetection::sendBroadcastData(const PacketData state)
{
    broadcastData = state;
    sendPackage(Serial1, broadcastData);
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
    uint8_t connectedFaces = 0;
    for (int i = 0; i < FACE_COUNT; ++i) {
        Face face = (Face)i;
        if (!hasNeighbor[face]) continue;

        if (currentTime - lastSeen[face] > neighborsTimeOut) {
            // invoke callback
            if(onFaceDisconnect) onFaceDisconnect(face);
            hasNeighbor[face] = false;
            lastSeen[face] = 0;
            continue;
        }
        ++connectedFaces;

        if(!node.positionKnown && neighborData[face].hasPos){
            
            Face oppositeFace = OPP_Face[face];

            node.pos[0] = neighborsData[face].pos[0] + face_dir_vector[oppositeFace][0];
            node.pos[1] = neighborsData[face].pos[1] + face_dir_vector[oppositeFace][1];
            node.pos[2] = neighborsData[face].pos[2] + face_dir_vector[oppositeFace][2];

            node.positionKnown = true;

            if(onPositionFound) onPositionFound();
        }
    }
    if(connectedFaces == 0 && node.positionKnown && !node.isSeed){
        node.positionKnown = false;
        if(onPositionLost) onPositionLost();
    }
}
