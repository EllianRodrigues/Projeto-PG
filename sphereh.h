#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere: public hitable {
    public:
        sphere() {}
        // Atualizando o construtor para aceitar vec3 como cor
        sphere(vec3 cen, float r, vec3 cor): center(cen), radius(r), cor(cor) {};  
        virtual bool hit(const ray&r, float tmin, float tmax, hit_record& rec) const;
        vec3 center;
        float radius;
        vec3 cor;  // Agora a cor é armazenada como um vec3 (RGB)
        
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
            // rec.cor = vec3(1.0, 0.0, 0.0); // Vermelho puro (RGB: 1.0, 0.0, 0.0)
            rec.objeto="esfera";
            rec.cor = cor;
            return true;  // O raio intersecta a esfera
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;  // Armazena a distância da interseção
            rec.p = r.at(rec.t);  // Armazena o ponto de interseção
            rec.normal = (rec.p - center) / radius;  // Normal no ponto de interseção
            // rec.cor = vec3(1.0, 0.0, 0.0); // Vermelho puro (RGB: 1.0, 0.0, 0.0)
            rec.objeto="esfera";
            rec.cor = cor;
            return true;  // O raio intersecta a esfera
        }
    }
    return false;  // O raio não intersecta a esfera
}


#endif