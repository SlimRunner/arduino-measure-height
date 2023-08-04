#pragma once
enum class DistUnit { CENTIMETERS, INCHES, MILLIMITERS, METERS };

class UltrasonicSensor {
private:
  int m_trigPin;
  int m_echoPin;
  float m_cache;
  float m_convfact;
  DistUnit m_unit;

  static float getConvFactor(DistUnit unit) {
    float factor = 343 / 2;
    switch (unit) {
    case DistUnit::CENTIMETERS:
      // 10^-6 * 10^2
      factor *= 0.0001;
      break;

    case DistUnit::INCHES:
      // 10^-6 * 10^2 / 2.54
      factor *= 3.937007874015748e-5f;
      break;

    case DistUnit::MILLIMITERS:
      // 10^-6 * 10^3
      factor *= 0.001;
      break;

    case DistUnit::METERS:
      // 10^-6 * 10^6
      factor *= 1;
      break;

    default:
      break;
    }
    return factor;
  }

public:
  UltrasonicSensor() = delete;
  ~UltrasonicSensor() {}

  UltrasonicSensor(int trigPin, int echoPin) : m_trigPin(trigPin), m_echoPin(echoPin) {
    setConversion(DistUnit::CENTIMETERS);
    pinMode(m_trigPin, OUTPUT);
    pinMode(m_echoPin, INPUT);
  }

  UltrasonicSensor(int trigPin, int echoPin, DistUnit unit) : m_trigPin(trigPin), m_echoPin(echoPin) {
    setConversion(unit);
    pinMode(m_trigPin, OUTPUT);
    pinMode(m_echoPin, INPUT);
  }

  float readDistance() {
    // set pin to reset sensor in low
    digitalWrite(m_trigPin, LOW);
    delayMicroseconds(2);

    // set sensor to send pulses
    digitalWrite(m_trigPin, HIGH);
    delayMicroseconds(10);
    // stop sending pulses and read delay (in us)
    digitalWrite(m_trigPin, LOW);

    m_cache = pulseIn(m_echoPin, HIGH);
    return m_cache * m_convfact;
  }

  float readCache(DistUnit unit) {
    return m_cache * getConvFactor(unit);
  }

  void setConversion(DistUnit unit) {
    m_unit = unit;
    m_convfact = UltrasonicSensor::getConvFactor(m_unit);
  }

  char * getUnitText() {
    switch (m_unit) {
    case DistUnit::CENTIMETERS:
      return "cm";

    case DistUnit::INCHES:
      return "in";

    case DistUnit::MILLIMITERS:
      return "mm";

    case DistUnit::METERS:
      return "m";

    default:
      return "";
    }
  }
};
