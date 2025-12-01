#include <SPI.h>
#include <RF24.h>

RF24 radio(4, 5);
const byte address[6] = "00001";



String carID = "CAR_123";   // <-- عدل رقم العربية هنا

void setup() {
  Serial.begin(115200);
  Serial.println("Car Transmitter Starting...");

  if (!radio.begin()) {
    Serial.println("Radio hardware NOT responding!");
    while (1);
  }

  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(address);
  radio.stopListening();
}

void loop() {
  char msg[32];
  carID.toCharArray(msg, sizeof(msg));  // تحويل String إلى char[]

  bool ok = radio.write(&msg, sizeof(msg));

  Serial.print("Sent Car ID: ");
  Serial.println(msg);
  Serial.println(ok ? "Status: OK" : "Status: Failed");

  delay(1000);  // إرسال كل ثانية
}
