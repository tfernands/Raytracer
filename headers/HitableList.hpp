#ifndef HITABLELISTHPP
#define HITABLELISTHPP

#include "Hitable.hpp"

class HitableList: public Hitable{
public:

	Hitable **list;
	int list_size;

	HitableList(Hitable **list, int list_size){
		this->list = list;
		this->list_size = list_size;
	}
	virtual ~HitableList(){}
	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const{
		HitRecord temp_rec;
		bool hit_anything = false;
		double closest_so_far = t_max;
		for (int i = 0; i < list_size; i++){
			if (list[i]->hit(r, t_min, closest_so_far, temp_rec)){
				hit_anything = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;
			}
		}
		return hit_anything;
	}
};


#endif
