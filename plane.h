#ifndef PLANE_H
#define PLANE_H

#include "hittable.h"
#include "material.h"

class plane : public hitable {
public:
    vec3 point;
    vec3 normal;
    vec3 cor;
    Material material; // Adicionando material

    plane() {}

    plane(const vec3& p, const vec3& n, const vec3& cor, const Material& mat)
        : point(p), normal(unit_vector(n)), cor(cor), material(mat) {}

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override {
        float denom = dot(normal, r.direction());
        if (fabs(denom) > 1e-6) {
            float t = dot(point - r.origin(), normal) / denom;
            if (t < t_max && t > t_min) {
                rec.t = t;
                rec.p = r.at(t);
                rec.normal = normal;
                rec.cor = cor;
                rec.material = material; // Armazena o material do plano
                return true;
            }
        }
        return false;
    }
};

#endif
