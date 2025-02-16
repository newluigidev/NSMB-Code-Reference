#pragma once
#include "nitro_if.h"
#include "math.h"


/*
*	3D Vector class
*/
class Vec3 : public VecFx32
{
public:

	inline Vec3() {}
	inline virtual ~Vec3() {}

	void applyRotYZ(fx16 angle);				// Rotates the vector around the x-axis
	void applyRotXZ(fx16 angle);				// Rotates the vector around the y-axis
	void applyRotXY(fx16 angle);				// Rotates the vector around the z-axis
	bool normalize();							// If x, y and z are not zero, the vector is normalized and returns true.
	bool operator!=(const VecFx32& in) const;	// returns true if vectors are not equal
	bool operator==(const VecFx32& in) const;	// returns true if vectors are equal
	void operator*=(fx32 in) ;					// Scalar multiplication

	static void normalize(Vec3& out, Vec3& in);									// Normalizes in and out
	static void cross(Vec3& out, Vec3& out2, const VecFx32& a, const VecFx32& b);		// Performs a x b and stores the result in out and out2
	static void cross(Vec3& out, const Vec3& a, const VecFx32& b);						// Performs a x b and stores the result in out
	static fx32 distance(const Vec3& a, const VecFx32& b);								// Calculates the distance between a and b
	static Vec3& mul(Vec3& out, const Vec3& v, fx32 factor);							// Scales v by factor and stores the result in out
	static Vec3& sub(Vec3& out, const Vec3& a, const VecFx32& b);						// Calculates a - b
	static Vec3& add(Vec3& out, const Vec3& a, const VecFx32& b);						// Calculates a + b

	// inline method re-creations.

	inline Vec3(fx32 val) {
		x = y = z = val;
	}

	inline Vec3(fx32 x, fx32 y, fx32 z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	inline Vec3(const VecFx32& vec) {
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}

	inline static Vec3& div(Vec3& out, const VecFx32& lhs, fx32 rhs) {
		out = Vec3(Math::div(lhs.x, rhs), Math::div(lhs.y, rhs), Math::div(lhs.z, rhs));
		return out;
	}

	inline static Vec3 lerp(const VecFx32& start, const VecFx32& end, fx32 step) {
		return Vec3(Math::lerpFx(start.x, end.x, step), Math::lerpFx(start.y, end.y, step), Math::lerpFx(start.z, end.z, step));
	}


	inline Vec3 operator+(const VecFx32& rhs) const {
		Vec3 out;
		Vec3::add(out, *this, rhs);
		return out;
	}

	inline Vec3 operator-(const VecFx32& rhs) const {
		Vec3 out;
		Vec3::sub(out, *this, rhs);
		return out;
	}

	inline Vec3 operator*(fx32 rhs) const {
		Vec3 out;
		Vec3::mul(out, *this, rhs);
		return out;
	}

	inline Vec3 operator/(fx32 rhs) const {
		Vec3 out;
		Vec3::div(out, *this, rhs);
		return out;
	}


	inline void operator+=(const VecFx32& rhs) {
		*this = *this + rhs;
	}

	inline void operator-=(const VecFx32& rhs) {
		*this = *this - rhs;
	}

	inline void operator/=(fx32 rhs) {
		*this = *this / rhs;
	}

	inline Vec3 operator-() const {
		Vec3 v;
		v.x = -x;
		v.y = -y;
		v.z = -z;
		return v;
	}

	inline Vec3 normalized() const {
		Vec3 out = *this;
		out.normalize();
		return out;
	}

	inline fx32 length() const {
		return VEC_Mag(this);
	}

	inline Vec3 cross(const Vec3& rhs) const {
		Vec3 out;
		Vec3::cross(out, *this, rhs);
		return out;
	}

	inline fx32 distance(const VecFx32& rhs) const {
		return Vec3::distance(*this, rhs);
	}

	inline void add(const VecFx32& rhs) {
		Vec3::add(*this, *this, rhs);
	}

	inline void sub(const VecFx32& rhs) {
		Vec3::sub(*this, *this, rhs);
	}

	inline void mul(fx32 rhs) {
		Vec3::mul(*this, *this, rhs);
	}

	inline void div(fx32 rhs) {
		Vec3::div(*this, *this, rhs);
	}
};



/*
*	2D Vector class
*/
class Vec2
{
public:

	fx32 x, y;

	inline Vec2() {}
	inline virtual ~Vec2() {}

	bool normalize();							// If x, and y are not zero, the vector is normalized and returns true.
	fx64 magnitudeSquared() const;				// Returns the squared magnitude (length^2).
	fx32 length() const;						// Returns the vector's length (magnitude).
	bool operator!=(const VecFx32& in) const;	// Returns true if vectors are not equal.
	bool operator==(const VecFx32& in) const;	// Returns true if vectors are equal.
	void operator*=(fx32 in);					// Scalar multiplication.

	static void normalize(Vec2& out, const Vec2& in);						// Normalizes in and stores the result in out.
	static fx32 crossLength(const Vec2& a, const VecFx32& b);				// Emulates a 3D cross product and returns the z-component of the resulting vector.
	static fx32 distance(const Vec2& a, const VecFx32& b);					// returns the distance between a and b.
	static void div(Vec2& out, const Vec2& v, fx32 divisor);				// Divides v by divisor.
	static void mul(Vec2& out, const Vec2& v, fx32 factor);					// Multiplies v by factor.
	static void sub(Vec2& out, const Vec2& a, const VecFx32& b);			// Calculates a - b.
	static void add(Vec2& out, const Vec2& a, const VecFx32& b);			// Calculates a + b.



