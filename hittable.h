#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "material.h" // Inclui o novo arquivo de materiais

struct hit_record {
    float t;
    vec3 p;
    vec3 normal;
    vec3 cor;
    string objeto;
    Material material; // Novo atributo para armazenar o material do objeto atingido
};

class hitable {
    public:
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};

#endif
