//Camera.cpp - Implementation of a camera in 3D space
//Programmed by Ed Lu

/*
		 Y
        |
		|
		|
		|________ X
	   /
	  /
	 / Z

	 YES!??!
*/

#include "Camera.h"

Camera::Camera()
{
	pos = Vec3(0, 100, 0);
	angleHoriz = 0;
	angleVert = 0;
	moveModifier = 1;
}

void Camera::Move(bool forward)
{
	//If we're moving forward, add; if not, subtract
	if(forward)
		pos += getPointVec(0, 0) * MOVE_SPEED * moveModifier;
	else
		pos -= getPointVec(0, 0) * MOVE_SPEED * moveModifier;
}

void Camera::Strafe(bool right)
{
	//Remove the Y coord from rotating the pointVec 90 degrees - strafing doesn't move in the Y dir at all
	//However, this makes it move dirt slow when looking up - we must keep the same length
	Vec3 strafeVec = getPointVec(90, 0);
	float length = strafeVec.Magnitude();
	strafeVec.y = 0;
	strafeVec = strafeVec * length/strafeVec.Magnitude();

	//If we're moving right, add; if not, subtract
	if(right)
		pos += strafeVec * MOVE_SPEED * moveModifier;
	else
		pos -= strafeVec * MOVE_SPEED * moveModifier;
}

void Camera::updateRot(float relX, float relY)
{
	angleHoriz += relX;
	angleVert -= relY;

	//Vertical angle should be capped at 90 and -90 (so that the player can't flip to being upsidedown)
	if(angleVert > 89)
		angleVert = 89;
	else if(angleVert < -89)
		angleVert = -89;

	//Make the horizontal angle less than 360 (to make sure they don't get too large)
	while(angleHoriz >= 360)
		angleHoriz -= 360;
}

float Camera::getAngleHoriz()
{
	return angleHoriz;
}

float Camera::getAngleVert()
{
	return angleVert;
}

Vec3 Camera::getPos()
{
	return pos;
}

Vec3 Camera::getPointVec(float offsetH, float offsetV)
{
	float hRad = (angleHoriz + offsetH) * DEG_TO_RAD;
	float vRad = (angleVert + offsetV) * DEG_TO_RAD;

	return Vec3(cos(hRad) * cos(vRad), sin(vRad), sin(hRad) * cos(vRad));
}

void Camera::setPos(Vec3 newPos)
{
	pos = newPos;
}

void Camera::incMoveModifier()
{
	moveModifier++;
}

void Camera::decMoveModifier()
{
	moveModifier--;
}