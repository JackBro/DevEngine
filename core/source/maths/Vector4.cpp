/**
 * @file maths/Vector4.cpp
 *
 * DevEngine
 * Copyright 2015 Eetu 'Devenec' Oinasmaa
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <core/maths/Vector4.h>

using namespace Maths;

// Public

const Vector4 Vector4::UNIT_X = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
const Vector4 Vector4::UNIT_Y = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
const Vector4 Vector4::UNIT_Z = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
const Vector4 Vector4::UNIT_W = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
const Vector4 Vector4::ZERO	  = Vector4(0.0f, 0.0f, 0.0f, 0.0f);

Vector4::Vector4(const Float32 value)
	: Vector4(value, value, value, value) { }

Vector4::Vector4(const Float32 x, const Float32 y, const Float32 z, const Float32 w)
	: x(x),
	  y(y),
	  z(z),
	  w(w) { }

Vector4::Vector4(const Vector2& vector, const Float32 z, const Float32 w)
	: Vector4(vector.x, vector.y, z, w) { }

Vector4::Vector4(const Vector3& vector, const Float32 w)
	: Vector4(vector.x, vector.y, vector.z, w) { }

// Operators

Vector4& Vector4::operator +=(const Vector4& vector)
{
	x += vector.x;
	y += vector.y;
	z += vector.z;
	w += vector.w;

	return *this;
}

Vector4& Vector4::operator +=(const Float32 scalar)
{
	x += scalar;
	y += scalar;
	z += scalar;
	w += scalar;

	return *this;
}

Vector4& Vector4::operator -=(const Vector4& vector)
{
	x -= vector.x;
	y -= vector.y;
	z -= vector.z;
	w -= vector.w;

	return *this;
}

Vector4& Vector4::operator -=(const Float32 scalar)
{
	x -= scalar;
	y -= scalar;
	z -= scalar;
	w -= scalar;

	return *this;
}

Vector4& Vector4::operator *=(const Vector4& vector)
{
	x *= vector.x;
	y *= vector.y;
	z *= vector.z;
	w *= vector.w;

	return *this;
}

Vector4& Vector4::operator *=(const Float32 scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;

	return *this;
}

Vector4& Vector4::operator /=(const Vector4& vector)
{
	DE_ASSERT(vector.x != 0.0f && vector.y != 0.0f && vector.z != 0.0f && vector.w != 0.0f);
	x /= vector.x;
	y /= vector.y;
	z /= vector.z;
	w /= vector.w;

	return *this;
}

Vector4& Vector4::operator /=(const Float32 scalar)
{
	DE_ASSERT(scalar != 0.0f);
	x /= scalar;
	y /= scalar;
	z /= scalar;
	w /= scalar;

	return *this;
}