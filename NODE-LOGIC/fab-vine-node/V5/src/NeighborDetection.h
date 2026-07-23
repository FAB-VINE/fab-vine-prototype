#ifndef NEIGHBOR_DETECTION_H
#define NEIGHBOR_DETECTION_H

#include <SoftwareSerial.h>
#include "Faces.h"
#include "NodeState.h"
#include "PacketCommunication.h"

typedef std::function<void(Face)> FaceEventFunction;
typedef std::function<void(void)> PositionEventFunction;
typedef const unsigned long& TimeInMillis;

struct PacketData {
  uint8_t state;
  uint8_t hasPos; 
  int8_t pos[3];
};

class NeighborDetection
{
  private:
  SoftwareSerial SerialLeft;
  SoftwareSerial SerialRight;
  SoftwareSerial SerialTop;
  SoftwareSerial SerialBottom;
  SoftwareSerial SerialFront;
  SoftwareSerial SerialPosterior;
  
  // keep the same order as Face enum to correctly address each face across the code
  SoftwareSerial* rxPorts[FACE_COUNT] = { &SerialLeft, &SerialRight, &SerialTop, &SerialBottom, &SerialFront, &SerialPosterior };
  
  unsigned long neighborsTimeOut = 3000;
  unsigned long lastSeen[FACE_COUNT];
  bool hasNeighbor[FACE_COUNT];

  PacketData broadcastData;
  PacketData neighborData[FACE_COUNT];
  PacketReceiver<PacketData> faceReceivers[FACE_COUNT];

  // callbacks 
  FaceEventFunction onFaceDisconnect;
  FaceEventFunction onFaceConnect;
  PositionEventFunction onPositionFound;
  PositionEventFunction onPositionLost;

  NodeState& node;

public:
  const PacketData* neighborsData;
  const bool* hasNeighborConnected;

public:
  NeighborDetection(
    int8_t leftRxPin,
    int8_t rightRxPin,
    int8_t topRxPin,
    int8_t bottomRxPin,
    int8_t frontRxPin,
    int8_t posteriorRxPin,
    NodeState& node
  )
  : SerialLeft{leftRxPin},
    SerialRight{rightRxPin},
    SerialTop{topRxPin},
    SerialBottom{bottomRxPin},
    SerialFront{frontRxPin},
    SerialPosterior{posteriorRxPin},
    neighborsData{neighborData},
    hasNeighborConnected{hasNeighbor},
    node{node}
  {}
  void begin(uint32_t baud);
  void update(TimeInMillis currentTime);
  void sendBroadcastData(PacketData state);
  void setOnFaceConnectedCallback(FaceEventFunction callback){onFaceConnect = callback;}
  void setOnFaceDisconnectedCallback(FaceEventFunction callback){onFaceDisconnect = callback;}
  void setOnPositionFoundCallback(PositionEventFunction callback){onPositionFound = callback;}
  void setOnPositionLostCallback(PositionEventFunction callback){onPositionLost = callback;}
  uint8_t countConnectedFaces();

private: 
  void readIncomingData(TimeInMillis currentTime);
  void processFaceDetection(TimeInMillis currentTime);
};

#endif // NEIGHBOR_DETECTION_H