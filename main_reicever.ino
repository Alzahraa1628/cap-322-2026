#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <RF24.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

RF24 radio(14, 27);  // CE, CSN
const uint64_t addressPipe = 0x1234567890LL;

int carCount = 0;
bool emergency = false;

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("System Start");

  if (!radio.begin()) {
    Serial.println("NRF ERROR!");
    lcd.setCursor(0, 1);
    lcd.print("NRF ERR!");
    while (1);
  }

  radio.setPALevel(RF24_PA_LOW);
  radio.setDataRate(RF24_1MBPS);
  radio.openReadingPipe(0, addressPipe);
  radio.startListening();

  Serial.println("Receiver Ready");
}

void loop() {
  if (radio.available()) {
    int msg = 0;
    radio.read(&msg, sizeof(msg));

    if (msg == 1) {
      carCount++;
      Serial.print("Car Count: ");
      Serial.println(carCount);

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Cars: ");
      lcd.print(carCount);

    } else if (msg == 999) {
      emergency = true;
      Serial.println("EMERGENCY DETECTED!");

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("!! EMERGENCY !!");
    }
  }
}