	// inline methods re-creations.

	inline Vec2(fx32 val) {
		x = y = val;
	}

	inline Vec2(fx32 x, fx32 y) {
		this->x = x;
		this->y = y;
	}

	inline Vec2(const Vec2& vec) {
		x = vec.x;
		y = vec.y;
	}



	inline VecFx32 vecfx() const {
		return { x, y, 0 };
	}

	// !!! NOT SAFE !!!
	inline const VecFx32& vecfx_cr() const {
		return *reinterpret_cast<const VecFx32*>(this);
	}



	inline Vec2 operator+(const Vec2& rhs) const {
		VecFx32 tmp = { rhs.x, rhs.y, 0 };
		Vec2 out;
		Vec2::add(out, *this, tmp);
		return out;
	}

	inline Vec2 operator-(const Vec2& rhs) const {
		VecFx32 tmp = { rhs.x, rhs.y, 0 };
		Vec2 out;
		Vec2::sub(out, *this, tmp);
		return out;
	}

	inline Vec2 operator*(fx32 rhs) const {
		Vec2 out;
		Vec2::mul(out, *this, rhs);
		return out;
	}

	inline Vec2 operator/(fx32 rhs) const {
		Vec2 out;
		Vec2::div(out, *this, rhs);
		return out;
	}


	inline bool operator!=(const Vec2& in) {
		return *this != in.vecfx_cr();
	}

	inline bool operator==(const Vec2& in) {
		return *this == in.vecfx_cr();
	}

	inline void operator+=(const Vec2& rhs) {
		*this = *this + rhs;
	}

	inline void operator-=(const Vec2& rhs) {
		*this = *this - rhs;
	}

	inline void operator/=(fx32 rhs) {
		*this = *this / rhs;
	}

	inline Vec2 operator-() const {
		Vec2 v;
		v.x = -x;
		v.y = -y;
		return v;
	}

	inline s32 crossLength(const Vec2& rhs) const {
		VecFx32 tmp = { rhs.x, rhs.y, 0 };
		return Vec2::crossLength(*this, tmp);
	}

	inline fx32 distance(const Vec2& rhs) const {
		VecFx32 tmp = { rhs.x, rhs.y, 0 };
		return Vec2::distance(*this, tmp);
	}

	inline void add(const Vec2& rhs) {
		VecFx32 tmp = { rhs.x, rhs.y, 0 };
		Vec2::add(*this, *this, tmp);
	}

	inline void sub(const Vec2& rhs) {
		VecFx32 tmp = { rhs.x, rhs.y, 0 };
		Vec2::sub(*this, *this, tmp);
	}

	inline void mul(fx32 rhs) {
		Vec2::mul(*this, *this, rhs);
	}

	inline void div(fx32 rhs) {
		Vec2::div(*this, *this, rhs);
	}
};



/*
*	3D Vector class (fx16 type)
*/
class Vec3s : public VecFx16
{
public:

	inline Vec3s() {}
	inline virtual ~Vec3s() {}

	// inline methods re-creations.

	inline Vec3s(fx16 val) {
		x = y = z = val;
	}

	inline Vec3s(fx16 x, fx16 y, fx16 z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	inline static Vec3s& add(Vec3s& out, const VecFx16& lhs, const VecFx16& rhs) {
		out = Vec3s(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z);
		return out;
	}

	inline static Vec3s& sub(Vec3s& out, const VecFx16& lhs, const VecFx16& rhs) {
		out = Vec3s(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z);
		return out;
	}

	inline static Vec3s& mul(Vec3s& out, const VecFx16& lhs, fx32 rhs) {
		out = Vec3s(Math::mul(lhs.x, rhs), Math::mul(lhs.y, rhs), Math::mul(lhs.z, rhs));
		return out;
	}

	inline static Vec3s& div(Vec3s& out, const VecFx16& lhs, fx32 rhs) {
		out = Vec3s(Math::div(lhs.x, rhs), Math::div(lhs.y, rhs), Math::div(lhs.z, rhs));
		return out;
	}


	inline Vec3s operator+(const VecFx16& rhs) const {
		Vec3s out;
		Vec3s::add(out, *this, rhs);
		return out;
	}

	inline Vec3s operator-(const VecFx16& rhs) const {
		Vec3s out;
		Vec3s::sub(out, *this, rhs);
		return out;
	}

	inline Vec3s operator*(fx32 rhs) const {
		Vec3s out;
		Vec3s::mul(out, *this, rhs);
		return out;
	}

	inline Vec3s operator/(fx32 rhs) const {
		Vec3s out;
		Vec3s::div(out, *this, rhs);
		return out;
	}


	inline void operator+=(const VecFx16& rhs) {
		*this = *this + rhs;
	}

	inline void operator-=(const VecFx16& rhs) {
		*this = *this - rhs;
	}

	inline void operator*=(fx32 rhs) {
		*this = *this * rhs;
	}

	inline void operator/=(fx32 rhs) {
		*this = *this / rhs;
	}

	inline Vec3s operator-() const {
		Vec3s v;
		v.x = -x;
		v.y = -y;
		v.z = -z;
		return v;
	}

	inline bool operator==(const Vec3s& rhs) const {
		if (x == rhs.x && y == rhs.y && z == rhs.x) {
			return true;
		}

		return false;
	}

	inline bool operator!=(const Vec3s& rhs) const {
		if (x != rhs.x || y != rhs.y || z != rhs.x) {
			return true;
		}

		return false;
	}

};
