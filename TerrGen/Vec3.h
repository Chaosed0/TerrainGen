//Vec3.h - Declaration for the Vec3 class, a representation of a vector in 3-D space
//Programmed by Ed Lu

#pragma once

#include <cmath>

struct Vec3
{
	//Constructors
	Vec3();
	Vec3(float xPos, float yPos, float zPos);

	//Operators
	Vec3 operator-();
	Vec3 operator+(const Vec3& vec);
	Vec3 operator+=(const Vec3& vec);
	Vec3 operator-(const Vec3& vec);
	Vec3 operator-=(const Vec3& vec);
	Vec3 operator/(float scalar);
	Vec3 operator*(float scalar);
	//Takes the dot product of the vector with another vector or a scalar
	float operator*(const Vec3& vec);
	//Takes the cross product
	Vec3 Cross(const Vec3& vec);
	//Returns the unit vector in the direction of this vector
	Vec3 Normalize();
	//Returns the magnitude (length) of this vector
	float Magnitude();
	//If a single component is greater or less than, returns true
	bool operator<(const Vec3& vec);
	bool operator>(const Vec3& vec);
	bool operator<=(const Vec3& vec);
	bool operator>=(const Vec3& vec);

	//Takes the modulus(length) of a vector
	static float Modulus(const Vec3& vec);

	float x;
	float y;
	float z;
};