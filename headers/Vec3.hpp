#ifndef VEC3HPP
#define VEC3HPP

#include <math.h>
#include <stdlib.h>
#include <iostream>

class Vec3{
public:

	float e[3];

	Vec3() {}
	Vec3(float e0, float e1, float e2) {
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
	}
	virtual ~Vec3(){}

	inline Vec3& set(float e0, float e1, float e2){
		e[0] = e0;
		e[1] = e1;
		e[2] = e2;
		return *this;
	}

	inline float operator[](int i) const { return e[i]; }
	inline float& operator[](int i) { return e[i]; }

	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }
	inline float z() const { return e[2]; }
	inline float r() const { return e[0]; }
	inline float g() const { return e[1]; }
	inline float b() const { return e[2]; }

	inline float length() const {
		return sqrt(squared_length());
	}

	inline float squared_length() const {
		return e[0]*e[0] + e[1] * e[1] + e[2]*e[2];
	}

	static inline float dot(const Vec3 &v1, const Vec3 &v2){
		return v1.e[0]*v2.e[0] + v1.e[1]*v2.e[1] + v1.e[2]*v2.e[2];
	}

	static inline Vec3 cross(const Vec3 &v1, const Vec3 &v2){
		return Vec3( v1.e[1]*v2.e[2]-v1.e[2]*v2.e[1],
					-(v1.e[0]*v2.e[2]-v1.e[2]*v2.e[0]),
					 v1.e[0]*v2.e[1]-v1.e[1]*v2.e[0] );
	}

	static inline Vec3 unit_vector(Vec3 v){
		return v / v.length();
	}

	inline Vec3& operator+=(const Vec3 &v){
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}
	inline Vec3& operator-=(const Vec3 &v){
		e[0] -= v.e[0];
		e[1] -= v.e[1];
		e[2] -= v.e[2];
		return *this;
	}
	inline Vec3& operator*=(const Vec3 &v){
		e[0] *= v.e[0];
		e[1] *= v.e[1];
		e[2] *= v.e[2];
		return *this;
	}
	inline Vec3& operator/=(const Vec3 &v){
		e[0] /= v.e[0];
		e[1] /= v.e[1];
		e[2] /= v.e[2];
		return *this;
	}

	inline Vec3& operator*=(const float s){
		e[0] *= s;
		e[1] *= s;
		e[2] *= s;
		return *this;
	}

	inline Vec3& operator/=(const float s){
		e[0] /= s;
		e[1] /= s;
		e[2] /= s;
		return *this;
	}

	inline Vec3& clip(float min_value, float max_value){
		if (e[0] < min_value)
			e[0] = min_value;
		if (e[1] < min_value)
			e[1] = min_value;
		if (e[2] < min_value)
			e[2] = min_value;
		if (e[0] > max_value)
			e[0] = max_value;
		if (e[1] > max_value)
			e[1] = max_value;
		if (e[2] > max_value)
			e[2] = max_value;
		return *this;		
	}

	friend inline Vec3 operator+(const Vec3 &v1, const Vec3 &v2){
		return Vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
	}

	friend inline Vec3 operator-(const Vec3 &v1, const Vec3 &v2){
		return Vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
	}

	friend inline Vec3 operator-(const Vec3 &v){
		return Vec3(-v.e[0], -v.e[1], -v.e[2]);
	}

	friend inline Vec3 operator*(const Vec3 &v1, const Vec3 &v2){
		return Vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
	}

	friend inline Vec3 operator/(const Vec3 &v1, const Vec3 &v2){
		return Vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
	}

	//float
	friend inline Vec3 operator*(const float s, const Vec3 &v2){
		return Vec3(s * v2.e[0], s * v2.e[1], s * v2.e[2]);
	}
	friend inline Vec3 operator*(const Vec3 &v1, const float s){
		return Vec3(v1.e[0] * s, v1.e[1] * s, v1.e[2] * s);
	}
	friend inline Vec3 operator/(const Vec3 &v1, const float s){
		return Vec3(v1.e[0] / s, v1.e[1] / s, v1.e[2] / s);
	}

	friend inline std::istream& operator>>(std::istream &in, Vec3 &v){
		in >> v.e[0] >> v.e[1] >> v.e[2];
		return in;
	}

	friend std::ostream& operator<<(std::ostream &out, Vec3 &v){
		out << "[" << v.e[0] << ", " << v.e[1] << ", " << v.e[2] << "]";
		return out;
	}
};

#endif