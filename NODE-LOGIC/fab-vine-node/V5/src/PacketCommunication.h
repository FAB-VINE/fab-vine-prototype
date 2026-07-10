#ifndef PACKET_COMMUNICATION_H
#define PACKET_COMMUNICATION_H

#include <SoftwareSerial.h>
#include <Arduino.h>

#define START_MARKER 0xAA
#define END_MARKER 0x55

template<typename T>
void sendPackage(HardwareSerial& serialPort, T& data)
{
  byte packet[sizeof(T) + 4];
  int packetIndex = 0;

  packet[packetIndex++] = START_MARKER;

  memcpy(&packet[packetIndex], &data, sizeof(T));
  packetIndex += sizeof(T);


  uint16_t checksum = 0;
  for(int i = 0; i<packetIndex; i++){
    checksum += packet[i];
  }
  byte complementChecksum = ~(lowByte(checksum) + highByte(checksum));
  packet[packetIndex++] = complementChecksum;
  packet[packetIndex++] = END_MARKER;


  serialPort.write(packet, packetIndex);
}

template<typename T>
bool processReceivedPackage(byte* packet, int packetLength, T& dataOut)
{
   // validate min packetLength
  if(packetLength < 2) return false;
  
  // validate checksum
  uint16_t checkSum = 0;
  for(int i = 0; i < packetLength-1; i++){
    checkSum += packet[i];
  }
  byte checksumByte =  ~(lowByte(checkSum) + highByte(checkSum));
  if(checksumByte != 0){
    // DBG_PRINTLN("Failed checksum");
    return false;
  }
  // extract data
  int dataIndex = 1;
  memcpy(&dataOut, &packet[dataIndex], sizeof(T));

  return true;
}

template <typename T>
class PacketReceiver{
  private:
    byte buffer[sizeof(T) + 4];
    int index;
    bool receiving;
  
  public:
    PacketReceiver() : index(0), receiving(false){}
    void reset();
    // receivedDataOut
    bool processIncomingData(byte incomingByte,  T& receivedDataOut);
};


template <typename T>
void PacketReceiver<T>::reset(){
  index = 0;
  receiving = false;
}

template <typename T>
bool PacketReceiver<T>::processIncomingData(byte incomingByte,  T& receivedDataOut){
  // byte incomingByte = serialPort.read();
  if(incomingByte == START_MARKER){
    index = 0;
    receiving = true;
  }
  if(receiving){
    buffer[index++] = incomingByte;

    // Check for buffer overflow
    if (index >= (int)sizeof(buffer)) {
        reset();
        return false;
    }
      
    if (incomingByte == END_MARKER) {
        // Process the packet
        bool valid = processReceivedPackage(buffer, index, receivedDataOut);
        reset();
        return valid;
    }
  }
  return false;
}

#endif /* PACKET_COMMUNICATION_H */