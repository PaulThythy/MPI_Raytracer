#pragma once

#include <iostream>
#include <cmath>

#include "vector3.h"

struct Sphere {
    Vector3 center;
    double radius;
    //add material member

    inline Sphere(const Vector3& _center, const double _radius): center(_center), radius(_radius) {}

    inline ~Sphere() {}

    inline Vector3 normalAt(const Vector3& intersection) const {
        Vector3 norm(intersection.x - center.x, intersection.y - center.y, intersection.z - center.z);
        return norm.normalize();
    }

    inline double distance(const Vector3& point) {
        int x1 = pow(point.x - this->center.x, 2);
        int y1 = pow(point.y - this->center.y, 2);
        int z1 = pow(point.z - this->center.z, 2);

        return sqrt(x1 + y1 + z1);
    }

    inline bool isInside(const Vector3& point) {
        return distance(point) < (pow(this->radius, 2));
    }

    inline bool isOn(const Vector3& point) {
        return distance(point) == (pow(this->radius, 2));
    }

    inline bool isOutside(const Vector3& point) {
        return distance(point) > (pow(this->radius, 2));
    }
};