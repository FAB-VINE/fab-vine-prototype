//************************************************************
// this is an example that uses the painlessmesh library to
// upload firmware to another node on the network.
// This will upload to an ESP device running the otaReceiver.ino
//
// The naming convetions should be as follows for bin files
// firmware_<hardware>_<role>.bin
// To create your own binary files, export them and rename
// them to follow this format, where hardware will be
// "ESP8266" or "ESP32" (capitalized.)
// If sending to the otacreceiver sketch, role should be
// "otareceiver" (lowercase)
//
// This sketch assumes a Nodemcu-32s with an SD card
// connected as shown in the jpg included in the sketch folder
// This code may have to be reworked/hardware adjusted for
// other boards or ESP8266. The core code works well though
//
// MAKE SURE YOUR UPLOADED OTA FIRMWARE INCLUDES OTA SUPPORT
// OR YOU WILL LOSE THE ABILITY TO UPLOAD MORE FIRMWARE.
// THE INCLUDED .bin DOES NOT HAVE OTA SUPPORT SO THIS MUST BE
// REFLASHED
//************************************************************

#ifdef ESP32
#else
#define CS_PIN D8
#endif

#include "painlessMesh.h"

#include "LittleFS.h"  // Replace #include "SD.h" and "SPI.h"

#define MESH_PREFIX "FabVineMesh"
#define MESH_PASSWORD "f4bV1n3M3sh"
#define MESH_PORT 5555

#define OTA_PART_SIZE 1024  //How many bytes to send per OTA data packet

painlessMesh mesh;
fs::File firmware_mirror;
// Helper function to restart ESP on error
void rebootEspWithReason(String reason) {
  Serial.println(reason);
  delay(1000);
  ESP.restart();
}

void listDir(fs::FS &fs, const char *dirname, uint8_t levels) {
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("- failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println(" - not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        listDir(fs, file.path(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Initialize Mesh
  mesh.setDebugMsgTypes(ERROR | STARTUP | CONNECTION | DEBUG);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, MESH_PORT);

  // Configure as Root (Sender usually acts as root)
  mesh.setRoot(true);
  mesh.setContainsRoot(true);

  delay(1000);

  // --- Initialize LittleFS ---
  if (!LittleFS.begin(true)) {  // 'true' formats if mount fails
    rebootEspWithReason("Could not mount LittleFS, restarting");
  }

  mesh.initOTASend(
    [&firmware_mirror](painlessmesh::plugin::ota::DataRequest pkg, char *buffer) {
      // Seek to the correct position for this packet
      firmware_mirror.seek(OTA_PART_SIZE * pkg.partNo);

      // Read data into buffer
      int bytesRead = firmware_mirror.read((uint8_t *)buffer, OTA_PART_SIZE);
      return bytesRead;
    },
    OTA_PART_SIZE);

  Serial.println("LittleFS mounted successfully");
  listDir(LittleFS, "/", 3);
  delay(1000);

  // Open root directory to scan for firmware
  File dir = LittleFS.open("/");
  if (!dir || !dir.isDirectory()) {
    rebootEspWithReason("Failed to open LittleFS root directory");
  }

  Serial.println("Scanning for firmware...");

  // // --- Scan for valid firmware files ---
  while (true) {
    File entry = dir.openNextFile();

    // End of directory
    if (!entry) {
      Serial.println("Could not find valid firmware, please upload to LittleFS, will retry in 3s");
      dir.rewindDirectory();
      delay(3000);
      continue;
    }

    if (!entry.isDirectory()) {
      String name = entry.name();
      String extension = name.substring(name.indexOf('.') + 1);
      Serial.println("Analizing: " + name);

      // Validate filename format: firmware_HARDWARE_ROLE.bin
      if (name.length() > 1 && name.indexOf('_') != -1 && name.indexOf('_') != name.lastIndexOf('_') && name.indexOf('.') != -1 && extension == "bin") {

        String firmware = name.substring(0, name.indexOf('_'));
        String hardware = name.substring(name.indexOf('_') + 1, name.lastIndexOf('_'));
        String role = name.substring(name.lastIndexOf('_') + 1, name.indexOf('.'));

        Serial.printf("OTA FIRMWARE FOUND (%s), NOW BROADCASTING\n", name);
        firmware_mirror = LittleFS.open(entry.path());
        // --- Calculate MD5 Hash ---
        // We must reset the file pointer to the beginning for hashing
        entry.seek(0);
        MD5Builder md5;
        md5.begin();
        md5.addStream(entry, entry.size());
        md5.calculate();
        String md5Str = md5.toString();

        // --- Offer OTA to the Network ---
        // Arguments: role, hardware, md5, totalParts, forcedUpdate
        float totalParts = ceil(((float)entry.size()) / OTA_PART_SIZE);
        mesh.offerOTA(role, hardware, md5Str, (size_t)totalParts, false);

        Serial.printf("Broadcasting OTA: Role=%s, Hardware=%s, Parts=%d, MD5=%s\n",
                      role.c_str(), hardware.c_str(), (int)totalParts, md5Str.c_str());

        //         // --- Infinite Loop for OTA Transmission ---
        //         // The sender dedicates itself entirely to serving OTA requestsls
        while (true) {
          mesh.update();
          delay(1);
        }
      }
    }
    entry.close();
  }
}

void loop(){};
