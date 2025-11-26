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

  radio.enableAckPayload();      // تفعيل خاصية التأكيد مع payload
  radio.enableDynamicPayloads(); // تفعيل بايلود ديناميكي

  radio.openWritingPipe(address);
  radio.stopListening();

  Serial.println("Transmitter ready");
}

void loop() {
  const char text[] = "Hello ESP32 Left Side";
  bool ok = radio.write(text, strlen(text) + 1); // +1 لل null terminator
  if (ok) {
    Serial.println("Sent message successfully.");
  } else {
    Serial.println("Failed to send message.");
  }
  delay(1000);
}
