#include "channel.h"

Adafruit_PWMServoDriver PWM_DRIVER;
uint8_t PIN_NUM;
bool isMotor;
uint16_t MIN;
uint16_t MAX;
uint16_t CENTER;

channel::channel(/* args */)
{
}

Adafruit_PWMServoDriver channel::get_pwm_driver()
{
      return PWM_DRIVER;
}
uint8_t channel::get_pin_num()
{
      return PIN_NUM;
}
bool channel::get_is_motor()
{
      return isMotor;
}
uint16_t channel::get_min()
{
      return MIN;
}
uint16_t channel::get_max()
{
      return MAX;
}
uint16_t channel::get_center()
{
      return CENTER;
}

void channel::set_pwm_driver(Adafruit_PWMServoDriver driver)
{
      PWM_DRIVER = driver;
}

void channel::set_pin_num(uint8_t pin_num)
{
      PIN_NUM = pin_num;
}

void channel::set_is_motor(bool is_motor)
{
      isMotor = is_motor;
}

void channel::set_min(uint16_t val)
{
      MIN = val;
}

void channel::set_max(uint16_t val)
{
      MAX = val;
}

void channel::set_center(uint16_t val)
{
      CENTER = val;
}

channel::~channel(/* args */)
{
}