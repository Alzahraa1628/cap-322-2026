#include <SPI.h>
#include <RF24.h>

RF24 radio(14, 27);
const byte address[6] = "00001";

void setup() {
  Serial.begin(115200);
  Serial.println("Transmitter Starting...");

  if (!radio.begin()) {
    Serial.println("Radio hardware NOT responding!");
    while (1);
  }

  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop() {
  const char msg[] = "Hi";
  bool ok = radio.write(&msg, sizeof(msg));

  Serial.print("Send status: ");
  Serial.println(ok ? "OK" : "Failed");

  delay(1000);
}
