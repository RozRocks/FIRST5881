// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "ArmCapture.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

ArmCapture::ArmCapture(): Command() {
        // Use requires() here to declare subsystem dependencies
    // eg. requires(chassis);
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	Requires(Robot::arm.get());
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
}

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTOR

// Called just before this Command runs the first time
void ArmCapture::Initialize() {

}

// Called repeatedly when this Command is scheduled to run
void ArmCapture::Execute() {
	RobotMap::armSpinnerSpeedController.get()->Set(0.3, 0);
}

// Make this return true when this Command no longer needs to run execute()
bool ArmCapture::IsFinished() {
    return false;
}

// Called once after isFinished returns true
void ArmCapture::End() {
	RobotMap::armSpinnerSpeedController.get()->Set(0, 0);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void ArmCapture::Interrupted() {
	End();
}
