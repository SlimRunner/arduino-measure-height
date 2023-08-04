#pragma once

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

namespace oled_ssd1306 {
enum class WriterUnit { centimeters, inches };

class SSD1306_Writer {
private:
  static constexpr int SCREEN_WIDTH = 128; // OLED display width,  in pixels
  static constexpr int SCREEN_HEIGHT = 64; // OLED display height, in pixels
  static constexpr int OLED_ADDR = 0x3C;

  static constexpr int CHAR_WIDTH = 5;
  static constexpr int CHAR_HEIGHT = 7;

  Adafruit_SSD1306 m_screen;
  int m_scaling;
  bool validState;

public:
  SSD1306_Writer();
  SSD1306_Writer(int scaling);

  bool begin();

  void write(int num, String unit);
};

SSD1306_Writer::SSD1306_Writer()
    : m_screen{SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1}, m_scaling(1) {}

SSD1306_Writer::SSD1306_Writer(int scaling)
    : m_screen{SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1}, m_scaling(scaling) {}

bool SSD1306_Writer::begin() {
  if (!m_screen.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    validState = false;
    return validState;
  }
  m_screen.setTextSize(m_scaling);
  m_screen.setTextColor(WHITE);
  m_screen.display();
  validState = true;
  return validState;
}

void SSD1306_Writer::write(int num, String unit) {
  if (!validState) {
    return;
  }
  using s13 = SSD1306_Writer;
  const int xhalf =
      (s13::SCREEN_WIDTH - (log10(num) + unit.length()) * s13::CHAR_WIDTH * m_scaling) / 2;
  const int yhalf = (s13::SCREEN_HEIGHT - s13::CHAR_HEIGHT * m_scaling) / 2;

  m_screen.clearDisplay();
  m_screen.setCursor(xhalf, yhalf);
  m_screen.print(num);
  m_screen.print(unit);
  m_screen.display();
}

} // namespace oled_ssd1306
