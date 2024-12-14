#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere: public hitable {
    public:
        sphere(){}
        sphere(vec3 cen, float r): center(cen), radius(r) {};
        virtual bool hit(const ray&r, float tmin, float tmax, hit_record& rec) const;
        vec3 center;
        float radius;
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    vec3 oc = r.origin() - center;  // Vetor que vai do centro da esfera à origem do raio
    float a = dot(r.direction(), r.direction());  // Componente quadrática da direção do raio
    float b = dot(oc, r.direction());  // Componente linear
    float c = dot(oc, oc) - radius * radius;  // Componente constante (distância ao centro da esfera menos o raio ao quadrado)
    float discriminant = b * b - a * c;  // Determinante da equação quadrática
    if (discriminant > 0) {
        // O raio intersecta a esfera em dois pontos, calculamos ambos
        float temp = (-b - sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;  // Armazena a distância da interseção
            rec.p = r.at(rec.t);  // Armazena o ponto de interseção
            rec.normal = (rec.p - center) / radius;  // Normal no ponto de interseção
            return true;  // O raio intersecta a esfera
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;  // Armazena a distância da interseção
            rec.p = r.at(rec.t);  // Armazena o ponto de interseção
            rec.normal = (rec.p - center) / radius;  // Normal no ponto de interseção
            return true;  // O raio intersecta a esfera
        }
    }
    return false;  // O raio não intersecta a esfera
}


#endif