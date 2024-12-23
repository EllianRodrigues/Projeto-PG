#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"

struct hit_record
{
    float t;
    vec3 p;
    vec3 normal;
    vec3 cor;
    string objeto;
};

class hitable {
    public:
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif
