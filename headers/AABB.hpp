#ifndef AABBHPP
#define AABBHPP

#include "Vec3.hpp"
#include "Ray.hpp"


inline float ffmin(float a, float b) { return a < b ? a : b;}
inline float ffmax(float a, float b) { return a > b ? a : b;}

class AABB {
private:
	Vec3 _min, _max;

public:

	AABB() {}
	AABB(const Vec3& a, const Vec3& b){
		_min = a;
		_max = b;
	}

	Vec3 min() const {return _min;}
	Vec3 max() const {return _max;}

	inline bool hit(const Ray& r, float tmin, float tmax) const {
		for (int dim = 0; dim < 3; dim++){
			float invD = 1.0f / r.direction()[dim];
			float t0 = (_min[dim] - r.origin()[dim])*invD;
			float t1 = (_max[dim] - r.origin()[dim])*invD;
			if (invD < 0.0f)
				std::swap(t0, t1);
			tmin = t0 > tmin ? t0 : tmin;
			tmax = t1 < tmax ? t1 : tmax;
			if (tmax <= tmin)
				return false;
		}
		return true;
	}

	static AABB surrounding_box(AABB box1, AABB box2){
		Vec3 min(	ffmin(box1.min().x(), box2.min().x()),
					ffmin(box1.min().y(), box2.min().y()),
					ffmin(box1.min().z(), box2.min().z())	);
		Vec3 max(	ffmax(box1.max().x(), box2.max().x()),
					ffmax(box1.max().y(), box2.max().y()),
					ffmax(box1.max().z(), box2.max().z())	);
		return AABB(min, max);
	}
};

#endif