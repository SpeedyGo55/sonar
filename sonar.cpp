/**
* Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#include <hardware/gpio.h>
#include <pico.h>
#include <pico/stdlib.h>
#include <pico/time.h>
#include <pico/types.h>
#include <stdio.h>
#include <string>
extern "C" {
    #include "pico_servo.h"
}

#define MAX_PULSE 2500
#define MIN_PULSE 500
#define SERVO_PIN 3
#define TIMEOUT 261000
#define TRIG_PIN 10
#define ECHO_PIN 11


void setupUltrasonic(int trigPin, int echoPin) {
    gpio_init(trigPin);
    gpio_init(echoPin);
    gpio_set_dir(trigPin, GPIO_OUT);
    gpio_set_dir(echoPin, GPIO_IN);
}

void setup() {
    servo_init();
    servo_clock_auto();
    servo_attach(SERVO_PIN);
    servo_set_bounds(300, 1250);
    setupUltrasonic(TRIG_PIN, ECHO_PIN);
    stdio_init_all();
}

int getPulse(int trigPin, int echoPin) {
    gpio_put(trigPin, 1);
    sleep_ms(10);
    gpio_put(trigPin, 0);

    int width = 0;

    while (gpio_get(echoPin) == 0) tight_loop_contents();
    absolute_time_t startTime = get_absolute_time();
    while (gpio_get(echoPin) == 1) {
        width++;
        if (width > TIMEOUT) return -1;
    }
    absolute_time_t endTime = get_absolute_time();

    return absolute_time_diff_us(startTime, endTime);
}

int getCM(int trigPin, int echoPin) {
    int pulseLength = getPulse(trigPin, echoPin);
    if (pulseLength==-1) {
        return -1;
    }
    return pulseLength / 29 / 2;
}

int sum(int numbers[5]) {
    int ret = 0;
    for (int i=0; i<5; i++){
        ret += numbers[i];
    }
    return ret;
}

int average(int numbers[5]) {
    return sum(numbers)/5;
}

int main() {
    setup();
    int angle = 100;
    bool direction = true;
    while (true)
    {
        int distances[5];
        for (int i=0; i<5; i++) {
            distances[i] = getCM(TRIG_PIN, ECHO_PIN);
            if (distances[i]==-1) {
                for (int j=0; i<5; i++){
                    distances[i] =-1;
                }
                break;
            }
            sleep_ms(10);
        }
        servo_move_to(SERVO_PIN, angle);
        int distance = average(distances);
        printf("%d ; %d\n", angle, distance);
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
