#include "midi_config.h"

midi_config::midi_config(/* args */)
{
}
// static channel c[72];

// DRIVER & CHANNEL VARIABLE
// Adafruit_PWMServoDriver DRIVER[] = {
//     Adafruit_PWMServoDriver(0x40),
//     Adafruit_PWMServoDriver(0x41),
//     Adafruit_PWMServoDriver(0x42),
//     Adafruit_PWMServoDriver(0x43)};

// const uint8_t midi_numbers[] = {M1, M2, M3, M4, M5, M6, M7, M8, M9, M10, M11,
//                                 M12, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10,
//                                 R11, R12, R13, R14, R15, R16, R17, R18, R19,
//                                 R20, R21, R22, R23, R24, R25, S1, S2, S3, S4,
//                                 S5, S6, S7, S8, S9, S10, S11, S12, S13, S14,
//                                 S15, S16, S17, S18, S19, S20, S21, S22, S23,
//                                 S24, S25};

// void midi_config::init_driver()
// {
//       for (uint8_t i = 0; i < sizeof(DRIVER) / sizeof(DRIVER[0]); i++)
//       {
//             DRIVER[i].begin();
//             DRIVER[i].setOscillatorFrequency(25000000);
//             DRIVER[i].setPWMFreq(50);
//       }
// }

// channel *midi_config::getConfiguration()
// {
//       // DEFAULT VALUE
//       uint8_t driver_num = 0;
//       uint8_t pin_num = 0;

//       for (uint8_t i = 0; i < sizeof(midi_numbers) / sizeof(midi_numbers[0]); i++)
//       {
//             c[midi_numbers[i]].set_is_motor(true);
//             c[midi_numbers[i]].set_pwm_driver(DRIVER[driver_num]);
//             c[midi_numbers[i]].set_pin_num(pin_num);
//             c[midi_numbers[i]].set_max(0);
//             c[midi_numbers[i]].set_min(0);
//             c[midi_numbers[i]].set_center(0);
//             Serial.print(pin_num);
//             Serial.print("  ");
//             Serial.println(driver_num);

//             pin_num++;
//             if (pin_num == 16)
//             {
//                   pin_num = 0;
//                   driver_num++;
//             }
//       }

//       c[midi_numbers[S1]].set_is_motor(false);
//       c[midi_numbers[S1]].set_max(1700);
//       c[midi_numbers[S1]].set_min(1300);
//       c[midi_numbers[S1]].set_center(1500);

//       c[midi_numbers[S2]].set_is_motor(false);
//       c[midi_numbers[S2]].set_max(1700);
//       c[midi_numbers[S2]].set_min(1300);
//       c[midi_numbers[S2]].set_center(1500);

//       c[midi_numbers[S3]].set_is_motor(false);
//       c[midi_numbers[S3]].set_max(1700);
//       c[midi_numbers[S3]].set_min(1300);
//       c[midi_numbers[S3]].set_center(1500);

//       c[midi_numbers[S4]].set_is_motor(false);
//       c[midi_numbers[S4]].set_max(1700);
//       c[midi_numbers[S4]].set_min(1300);
//       c[midi_numbers[S4]].set_center(1500);

//       c[midi_numbers[S5]].set_is_motor(false);
//       c[midi_numbers[S5]].set_max(1700);
//       c[midi_numbers[S5]].set_min(1300);
//       c[midi_numbers[S5]].set_center(1500);

//       c[midi_numbers[S6]].set_is_motor(false);
//       c[midi_numbers[S6]].set_max(1700);
//       c[midi_numbers[S6]].set_min(1300);
//       c[midi_numbers[S6]].set_center(1500);

//       c[midi_numbers[S7]].set_is_motor(false);
//       c[midi_numbers[S7]].set_max(1700);
//       c[midi_numbers[S7]].set_min(1300);
//       c[midi_numbers[S7]].set_center(1500);

//       c[midi_numbers[S8]].set_is_motor(false);
//       c[midi_numbers[S8]].set_max(1700);
//       c[midi_numbers[S8]].set_min(1300);
//       c[midi_numbers[S8]].set_center(1500);

