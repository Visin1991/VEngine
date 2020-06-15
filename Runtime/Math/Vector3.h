#pragma once

#include <string>
#include <cmath>

#include "../Core/EngineDefs.h"
#include "MathHelper.h"

namespace V::Math
{
	class Vector4;

	class V_API Vector3
	{
	public:
		Vector3()
		{
			x = 0;
			y = 0;
			z = 0;
		}

		Vector3(const Vector3& vector)
		{
			x = vector.x;
			y = vector.y;
			z = vector.z;
		}

		Vector3(const Vector4& vector);

		Vector3(float x, float y, float z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		Vector3(float f)
		{
			x = f;
			y = f;
			z = f;
		}

		void Normalize()
		{
			const auto length_squared = LengthSquared();
			if (!Helper::Equals(length_squared, 1.0f) && length_squared > 0.0f)
			{
				const auto length_inverted = 1.0f / Helper::Sqrt(length_squared);
				x *= length_inverted;
				y *= length_inverted;
				z *= length_inverted;
			}
		};

		[[nodiscard]] Vector3 Normalized() const
		{
			const auto length_squared = LengthSquared();
			if (!Helper::Equals(length_squared, 1.0f) && length_squared > 0.0f)
			{
				const auto length_inverted = 1.0f / Helper::Sqrt(length_squared);
				return (*this) * length_inverted;
			}
			else
				return *this;
		}

		static inline Vector3 Normalize(const Vector3& v) { return v.Normalized(); }

		static inline float Dot(const Vector3& v1, const Vector3& v2) { return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z); }

		[[nodiscard]] float Dot(const Vector3& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z; }

		static inline Vector3 Cross(const Vector3& v1, const Vector3& v2)
		{
			return Vector3
			(
				v1.y * v2.z - v2.y * v1.z,
				-(v1.x * v2.z - v2.x * v1.z),
				v1.x * v2.y - v2.x * v1.y
			);
		}

		[[nodiscard]] Vector3 Cross(const Vector3& v2) const { return Cross(*this, v2); }

		[[nodiscard]] float Length() const { return Helper::Sqrt(x * x + y * y + z * z); }

		[[nodiscard]] float LengthSquared() const { return x * x + y * y + z * z; }

		static inline float Distance(const Vector3& a, const Vector3& b) { return (b - a).Length(); }

		static inline float DistanceSquared(const Vector3& a, const Vector3& b) { return (b - a).LengthSquared(); }

		void Floor()
		{
			x = floor(x);
			y = floor(y);
			z = floor(z);
		}

		[[nodiscard]] Vector3 Abs() const { return Vector3(Helper::Abs(x), Helper::Abs(y), Helper::Abs(z)); }

		Vector3 operator*(const Vector3& b) const
		{
			return Vector3(
				x * b.x,
				y * b.y,
				z * b.z
				);
		}

		void operator*=(const Vector3& b)
		{
			x *= b.x;
			y *= b.y;
			z *= b.z;
		}

		Vector3 operator*(const float value) const
		{
			return Vector3(
				x * value,
				y * value,
				z * value
				);
		}

		void operator*=(const float value)
		{
			x *= value;
			y *= value;
			z *= value;
		}

		Vector3 operator+(const Vector3& b) const { return Vector3(x + b.x, y + b.y, z + b.z); }
		Vector3 operator+(const float value) const { return Vector3(x + value, y + value, z + value); }

		void operator+=(const Vector3& b)
		{
			x += b.x;
			y += b.y;
			z += b.z;
		}

		void operator+=(const float value)
		{
			x += value;
			y += value;
			z += value;
		}

		Vector3 operator-(const Vector3& b) const { return Vector3(x - b.x, y - b.y, z - b.z); }
		Vector3 operator-(const float value) const { return Vector3(x - value, y - value, z - value); }

		void operator-=(const Vector3& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
			z -= rhs.z;
		}

		Vector3 operator/(const Vector3& rhs) const { return Vector3(x / rhs.x, y / rhs.y, z / rhs.z); }
		Vector3 operator/(const float rhs) const { return Vector3(x / rhs, y / rhs, z / rhs); }

		void operator/=(const Vector3& rhs)
		{
			x /= rhs.x;
			y /= rhs.y;
			z /= rhs.z;
		}

		// Test for equality without using epsilon
		bool operator==(const Vector3& rhs) const
		{
			return x == rhs.x && y == rhs.y && z == rhs.z;
		}

		// Test for inequality without using epsilon
		bool operator!=(const Vector3& rhs) const
		{
			return !(*this == rhs);
		}

		// Return negation.
		Vector3 operator -() const { return Vector3(-x, -y, -z); }

		[[nodiscard]] std::string ToString() const;
		[[nodiscard]] const float* Data() const { return &x; }

		float x;
		float y;
		float z;

		static const Vector3 Zero;
		static const Vector3 Left;
		static const Vector3 Right;
		static const Vector3 Up;
		static const Vector3 Down;
		static const Vector3 Forward;
		static const Vector3 Backward;
		static const Vector3 One;
		static const Vector3 Infinity;
		static const Vector3 InfinityNeg;
	};

}