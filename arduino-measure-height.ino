#include "SSD1306_Writer.hpp"
#include "Toggle.hpp"
#include "Ultrasonic.hpp"

/*
nano:
- SDA: A4
- SCL: A5
*/

constexpr int DEBOUNCE_DELAY = 20; // ms
constexpr int TOGGLE_PIN = 4;
constexpr int TRIG_PIN = 7;
constexpr int ECHO_PIN = 8;

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
  static Toggle unitToggle(TOGGLE_PIN, DEBOUNCE_DELAY);
  static UltrasonicSensor ultrasonic(TRIG_PIN, ECHO_PIN, DistUnit::CENTIMETERS);

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