#include <Adafruit_PWMServoDriver.h>

class channel
{
public:
      channel(/* args */);
      ~channel();

      // Adafruit_PWMServoDriver PWM_DRIVER;
      // uint8_t PIN_NUM;
      // bool isMotor;
      // uint16_t MIN;
      // uint16_t MAX;
      // uint16_t CENTER;

      Adafruit_PWMServoDriver get_pwm_driver();
      uint8_t get_pin_num();
      bool get_is_motor();
      uint16_t get_min();
      uint16_t get_max();
      uint16_t get_center();

      void set_pwm_driver(Adafruit_PWMServoDriver driver);
      void set_pin_num(uint8_t pin_num);
      void set_is_motor(bool is_motor);
      void set_min(uint16_t val);
      void set_max(uint16_t val);
      void set_center(uint16_t val);
};