#include "pi_controller.h"

// CLASS METHODS:

// Constructor:
PIController::PIController(void) 
{
	data.rot_ref = vect3Make(0,0,0);
	data.rot_est = vect3Make(0,0,0);
	data.rot_est_vel = vect3Make(0,0,0);
	data.lastForce = vect3Make(0,0,0);
	data.CObias = vect3Make(0,0,0);
	data.integralSum = vect3Make(0,0,0);
	data.lastTime = 0;
	data.timeDiff = 0;

	consts.P = P_INIT_VALUE;
	consts.I = I_INIT_VALUE;
	ON_OFF = OFF;
}


// Call this every time that the PI controller should stop affecting the force vector.
void PIController::stop(void)
{
	ON_OFF = OFF;
}

// Call every time the PI Controller will be started or restarted.
// This method ensures that all variables are set to what they should be to be restarted.
void PIController::start(void)
{
	data.rot_ref = data.rot_est;
	data.rot_est = vect3Make(0,0,0);
	data.rot_est_vel = vect3Make(0,0,0);
	data.CObias = data.lastForce;
	data.integralSum.x = 0;
	data.integralSum.y = 0;
	data.integralSum.z = 0;
	data.lastTime = 0;
	ON_OFF = ON;
}

// Sets new target rotation orientation.
// Perform this method even when PI is off.
void PIController::setNewRotation(vect3 rot_ref)
{
	data.rot_ref = rot_ref;
}
void PIController::setNewP(double newP)
{
	consts.P = newP;
}
void PIController::setNewI(double newI)
{
	consts.I = newI;
}

// Sets the fusion data from the sensors to the data structure.
// Perform this method even when PI is off.
void PIController::sensorInput(vect3 rot_est, vect3 rot_est_vel, uint32_t timems)
{
	data.timeDiff = timems - data.lastTime;
	data.lastTime = timems;
	// if lastTime is 0, then this is the first update since the PI controller has been turned on.
	if (data.lastTime != 0)
	{
		data.integralSum.x += data.rot_est.x * data.timeDiff;
		data.integralSum.y += data.rot_est.y * data.timeDiff;
		data.integralSum.z += data.rot_est.z * data.timeDiff;
	}
	data.rot_est = rot_est;
	data.rot_est_vel = rot_est_vel;
}

// Computes the resultant force vector to achieve the desired rotational orientation using the PI Controller.
// 
// @returns - the force vector computed from the PI Controller, or just the incoming force vector set by
//			Control.
// @note - May want to shift these computations to the sensorInput function... If data needs to be 
//			accumulated even when the controller is off.
vect3 PIController::getOutput(void)
{
	if (ON_OFF == OFF)
		return data.rot_ref;

	data.lastForce.x = data.CObias.x + consts.P * (data.rot_ref.x - data.rot_est.x) + consts.I * data.integralSum.x;
	data.lastForce.y = data.CObias.y + consts.P * (data.rot_ref.y - data.rot_est.y) + consts.I * data.integralSum.y;
	data.lastForce.z = data.CObias.z + consts.P * (data.rot_ref.z - data.rot_est.z) + consts.I * data.integralSum.z;

	return data.lastForce;
}