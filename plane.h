#ifndef PLANE_H
#define PLANE_H

#include "hittable.h"

class plane : public hitable {
public:
    vec3 point;   // Um ponto no plano
    vec3 normal;  // Vetor normal ao plano
    vec3 cor;     // Cor do plano

    plane() {}
    // Atualizando o construtor para aceitar a cor
    plane(const vec3& p, const vec3& n, const vec3& cor) : point(p), normal(unit_vector(n)), cor(cor) {}

    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override {
        float denom = dot(normal, r.direction());
        if (fabs(denom) > 1e-6) { // Verifica se o raio não é paralelo ao plano
            float t = dot(point - r.origin(), normal) / denom;
            if (t < t_max && t > t_min) {
                rec.t = t;
                rec.p = r.at(t);
                rec.normal = normal;
                rec.cor = cor;  // Passando a cor para o hit_record
                return true;
            }
        }
        return false;
    }
};


#endif
