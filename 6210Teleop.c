#pragma config(Hubs,  S1, HTMotor,  HTServo,  HTMotor,  HTMotor)
#pragma config(Hubs,  S2, HTMotor,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     SENSOR_GYRO,    sensorI2CHiTechnicGyro)
#pragma config(Sensor, S4,     SENSOR_IR,      sensorHiTechnicIRSeeker1200)
#pragma config(Motor,  mtr_S1_C1_1,     motorFR,       tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     motorFL,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_1,     motorBL,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     motorBR,       tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C4_1,     motorLR,       tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C4_2,     motorLL,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C1_1,     motorMR,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C1_2,     motorML,       tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C2_1,    sBasketR,             tServoStandard)
#pragma config(Servo,  srvo_S1_C2_2,    sBasketL,             tServoStandard)
#pragma config(Servo,  srvo_S1_C2_3,    sTubeGrabberL,        tServoStandard)
#pragma config(Servo,  srvo_S1_C2_4,    sTubeGrabberR,        tServoStandard)
#pragma config(Servo,  srvo_S1_C2_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c";
#include "6210_autonomous_functions.h";

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

		int treadButtonIn = joy2Btn(2); //Control variables for responsiveness, ease of programming, and changeability
		int treadButtonOut = joy2Btn(1);

		//if both buttons are pressed, treads stop
		if (treadButtonIn && treadButtonOut) {
			treadControl(2);
		}
		//if the in button is pressed, pull balls in
		else if (treadButtonIn) {
			treadControl(1);
		}
		//if the out button is pressed, push balls out
		else if (treadButtonOut) {
			treadControl(3);
		}
		//if neither button is pressed, stop treads
		else if (!treadButtonIn && !treadButtonOut) {
			treadControl(2);
		}
		wait1Msec(5); //wait for latency
	}
}

//task for controlling the lift to raise or lower the basket with balls
task lift() {
	while(true) {
		getJoystickSettings(joystick);

		int liftUp = joy2Btn(6);
		int liftDown = joy2Btn(8);

		if (liftUp == 1 || liftDown == 1) { //lift control
			if (liftUp == 1 && liftDown == 1) {
				liftControl(2);
			}
			else if (liftUp == 1) { //up button pressed, lift raises
				liftControl(1);
			}
			else if (liftDown == 1) { //down button pressed, lift lowers
				liftControl(3);
			}
		}
		else
		{
			liftControl(2); //no button pressed, lift is stopped
		}
		wait1Msec(5);
	}
}

task basket()
{
	while(true)
	{
		getJoystickSettings(joystick);

		int lowPreset = joystick.joy2_TopHat;
		int midPreset = joy2Btn(5);
		int highPreset = joy2Btn(7);

		if (lowPreset == 4) //low button is pressed (D-Pad Down), basket is flat
		{
			setBasket(1);
		}
		else if (midPreset == 1) //mid button pressed, basket tilted slightly
		{
			setBasket(2);
		}
		else if (highPreset == 1) //high button pressed, basket dumps balls
		{
			setBasket(3);
		}
		wait1Msec(5);
	}
}

task grabber() {
	while(true) {
		getJoystickSettings(joystick);

		int open = joy1Btn(3);
		int closed = joy1Btn(4);

		if (open == 1) //open button pressed, tube grabber opens
		{
			openGrabber();
		}

		if (closed == 1) //close button pressed, tube grabber closes
		{
			closeGrabber();
		}
		wait1Msec(5);
	}
}

//starts all tasks for driver control
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
