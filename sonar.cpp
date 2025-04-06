/**
* Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#include <pico/stdlib.h>
#include <stdio.h>
#include <string>
extern "C" {
    #include "pico_servo.h"
}

#define MAX_PULSE 2500
#define MIN_PULSE 500
#define SERVO_PIN 3

void setup() {
    servo_init();
    servo_clock_auto();
    servo_attach(SERVO_PIN);
    servo_set_bounds(300, 1250);
    stdio_init_all();
}

int main() {
    setup();
    int angle = 100;
    bool direction = true;
    while (true)
    {
        servo_move_to(SERVO_PIN, angle);
        printf("%d\n", angle);
        sleep_ms(10);
        if (angle >= 180 || angle <= 0) {
            direction = !direction;
        }
        if (direction) {
            angle++;
        } else {
            angle--;
        }
    }

}
