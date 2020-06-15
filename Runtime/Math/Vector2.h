#pragma once

#include <string>

#include "../Core/EngineDefs.h"

namespace V::Math
{
	class V_API Vector2
	{
	public:
		Vector2()
		{
			x = 0;
			y = 0;
		}

		Vector2(const Vector2& vector)
		{
			this->x = vector.x;
			this->y = vector.y;
		}

		Vector2(float x, float y)
		{
			this->x = x;
			this->y = y;
		}
		Vector2(int x, int y)
		{
			this->x = static_cast<float>(x);
			this->y = static_cast<float>(y);
		}
		Vector2(uint32_t x, uint32_t y)
		{
			this->x = static_cast<float>(x);
			this->y = static_cast<float>(y);
		}
		Vector2(float x)
		{
			this->x = x;
			this->y = x;
		}
		~Vector2() = default;

		Vector2 operator+(const Vector2& b) const
		{
			return Vector2
			(
				this->x + b.x,
				this->y + b.y
			);
		}

		void operator+=(const Vector2& b)
		{
			this->x += b.x;
			this->y += b.y;
		}

		Vector2 operator*(const Vector2& b) const
		{
			return Vector2(x * b.x, y * b.y);
		}

		void operator*=(const Vector2& b)
		{
			x *= b.x;
			y *= b.y;
		}

		Vector2 operator*(const float value) const
		{
			return Vector2(x * value, y * value);
		}

		void operator*=(const float value)
		{
			x *= value;
			y *= value;
		}

		Vector2 operator-(const Vector2& b) const { return Vector2(x - b.x, y - b.y); }
		Vector2 operator-(const float value) const { return Vector2(x - value, y - value); }

		void operator-=(const Vector2& rhs)
		{
			x -= rhs.x;
			y -= rhs.y;
		}

		Vector2 operator/(const Vector2& rhs) const { return Vector2(x / rhs.x, y / rhs.y); }
		Vector2 operator/(const float rhs) const { return Vector2(x / rhs, y / rhs); }

		void operator/=(const Vector2& rhs)
		{
			x /= rhs.x;
			y /= rhs.y;
		}

		bool operator==(const Vector2& b) const
		{
			return x == b.x && y == b.y;
		}

		bool operator!=(const Vector2& b) const
		{
			return x != b.x || y != b.y;
		}

		[[nodiscard]] const float* Data() const { return &x; }
		std::string ToString() const;

		float x;
		float y;
		static const Vector2 Zero;
		static const Vector2 One;

	};
}