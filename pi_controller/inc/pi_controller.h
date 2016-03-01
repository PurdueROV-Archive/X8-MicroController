#ifndef PI_CONTROLLER
#define PI_CONTROLLER

// INCLUDES:

#include "matrices.h"
#include <math.h>
#include <stdint.h>

// STRUCTS:

typedef struct {

	// Proportional Constant = controller gain
	double P;
	// Integral Constant = controller gain / reset time
	double I;

} PIDConstants;

typedef struct {

	// rotation position to keep ROV at (stabilize ROV at this rotation).
	vect3 rot_ref;
	// rotation position
	vect3 rot_est;
	// /_\ (delta) rotation velocity
	vect3 rot_est_vel;
	// Last Force vector calculated from the PID controller (from the last time getOutput was called).
	vect3 lastForce;
	// Integral sum
	vect3 integralSum;
	uint32_t lastTime;
	uint32_t timeDiff;

	vect3 CObias;

} PIDInputData;

// DEFINES:
#ifndef ON
#define ON 1
#endif
#ifndef OFF
#define OFF 0
#endif

#define P_INIT_VALUE 0
#define I_INIT_VALUE 0
#define INTEGRAL_RECORDING_SIZE 100

// CLASS:

class PIController
{

	public:

		PIController(void);
		void stop(void);
		void start(void);
		void setNewRotation(vect3 rot_ref);
		void setNewP(double newP);
		void setNewI(double newI);
		void sensorInput(vect3 rot_est, vect3 rot_est_vel, uint32_t timems);
		vect3 getOutput(void);

	private:

		int ON_OFF;
		PIDConstants consts;
		PIDInputData data;

};

#endif