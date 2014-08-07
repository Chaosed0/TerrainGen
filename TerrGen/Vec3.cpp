//Vec3.cpp - Implementation of the Vec3.h class
//Programmed by Ed Lu

#include "Vec3.h"

Vec3::Vec3()
{
	x = 0;
	y = 0;
	z = 0;
}

Vec3::Vec3(float xPos, float yPos, float zPos)
{
	x = xPos;
	y = yPos;
	z = zPos;
}

Vec3 Vec3::operator-()
{
	return Vec3(-x, -y, -z);
}

Vec3 Vec3::operator+(const Vec3& vec)
{
	//Add the components of the vectors being added
	return Vec3(x + vec.x, y + vec.y, z + vec.z);
}

Vec3 Vec3::operator+=(const Vec3& vec)
{
	//Add the components of the vectors being added
	x += vec.x;
	y += vec.y;
	z += vec.z;

	//Return this
	return *this;
}

Vec3 Vec3::operator-(const Vec3& vec)
{
	//Add the components of the vectors being added
	return Vec3(x - vec.x, y - vec.y, z - vec.z);
}

Vec3 Vec3::operator-=(const Vec3& vec)
{
	//Add the components of the vectors being added
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;

	//Return this
	return *this;
}

Vec3 Vec3::operator/(float scalar)
{
	return Vec3(x / scalar, y / scalar, z / scalar);
}

//Returns the dot product of this vector and another.
float Vec3::operator*(const Vec3& vec)
{
	//Return the product of the vectors' components
	return x * vec.x + y * vec.y + z * vec.z;
}

//Returns the dot product of this vector and a scalar.
Vec3 Vec3::operator*(float scalar)
{
	return Vec3(x * scalar, y * scalar, z * scalar);
}

bool Vec3::operator<(const Vec3& vec)
{
	if(x < vec.x || y < vec.y || z < vec.z)
		return false;
	return true;
}

bool Vec3::operator>(const Vec3& vec)
{
	if(x > vec.x || y > vec.y || z > vec.z)
		return false;
	return true;
}

bool Vec3::operator<=(const Vec3& vec)
{
	if(x <= vec.x || y <= vec.y || z <= vec.z)
		return false;
	return true;
}

bool Vec3::operator>=(const Vec3& vec)
{
	if(x >= vec.x || y >= vec.y || z >= vec.z)
		return false;
	return true;
}

//Returns the cross product of this vector and another.
Vec3 Vec3::Cross(const Vec3& vec)
{
	//Return the cross product of the two vectors
	return Vec3(y * vec.z - z * vec.y, -x * vec.z + z * vec.x, x * vec.y - y * vec.x);
}

Vec3 Vec3::Normalize()
{
	//Return the unit vector in the direction of this vector
	return this->operator/(Magnitude());
}

float Vec3::Magnitude()
{
	return sqrt(x * x + y * y + z * z);
}

//Takes the modulus(length) of a vector
float Vec3::Modulus(const Vec3& vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
}