//       c[midi_numbers[S9]].set_is_motor(false);
//       c[midi_numbers[S9]].set_max(1700);
//       c[midi_numbers[S9]].set_min(1300);
//       c[midi_numbers[S9]].set_center(1500);

//       c[midi_numbers[S10]].set_is_motor(false);
//       c[midi_numbers[S10]].set_max(1700);
//       c[midi_numbers[S10]].set_min(1300);
//       c[midi_numbers[S10]].set_center(1500);

//       c[midi_numbers[S11]].set_is_motor(false);
//       c[midi_numbers[S11]].set_max(1700);
//       c[midi_numbers[S11]].set_min(1300);
//       c[midi_numbers[S11]].set_center(1500);

//       c[midi_numbers[S12]].set_is_motor(false);
//       c[midi_numbers[S12]].set_max(1700);
//       c[midi_numbers[S12]].set_min(1300);
//       c[midi_numbers[S12]].set_center(1500);

//       c[midi_numbers[S13]].set_is_motor(false);
//       c[midi_numbers[S13]].set_max(1700);
//       c[midi_numbers[S13]].set_min(1300);
//       c[midi_numbers[S13]].set_center(1500);

//       c[midi_numbers[S14]].set_is_motor(false);
//       c[midi_numbers[S14]].set_max(1700);
//       c[midi_numbers[S14]].set_min(1300);
//       c[midi_numbers[S14]].set_center(1500);

//       c[midi_numbers[S15]].set_is_motor(false);
//       c[midi_numbers[S15]].set_max(1700);
//       c[midi_numbers[S15]].set_min(1300);
//       c[midi_numbers[S15]].set_center(1500);

//       c[midi_numbers[S16]].set_is_motor(false);
//       c[midi_numbers[S16]].set_max(1700);
//       c[midi_numbers[S16]].set_min(1300);
//       c[midi_numbers[S16]].set_center(1500);

//       c[midi_numbers[S17]].set_is_motor(false);
//       c[midi_numbers[S17]].set_max(1700);
//       c[midi_numbers[S17]].set_min(1300);
//       c[midi_numbers[S17]].set_center(1500);

//       c[midi_numbers[S18]].set_is_motor(false);
//       c[midi_numbers[S18]].set_max(1700);
//       c[midi_numbers[S18]].set_min(1300);
//       c[midi_numbers[S18]].set_center(1500);

//       c[midi_numbers[S19]].set_is_motor(false);
//       c[midi_numbers[S19]].set_max(1700);
//       c[midi_numbers[S19]].set_min(1300);
//       c[midi_numbers[S19]].set_center(1500);

//       c[midi_numbers[S20]].set_is_motor(false);
//       c[midi_numbers[S20]].set_max(1700);
//       c[midi_numbers[S20]].set_min(1300);
//       c[midi_numbers[S20]].set_center(1500);

//       c[midi_numbers[S21]].set_is_motor(false);
//       c[midi_numbers[S21]].set_max(1700);
//       c[midi_numbers[S21]].set_min(1300);
//       c[midi_numbers[S21]].set_center(1500);

//       c[midi_numbers[S22]].set_is_motor(false);
//       c[midi_numbers[S22]].set_max(1700);
//       c[midi_numbers[S22]].set_min(1300);
//       c[midi_numbers[S22]].set_center(1500);

//       c[midi_numbers[S23]].set_is_motor(false);
//       c[midi_numbers[S23]].set_max(1700);
//       c[midi_numbers[S23]].set_min(1300);
//       c[midi_numbers[S23]].set_center(1500);

//       c[midi_numbers[S24]].set_is_motor(false);
//       c[midi_numbers[S24]].set_max(1700);
//       c[midi_numbers[S24]].set_min(1300);
//       c[midi_numbers[S24]].set_center(1500);

//       c[midi_numbers[S25]].set_is_motor(false);
//       c[midi_numbers[S25]].set_max(1700);
//       c[midi_numbers[S25]].set_min(1300);
//       c[midi_numbers[S25]].set_center(1500);

//       return c;
// }

midi_config::~midi_config()
{
}
