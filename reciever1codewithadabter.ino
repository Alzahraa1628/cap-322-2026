#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

RF24 radio(14, 27); // CE = GPIO14, CSN = GPIO27

const byte address[6] = "00001";

void setup() {
  Serial.begin(115200);
  SPI.begin();
  radio.begin();

  radio.setPALevel(RF24_PA_LOW);

  radio.enableAckPayload();
  radio.enableDynamicPayloads();

  radio.openReadingPipe(0, address);
  radio.startListening();

  Serial.println("Receiver ready");
}

void loop() {
  if (radio.available()) {
    Serial.println("Data available!");
    uint8_t len = radio.getDynamicPayloadSize();
    if (len > 0 && len < 32) {
      char text[32] = {0};
      radio.read(&text, len);
      Serial.print("Received: ");
      Serial.println(text);
    } else {
      Serial.println("Error: Payload size invalid or zero.");
    }
  }
  delay(10);
}
