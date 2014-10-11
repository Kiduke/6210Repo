#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     SENSOR_GYRO,    sensorI2CHiTechnicGyro)
#pragma config(Motor,  mtr_S1_C1_1,     motorBL,       tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     motorBR,       tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     motorFL,       tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     motorFR,       tmotorTetrix, openLoop, reversed, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c";

task teleop()
{
	while(true)
	{
		getJoystickSettings(joystick);
		if (abs(joystick.joy1_y1) > 10)
		{
			motor[motorFL] = joystick.joy1_y1 / 1.28;
			motor[motorBL] = joystick.joy1_y1 / 1.28;
		}
		else
		{
			motor[motorFL] = 0;
			motor[motorBL] = 0;
		}
		if (abs(joystick.joy1_y2) > 10)
		{
			motor[motorBR] = joystick.joy1_y2 * 1.28;
			motor[motorFR] = joystick.joy1_y2 * 1.28;
		}
		else
		{
			motor[motorFR] = 0;
			motor[motorBR] = 0;
		}
		wait1Msec(5);
	}
}

task main()
{
	waitForStart();
	startTask(teleop);
	while(true)
	{
		wait1Msec(5);
	}
}
