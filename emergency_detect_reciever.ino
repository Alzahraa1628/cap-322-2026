#include <SPI.h>
#include <RF24.h>

#define CE_PIN 4
#define CSN_PIN 5

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "00001";

uint8_t packet[32];
int lastSeq = -1;

uint8_t calcChecksum(uint8_t *buf, int len) {
  uint8_t x=0;
  for (int i=0;i<len;i++) x ^= buf[i];
  return x;
}

void setup() {
  Serial.begin(115200);
  delay(200);
  Serial.println("RX: starting...");
  if (!radio.begin()) Serial.println("RX: radio.begin() FAILED");
  else Serial.println("RX: radio.begin() OK");

  if (!radio.isChipConnected()) Serial.println("RX: nRF NOT connected!");
  else Serial.println("RX: nRF connected");

  radio.setPALevel(RF24_PA_HIGH);
  radio.setDataRate(RF24_1MBPS);
  radio.setRetries(5, 15);
  radio.openReadingPipe(0, address);
  radio.startListening();
  radio.setChannel(76);
}

void loop() {
  if (radio.available()) {
    radio.read(packet, sizeof(packet));

    // quick corrupted content detection before strlen: checksum
    uint8_t chk = calcChecksum(packet, 31);
    if (chk != packet[31]) {
      Serial.println("Corrupted packet (bad checksum) -> ignored");
      return;
    }

    uint8_t seq = packet[0];
    if (seq == lastSeq) {
      Serial.print("Duplicate seq=");
      Serial.println(seq);
      return;
    }
    lastSeq = seq;

    // payload as C-string starting index 1
    char payload[31];
    memcpy(payload, &packet[1], 30);
    payload[30] = '\0';

    Serial.print("Received seq=");
    Serial.print(seq);
    Serial.print(" payload=");
    Serial.println(payload);

    String msg = String(payload);
    if (msg.indexOf("EMERGENCY") != -1) {
      Serial.println("⚠ Emergency Vehicle Detected!");
    } else if (msg.indexOf("NORMAL") != -1) {
      Serial.println("Regular Vehicle.");
    } else {
      Serial.println("Unknown Data.");
    }
  }
}
