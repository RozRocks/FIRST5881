// RobotBuilder Version: 2.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


#include "OI.h"

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES
#include "SmartDashboard/SmartDashboard.h"
#include "Commands/ArmCapture.h"
#include "Commands/ArmEject.h"
#include "Commands/ArmLock.h"
#include "Commands/ArmOpen.h"
#include "Commands/AssistedDrive.h"
#include "Commands/AssistedTurn.h"
#include "Commands/AutonomousCommand.h"
#include "Commands/Drive.h"
#include "Commands/ReverseOrientation.h"


    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=INCLUDES

OI::OI() {
    // Process operator interface input here.
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
    joystick.reset(new Joystick(0));
    
    joystickButton4.reset(new JoystickButton(joystick.get(), 4));
    joystickButton4->WhenPressed(new ArmLock());
    joystickButton3.reset(new JoystickButton(joystick.get(), 3));
    joystickButton3->WhenPressed(new ArmOpen());
    joystickButton2.reset(new JoystickButton(joystick.get(), 2));
    joystickButton2->WhenPressed(new ArmCapture());
    joystickButton1.reset(new JoystickButton(joystick.get(), 1));
    joystickButton1->WhenPressed(new ArmEject());

    // SmartDashboard Buttons
    SmartDashboard::PutData("Reverse Orientation", new ReverseOrientation());
    SmartDashboard::PutData("ArmEject", new ArmEject());
    SmartDashboard::PutData("ArmLock", new ArmLock());
    SmartDashboard::PutData("ArmCapture", new ArmCapture());
    SmartDashboard::PutData("ArmOpen", new ArmOpen());
    SmartDashboard::PutData("Drive: Flatter", new Drive(2));
    SmartDashboard::PutData("Drive: Medium Slow", new Drive(5));
    SmartDashboard::PutData("Drive: Slow RampUp", new Drive(10));

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
}

// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=FUNCTIONS

std::shared_ptr<Joystick> OI::getJoystick() {
   return joystick;
}


    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=FUNCTIONS
