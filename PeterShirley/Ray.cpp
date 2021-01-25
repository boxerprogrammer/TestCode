#include "Ray.h"
Ray::Ray() {

}
Ray::Ray(const Position3& org, const Vector3& d) :origin_(org), dir_(d) {}

Position3 
Ray::Origin()const {
	return origin_;
}
Vector3 
Ray::Direction()const {
	return dir_;
}