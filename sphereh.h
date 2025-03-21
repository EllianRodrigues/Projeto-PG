#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "material.h"

class sphere: public hitable {
public:
    vec3 center;
    float radius;
    vec3 cor;
    Material material; // Adicionando material

    sphere() {}

    sphere(vec3 cen, float r, vec3 cor, const Material& mat)
        : center(cen), radius(r), cor(cor), material(mat) {}

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override {
        vec3 oc = r.origin() - center;
        float a = dot(r.direction(), r.direction());
        float b = dot(oc, r.direction());
        float c = dot(oc, oc) - radius * radius;
        float discriminant = b * b - a * c;

        if (discriminant > 0) {
            float temp = (-b - sqrt(discriminant)) / a;
            if (temp < t_max && temp > t_min) {
                rec.t = temp;
                rec.p = r.at(rec.t);
                rec.normal = (rec.p - center) / radius;
                rec.cor = cor;
                rec.objeto = "esfera";
                rec.material = material; // Armazena o material da esfera
                return true;
            }
            temp = (-b + sqrt(discriminant)) / a;
            if (temp < t_max && temp > t_min) {
                rec.t = temp;
                rec.p = r.at(rec.t);
                rec.normal = (rec.p - center) / radius;
                rec.cor = cor;
                rec.objeto = "esfera";
                rec.material = material; // Armazena o material da esfera
                return true;
            }
        }
        return false;
    }
};

#endif
