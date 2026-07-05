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
bool processRecievedPackage(byte* packet, int packetLenght, T& dataOut)
{
   // validate min packetLenght
  if(packetLenght < 2) return false;

  // validate checksum
  uint16_t checkSum = 0;
  for(int i = 0; i < packetLenght-1; i++){
    checkSum += packet[i];
  }
  byte checksumByte =  ~(lowByte(checkSum) + highByte(checkSum));
  if(checksumByte != 0){
    Serial.println("Failed checksum");
    return false;
  }
  // extract data
  int dataIndex = 1;
  memcpy(&dataOut, &packet[dataIndex], sizeof(T));

  return true;
}

template <typename T>
class PacketReciever{
  private:
    byte buffer[sizeof(T) + 4];
    int index;
    bool receiving;

  public:
    PacketReciever() : index(0), receiving(false){}
    void reset();
    bool processIncomingData(byte incomingByte,  T& recievedDataOut);
};


template <typename T>
void PacketReciever<T>::reset(){
  index = 0;
  receiving = false;
}

template <typename T>
bool PacketReciever<T>::processIncomingData(byte incomingByte,  T& recievedDataOut){
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
        bool valid = processRecievedPackage(buffer, index, recievedDataOut);
        reset();
        return valid;
    }
  }
  return false;
}

#endif /* PACKET_COMMUNICATION_H */
