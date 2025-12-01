#include <SPI.h>
#include <RF24.h>

RF24 radio(4, 5);
const byte address[6] = "00001";

unsigned long carCount = 0;
String carList[200];   // تخزين 200 سيارة كحد أقصى
int indexCars = 0;

bool isNewCar(String id) {
  for (int i = 0; i < indexCars; i++) {
    if (carList[i] == id) return false;
  }
  return true;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Main Receiver Starting...");

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
    String receivedID = String(txt);

    // تنظيف ID من أي garbage 
    receivedID.trim();

    if (isNewCar(receivedID)) {
      carList[indexCars++] = receivedID;
      carCount++;

      Serial.print("New Car Detected: ");
      Serial.println(receivedID);

      Serial.print("Total Cars: ");
      Serial.println(carCount);
    } else {
      Serial.print("Duplicate Car Ignored: ");
      Serial.println(receivedID);
    }
  }
}
