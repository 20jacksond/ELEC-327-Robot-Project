//testing the implementation of a PID controller

#include <msp430.h>
#include <stdint.h>

extern int check_error_flag;

//system inputs
extern uint8_t error;
uint8_t previous_error = 0;
extern uint8_t speed;
uint8_t integral = 0;

//PID parameters (set as constants for now, will optimize later)
uint8_t kp = 0; //proportional
uint8_t kd = 0; //derivative
uint8_t ki = 0; //integral

uint8_t pid(error, speed) {
    //implementing the controller
    while (check_error_flag) {
        
        //calculate the integral of the position error
        integral = error + previous_error;

        //PID controller
        next_turn = (kp * error) + (kd * speed) + (ki * integral); //this is not correct I just haven't written it out yet

        previous_error = error;
    }   
}
