// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.




#include "Arm.h"
#include "../RobotMap.h"
// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
#include "SmartDashboard/SmartDashboard.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTANTS

Arm::Arm() : Subsystem("Arm") {
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
    spinnerSpeedController = RobotMap::armSpinnerSpeedController;
    armUpperLimitSwitch = RobotMap::armArmUpperLimitSwitch;
    armLowerLimitSwitch = RobotMap::armArmLowerLimitSwitch;
    armSpeedController = RobotMap::armArmSpeedController;
    ultrasonicBallSensor = RobotMap::armUltrasonicBallSensor;
    ballCaptureLimitSwitch = RobotMap::armBallCaptureLimitSwitch;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS

    SmartDashboard::PutNumber("Spinner Max", 0.5);
    SmartDashboard::PutNumber("Spinner Min", 0.3);
    SmartDashboard::PutNumber("Spinner Start", 0.4);
}

void Arm::InitDefaultCommand() {
    // Set the default command for a subsystem here.
    // SetDefaultCommand(new MySpecialCommand());
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND


    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND
}


// Put methods for controlling this subsystem
// here. Call these from Commands.

