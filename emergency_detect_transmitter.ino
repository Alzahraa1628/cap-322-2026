#include <SPI.h>
#include <RF24.h>

#define CE_PIN 4
#define CSN_PIN 5

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

uint8_t packet[32];
uint8_t seq = 0;
const char vehicleType[] = "EMERGENCY"; // <= fits in payload

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("TX: starting...");
  if (!radio.begin()) Serial.println("TX: radio.begin() FAILED");
  else Serial.println("TX: radio.begin() OK");

  if (!radio.isChipConnected()) Serial.println("TX: nRF NOT connected!");
  else Serial.println("TX: nRF connected");

  // reliability settings
  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_1MBPS);
  radio.setRetries(5, 15); // (delay, count)
  radio.openWritingPipe(address);
  radio.stopListening();
  radio.setChannel(76); // change if interference suspected
}

uint8_t calcChecksum(uint8_t *buf, int len) {
  uint8_t x=0;
  for (int i=0;i<len;i++) x ^= buf[i];
  return x;
}

void buildPacket() {
  memset(packet, 0, sizeof(packet));
  packet[0] = seq;
  // copy vehicleType starting at index 1, leave space for checksum at [31]
  strncpy((char*)&packet[1], vehicleType, 30-1); // at most 30 bytes payload (1 seq + 30 + 1 checksum =32)
  packet[31] = calcChecksum(packet, 31); // checksum over bytes 0..30
}

void loop() {
  buildPacket();
  bool ok = radio.write(packet, sizeof(packet));
  if (ok) {
    Serial.print("Message Sent seq=");
    Serial.println(seq);
  } else {
    Serial.println("Failed to send!");
  }
  seq++; // wrap at 0..255
  delay(200); // small delay
}
