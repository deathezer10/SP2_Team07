/******************************************************************************/
/*!
\file	Vector3.h
\author Wen Sheng Tang
\par	email: tang_wen_sheng\@nyp.edu.sg
\brief
Struct to define a 3D vector
*/
/******************************************************************************/

#ifndef VECTOR3_H
#define VECTOR3_H

#include "MyMath.h"
#include <iostream>

#pragma warning( disable: 4290 ) //for throw(DivideByZero)

/******************************************************************************/
/*!
		Class Vector3:
		\brief	Defines a 3D vector and its methods
		*/
/******************************************************************************/
struct Vector3 {
	float x, y, z;
	bool IsEqual(float a, float b) const;

	Vector3();
	Vector3(float xyz);
	Vector3(float a, float b, float c);
	Vector3(const Vector3 &rhs);
	~Vector3();

	std::string toString();

	Vector3& Set(float a = 0, float b = 0, float c = 0); //Set all data
	Vector3& SetZero(void); //Set all data to zero
	bool IsZero(void) const; //Check if data is zero

	bool IsFacingVector(Vector3& point, Vector3& forward, float fovDegrees);

	Vector3 operator+(const Vector3& rhs) const; //Vector addition
	Vector3& operator+=(const Vector3& rhs);

	Vector3 operator-(const Vector3& rhs) const; //Vector subtraction
	Vector3& operator-=(const Vector3& rhs);

	Vector3 operator-(void) const; //Unary negation

	Vector3 operator*(float scalar) const; //Scalar multiplication
	Vector3& operator*=(float scalar);

	bool operator==(const Vector3& rhs) const; //Equality check
	bool operator!= (const Vector3& rhs) const; //Inequality check

	Vector3& operator=(const Vector3& rhs); //Assignment operator

	float Length(void) const; //Get magnitude
	float HorizontalLength(void) const; // Get horizontal magnitude
	float LengthSquared(void) const; //Get square of magnitude
	float HorizontalLengthSquared(void) const;

	float Dot(const Vector3& rhs) const; //Dot product
	Vector3 Cross(const Vector3& rhs) const; //Cross product

	//Return a copy of this vector, normalized
	//Throw a divide by zero exception if normalizing a zero vector
	Vector3 Normalized(void) const throw(DivideByZero);

	//Normalize this vector and return a reference to it
	//Throw a divide by zero exception if normalizing a zero vector
	Vector3& Normalize(void) throw(DivideByZero);

	friend std::ostream& operator<<(std::ostream& os, Vector3& rhs); //print to ostream

	friend Vector3 operator*(float scalar, const Vector3& rhs); //what is this for?
};

#endif //VECTOR3_H
