#ifndef PLANE_H
#define PLANE_H

#include "hittable.h"

class plane : public hitable {
public:
    vec3 point;   // Um ponto no plano
    vec3 normal;  // Vetor normal ao plano

    plane() {}
    plane(const vec3& p, const vec3& n) : point(p), normal(unit_vector(n)) {}

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override {
        float denom = dot(normal, r.direction());
        if (fabs(denom) > 1e-6) { // Verifica se o raio não é paralelo ao plano
            float t = dot(point - r.origin(), normal) / denom;
            if (t < t_max && t > t_min) {
                rec.t = t;
                rec.p = r.at(t);
                rec.normal = normal;
                return true;
            }
        }
        return false;
    }
};

#endif
