#pragma once

enum class ToggleWhen { low, high };

/**
 * @brief Keeps track of the press state of a press switch, so that it can
 * simulate a toggle switch. It also starts pin in INPUT_PULLUP automatically.
 *
 */
class Toggle {
private:
  const long m_debounceDelay;
  const int m_pin;
  const ToggleWhen m_switchWhen;

  // m_lastDebounce is a member variable because that way I can initialize a
  // base time when begin is called.

  long m_lastDebounce;
  int m_switchState;
  bool m_toggleState;

  void begin();
  bool canToggle(int thisState) const;

public:
  /**
   * @brief Default constructor is disallowed
   *
   */
  Toggle() = delete;

  /**
   * @brief Construct a new Toggle object with a given initial state
   *
   * @param pin pin id where switch is connected
   */
  Toggle(uint8_t pin);

  /**
   * @brief Construct a new Toggle object with a given initial state
   *
   * @param pin pin id where switch is connected
   * @param switchWhen state on which a toggle will occur (default is HIGH)
   */
  Toggle(uint8_t pin, ToggleWhen switchWhen);

  /**
   * @brief Construct a new Toggle object with a given initial state
   *
   * @param pin pin id where switch is connected
   * @param dTime amount of time under which reject changes (debounce)
   */
  Toggle(uint8_t pin, long dTime);

  /**
   * @brief Construct a new Toggle object with a given initial state
   *
   * @param pin pin id where switch is connected
   * @param switchWhen state on which a toggle will occur (default is HIGH)
   * @param dTime amount of time under which reject changes (debounce)
   */
  Toggle(uint8_t pin, ToggleWhen switchWhen, long dTime);

  // default destructor

  /**
   * @brief Determine if signal has changed since last call
   *
   * @return true if switch state has changed
   * @return false otherwise
   */
  bool isToggled();

  /**
   * @brief Determine the state of the last update
   *
   * @return true if last update recorded HIGH,
   * @return false otherwise
   */
  bool isHigh() const;

  /**
   * @brief Determine the state of the last update
   *
   * @return true if last update recorded LOW,
   * @return false otherwise
   */
  bool isLow() const;

  /**
   * @brief Determine the state of the simulated toggle
   *
   * @return true if the toggle would be ON,
   * @return false otherwise
   */
  bool isOn() const;

  /**
   * @brief Determine the state of the simulated toggle
   *
   * @return true if the toggle would be OFF,
   * @return false otherwise
   */
  bool isOff() const;
};

void Toggle::begin() {
  pinMode(m_pin, INPUT_PULLUP);
  m_switchState = digitalRead(m_pin);
}

bool Toggle::canToggle(int thisState) const {
  switch (m_switchWhen) {
  case ToggleWhen::high:
    return thisState == HIGH;

  case ToggleWhen::low:
    return thisState == LOW;

  default:
    // this should never happen
    return false;
  }
}

Toggle::Toggle(uint8_t pin)
    : m_pin(pin), m_toggleState(false), m_lastDebounce(millis()),
      m_debounceDelay(0), m_switchWhen(ToggleWhen::high) {
  begin();
}

Toggle::Toggle(uint8_t pin, ToggleWhen switchWhen)
    : m_pin(pin), m_toggleState(false), m_lastDebounce(millis()),
      m_debounceDelay(0), m_switchWhen(switchWhen) {
  begin();
}

Toggle::Toggle(uint8_t pin, long dTime)
    : m_pin(pin), m_toggleState(false), m_lastDebounce(millis()),
      m_debounceDelay(dTime), m_switchWhen(ToggleWhen::high) {
  begin();
}

Toggle::Toggle(uint8_t pin, ToggleWhen switchWhen, long dTime)
    : m_pin(pin), m_toggleState(false), m_lastDebounce(millis()),
      m_debounceDelay(dTime), m_switchWhen(switchWhen) {
  begin();
}

bool Toggle::isToggled() {
  // reference for debounce code
  // https://docs.arduino.cc/built-in-examples/digital/Debounce

  static int lastState;
  int newState = digitalRead(m_pin);
  long now = millis();
  if (lastState != newState) {
    m_lastDebounce = now;
  }
  lastState = newState;
  if (now - m_lastDebounce > m_debounceDelay) {
    bool changed = m_switchState != newState;
    m_switchState = newState;
    if (changed && canToggle(newState)) {
      m_toggleState = !m_toggleState;
      return true;
    }
    return false;
  }
}

bool Toggle::isHigh() const { return m_switchState == HIGH; }

bool Toggle::isLow() const { return m_switchState == LOW; }

bool Toggle::isOn() const { return m_toggleState; }

bool Toggle::isOff() const { return !m_toggleState; }
