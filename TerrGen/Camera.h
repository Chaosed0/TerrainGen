//Camera.h - Declaration of a camera object in 3D space.
//Programmed by Ed Lu

#pragma once

#include "Vec3.h"

#define MOVE_SPEED 1
#define DEG_TO_RAD 3.14/180

class Camera
{
private:
	float angleHoriz;		//Angle with regard to the horizontal plane
	float angleVert;		//Angle with regard to the vertical plane
	Vec3 pos;				//Camera position
	int moveModifier;		//Multiplier for the move speed

public:
	Camera();									//Default constructor
	void Move(bool forward);					//Move forward or backward
	void Strafe(bool right);					//Move sideways
	void updateRot(float relX, float relY);		//Update rotation with relative mouse coords
	void setPos(Vec3 newPos);					//Sets the position of the camera to the specified coordinate
	void incMoveModifier();						//Adds 1 to moveModifier
	void decMoveModifier();						//Subtracts 1 from moveModifier

	float getAngleHoriz();		//Returns angleHoriz
	float getAngleVert();		//Returns angleVert
	Vec3 getPos();				//Returns pos

	//EXTREMELY USEFUL
	//This function returns the "pointing vector," the vector at which the camera would be pointing if it were at (0, 0, 0),
	//  rotated by some angle in the horizontal/vertical directions (offsetH and offsetY, respectively).
	Vec3 getPointVec(float offsetH, float offsetV);

	Vec3 getVertRot();			//Returns the vector around which the camera should be rotated vertically
};