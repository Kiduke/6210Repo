#pragma config(Hubs,  S1, HTMotor,  HTServo,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTMotor,  none,     none,     none)
#pragma config(Sensor, S3,     SENSOR_GYRO,    sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     SENSOR_IR,      sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  mtr_S1_C1_1,     motorFR,       tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     motorFL,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     motorBL,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     motorBR,       tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C4_1,     motorLR,       tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C4_2,     motorLL,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C1_1,     motorM,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C1_2,     motorKek,      tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C2_1,    sBasketR,             tServoStandard)
#pragma config(Servo,  srvo_S1_C2_2,    sBasketL,             tServoStandard)
#pragma config(Servo,  srvo_S1_C2_3,    sTubeGrabber,         tServoStandard)
#pragma config(Servo,  srvo_S1_C2_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c";

/* 6210Teleop.c
* Program for driver operated phase of Cascade Effect
* Created by by 6210 Stryke
* 2014-2015 */

//task dedicated to controlling the base and motion of the entire robot in respect to the arena
task base() {
	while(true) {
		getJoystickSettings(joystick);

		int leftControl = joystick.joy1_y1; //Control variables for responsiveness, ease of programming, and changeability
		int rightControl = joystick.joy1_y2;

		//Controls and scales left-side base wheels off of leftControl variable
		if (abs(leftControl) > 10) {
			motor[motorFL] = leftControl / 1.28; //Division by 1.28 is to scale joystick maximum of 128 down to motor maximum of 100
			motor[motorBL] = leftControl / 1.28;
		}
		else {
			motor[motorFL] = 0; //if leftControl is within deadzone, no power is carried to the motors
			motor[motorBL] = 0;
		}

		//Controls and scales right-side base wheels off of rightControl variable
		if (abs(rightControl) > 10) {
			motor[motorBR] = rightControl / 1.28; //Division by 1.28 is to scale joystick maximum of 128 down to motor maximum of 100
			motor[motorFR] = rightControl / 1.28;
		}
		else {
			motor[motorFR] = 0; //if rightControl is within deadzone, no power is carried to the motors
			motor[motorBR] = 0;
		}
		wait1Msec(5); //wait 5 miliseconds for latency
	}
}

//task dedicated to controlling the motion of the treads in front of the robot
task tread() {
	while(true)
	{
		getJoystickSettings(joystick);
		int treadDir = 0;

		int treadButtonIn = joy1Btn(5); //Control variables for responsiveness, ease of programming, and changeability
		int treadButtonOut = joy1Btn(7);

		if (treadButtonIn && treadButtonOut) {
			treadDir = 0;
		}
		else if (treadButtonIn) {
			treadDir = 1;
		}
		else if (treadButtonOut) {
			treadDir = -1;
		}
		else if (!treadButtonIn && !treadButtonOut) {
			treadDir = 0;
		}

		motor[motorM] = treadDir * 50;
		wait1Msec(5);
	}
}



task lift() {
	while(true) {
		getJoystickSettings(joystick);

		int liftUp = joy2Btn(6);
		int liftDown = joy2Btn(8);

		if (liftUp == 1 || liftDown == 1) { //lift control
			if (liftUp == 1 && liftDown == 1) {
				motor[motorLL] = 0;
				motor[motorLR] = 0;
			}
			else if (liftUp == 1) { //lift rises if the liftUp button is pressed
				motor[motorLL] = 85;
				motor[motorLR] = 85;
			}
			else if (liftDown == 1) { //lift lowers if the liftDown button is pressed
				motor[motorLL] = -45;
				motor[motorLR] = -45;
			}
		}
		else
		{
			motor[motorLL] = 0;
			motor[motorLR] = 0;
		}
		wait1Msec(5);
	}
}

task basket()
{
	while(true)
	{
		getJoystickSettings(joystick);

		if (joystick.joy2_TopHat == 4 && !joy2Btn(3))
		{
			servo[sBasketL] = 240;
			servo[sBasketR] = 15;
		}
		else if (joy2Btn(5))
		{
			servo[sBasketL] = 205;
			servo[sBasketR] = 50;
		}
		else if (joy2Btn(7)) // changed from 100 and 155
		{
			servo[sBasketL] = 120;
			servo[sBasketR] = 135;
		}
		wait1Msec(5);
	}
}

task grabber() {
	while(true) {
		getJoystickSettings(joystick);

		if (joy1Btn(3) == 1)
		{
			servo[sTubeGrabber] = 100;
		}

		if (joy1Btn(4) == 1) {
			servo[sTubeGrabber] = 0;
		}
		wait1Msec(5);
	}
}

task main() {
	startTask(base);
	startTask(tread);
	startTask(basket);
	startTask(grabber);
	startTask(lift);
	while(true) {
		wait1Msec(5);
	}
}
