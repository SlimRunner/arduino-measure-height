#include "SSD1306_Writer.hpp"
#include "Toggle.hpp"
#include "Ultrasonic.hpp"

oled_ssd1306::SSD1306_Writer screen(2);

void setup() {
  Serial.begin(115200);
  screen.begin();
}

void loop() {
  constexpr int SENSOR_POLL_DELAY = 200;
  static long mainTimeReset = 0;
  long currentTime = millis();

  static int unitIndex = 0;
  static Toggle unitToggle(4, 20);
  static UltrasonicSensor ultrasonic(7, 8, DistUnit::CENTIMETERS);

  if (unitToggle.isToggled()) {
    unitIndex = (unitIndex + 1) % 3;
    switch (unitIndex) {
    case 0:
      ultrasonic.setConversion(DistUnit::CENTIMETERS);
      break;
    case 1:
      ultrasonic.setConversion(DistUnit::INCHES);
      break;
    case 2:
      ultrasonic.setConversion(DistUnit::MILLIMITERS);
      break;
    default:
      break;
    }
  }

  if (currentTime - mainTimeReset > SENSOR_POLL_DELAY) {
    mainTimeReset = currentTime;
    screen.write(ultrasonic.readDistance(), ultrasonic.getUnitText());
  }
}