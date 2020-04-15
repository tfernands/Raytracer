#ifndef BVHNODEHPP
#define BVHNODEHPP

// Bounding Volume Hierarchies

#include <stdlib.h>
#include "Hitable.hpp"
#include "utils.hpp"

class BVHNode: public Hitable{
private:
	Hitable* left;
	Hitable* right;
	AABB box;

	static int box_compare_pos(const Hitable* a, const Hitable* b, int axis){
		AABB box_left, box_right;
		if (!a->bounding_box(box_left) || !b->bounding_box(box_right))
			std::cerr << "No bounding box in BHVNode constructor\n";
		if (box_left.min()[axis] - box_right.min()[axis] < 0.0f)
			return -1;
		else
			return 1;
	}

	static int box_x_compare(const void* a, const void* b){
		return box_compare_pos(*(Hitable**)a, *(Hitable**)b, 0);
	}

	static int box_y_compare(const void* a, const void* b){
		return box_compare_pos(*(Hitable**)a, *(Hitable**)b, 1);
	}

	static int box_z_compare(const void* a, const void* b){
		return box_compare_pos(*(Hitable**)a, *(Hitable**)b, 2);
	}

public:
	BVHNode(){}

	BVHNode(Hitable **l, int n){
		int axis = int(3*random());
		if (axis == 0){
			qsort(l, n, sizeof(Hitable*), box_x_compare);
		}
		else if (axis == 1){
			qsort(l, n, sizeof(Hitable*), box_y_compare);
		}
		else{
			qsort(l, n, sizeof(Hitable*), box_z_compare);
		}
		if (n == 1){
			left = right = l[0];
		}
		else if (n == 2){
			left = l[0];
			right = l[1];
		}
		else{
			left = new BVHNode(l, n/2);
			right = new BVHNode(l+n/2, n-n/2);
		}
		AABB box_left, box_right;
		if (!left->bounding_box(box_left) || !right->bounding_box(box_right))
			std::cerr << "No bounding box in BHVNode constructor\n";
		box = AABB::surrounding_box(box_left, box_right);
	}

	virtual ~BVHNode(){}

	virtual bool bounding_box(AABB& box) const override{
		box = this->box;
		return true;
	}

	virtual bool hit(const Ray& r, float tmin, float tmax, HitRecord& rec) const override{
		if (box.hit(r, tmin, tmax)){
			HitRecord left_rec, right_rec;
			bool hit_left = left->hit(r, tmin, tmax, left_rec);
			bool hit_right = right->hit(r, tmin, tmax, right_rec);
			if (hit_left && hit_right){
				if (left_rec.t < right_rec.t)
					rec = left_rec;
				else
					rec = right_rec;
				return true;
			}
			else if (hit_left){
				rec = left_rec;
				return true;
			}
			else if (hit_right){
				rec = right_rec;
				return true;
			}
			else{
				return false;
			}
		}
		return false;
	}

};


#endif