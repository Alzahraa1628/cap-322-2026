#include <SPI.h>
#include <RF24.h>

RF24 radio(14, 27);
const byte address[6] = "00001";

void setup() {
  Serial.begin(115200);
  Serial.println("Receiver Starting...");

  if (!radio.begin()) {
    Serial.println("Radio hardware NOT responding!");
    while (1);
  }

  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(0, address);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char txt[32];
    radio.read(&txt, sizeof(txt));
    Serial.print("Received: ");
    Serial.println(txt);
  }
}
