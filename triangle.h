#ifndef TRIANGLE_H
#define TRIANGLE_H


#include "hittable.h"
#include "material.h"


class triangle : public hitable {
public:
    vec3 vertex1, vertex2, vertex3;
    vec3 cor;
    Material material; // Adicionando material

    triangle() {}

    triangle(const vec3& v1, const vec3& v2, const vec3& v3, const vec3& cor, const Material& mat)
        : vertex1(v1), vertex2(v2), vertex3(v3), cor(cor), material(mat) {}

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override {
        vec3 e1 = vertex2 - vertex1;
        vec3 e2 = vertex3 - vertex1;
        vec3 h = cross(r.direction(), e2);
        float a = dot(e1, h);

        if (a > -1e-6 && a < 1e-6)
            return false;

        float f = 1.0 / a;
        vec3 s = r.origin() - vertex1;
        float u = f * dot(s, h);

        if (u < 0.0 || u > 1.0)
            return false;

        vec3 q = cross(s, e1);
        float v = f * dot(r.direction(), q);

        if (v < 0.0 || u + v > 1.0)
            return false;

        float t = f * dot(e2, q);

        if (t > t_min && t < t_max) {
            rec.t = t;
            rec.p = r.at(t);
            rec.normal = unit_vector(cross(e1, e2));
            rec.cor = cor;
            rec.material = material; // Armazena o material do triângulo
            return true;
        }

        return false;
    }
};

#endif
