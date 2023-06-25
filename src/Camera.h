#ifndef HEADER_C75B0A0BEC9BEE82
#define HEADER_C75B0A0BEC9BEE82

//
// Quake map viewer
//
// Author: Johan Gardhage <johan.gardhage@gmail.com>
//
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "vector3.h"

class Camera
{
private:
	float yaw;			// Direction of travel
	float pitch;		// Neck angle
	float speed;		// Speed along heading
	float strafe;		// Speed along heading

public:
	vector3 head;		// Position of head
	vector3 view;		// Normal along viewing direction

	Camera();
	void UpdatePosition(void);
	void Pitch(float degrees);
	void Yaw(float degrees);
	void PitchUp(void);
	void PitchDown(void);
	void MoveForward(void);
	void MoveBackward(void);
	void TurnRight(void);
	void TurnLeft(void);
	void StrafeRight(void);
	void StrafeLeft(void);
};

#endif
#endif // header guard

