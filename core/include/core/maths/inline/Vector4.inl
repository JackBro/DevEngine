/**
 * @file maths/inline/Vector4.inl
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

// Public

Float32 Vector4::length() const
{
	return squareRoot(x * x + y * y + z * z + w * w);
}

void Vector4::normalise()
{
	operator /=(length());
}

Vector3 Vector4::xyz() const
{
	return Vector3(x, y, z);
}

// Operators

Vector4 Vector4::operator -() const
{
	return Vector4(-x, -y, -z, -w);
}

Float32& Vector4::operator [](const Uint32 index)
{
	DE_ASSERT(index < 4u);
	return (&x)[index];
}

const Float32& Vector4::operator [](const Uint32 index) const
{
	DE_ASSERT(index < 4u);
	return (&x)[index];
}

// Static

Float32 Vector4::distance(const Vector4& vectorA, const Vector4& vectorB)
{
	return (vectorB - vectorA).length();
}

Float32 Vector4::dot(const Vector4& vectorA, const Vector4& vectorB)
{
	return vectorA.x * vectorB.x + vectorA.y * vectorB.y + vectorA.z * vectorB.z + vectorA.w * vectorB.w;
}

Vector4 Vector4::lerp(const Vector4& vectorA, const Vector4& vectorB, const Float32 weight)
{
	return weight * (vectorB - vectorA) + vectorA;
}

Vector4 Vector4::maximum(const Vector4& vectorA, const Vector4& vectorB)
{
	return Vector4
	(
		Maths::maximum(vectorA.x, vectorB.x),
		Maths::maximum(vectorA.y, vectorB.y),
		Maths::maximum(vectorA.z, vectorB.z),
		Maths::maximum(vectorA.w, vectorB.w)
	);
}

Vector4 Vector4::minimum(const Vector4& vectorA, const Vector4& vectorB)
{
	return Vector4
	(
		Maths::minimum(vectorA.x, vectorB.x),
		Maths::minimum(vectorA.y, vectorB.y),
		Maths::minimum(vectorA.z, vectorB.z),
		Maths::minimum(vectorA.w, vectorB.w)
	);
}

Vector4 Vector4::normal(Vector4 vector)
{
	vector.normalise();
	return vector;
}


// Maths

Vector4 operator +(Vector4 vectorA, const Vector4& vectorB)
{
	vectorA += vectorB;
	return vectorA;
}

Vector4 operator +(Vector4 vector, const Float32 scalar)
{
	vector += scalar;
	return vector;
}

Vector4 operator +(const Float32 scalar, Vector4 vector)
{
	vector += scalar;
	return vector;
}

Vector4 operator -(Vector4 vectorA, const Vector4& vectorB)
{
	vectorA -= vectorB;
	return vectorA;
}

Vector4 operator -(Vector4 vector, const Float32 scalar)
{
	vector -= scalar;
	return vector;
}

Vector4 operator *(Vector4 vectorA, const Vector4& vectorB)
{
	vectorA *= vectorB;
	return vectorA;
}

Vector4 operator *(Vector4 vector, const Float32 scalar)
{
	vector *= scalar;
	return vector;
}

Vector4 operator *(const Float32 scalar, Vector4 vector)
{
	vector *= scalar;
	return vector;
}

Vector4 operator /(Vector4 vectorA, const Vector4& vectorB)
{
	vectorA /= vectorB;
	return vectorA;
}

Vector4 operator /(Vector4 vector, const Float32 scalar)
{
	vector /= scalar;
	return vector;
}

Bool operator ==(const Vector4& vectorA, const Vector4& vectorB)
{
	return vectorA.x == vectorB.x && vectorA.y == vectorB.y && vectorA.z == vectorB.z && vectorA.w == vectorB.w;
}

Bool operator !=(const Vector4& vectorA, const Vector4& vectorB)
{
	return !(vectorA == vectorB);
}