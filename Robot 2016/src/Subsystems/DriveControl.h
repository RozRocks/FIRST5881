// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#ifndef DRIVE_CONTROL_H
#define DRIVE_CONTROL_H
#include "Commands/Subsystem.h"
#include "WPILib.h"

/**
 *
 *
 * @author ExampleAuthor
 */
class DriveControl: public Subsystem {
private:
	// It's desirable that everything possible is private except
	// for methods that implement subsystem capabilities
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	std::shared_ptr<SpeedController> leftSpeedController1;
	std::shared_ptr<SpeedController> leftSpeedController2;
	std::shared_ptr<SpeedController> rightSpeedController1;
	std::shared_ptr<SpeedController> rightSpeedController2;
	std::shared_ptr<RobotDrive> robotDrive;
	std::shared_ptr<AnalogAccelerometer> analogAccelerometer1WRONG;
	std::shared_ptr<ADXRS450_Gyro> digitalGyro;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
public:
	DriveControl();
	void InitDefaultCommand();
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS
	void TakeJoystickInputs(float x, float y);
};

#endif
