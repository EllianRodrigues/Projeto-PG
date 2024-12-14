#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    public:
        ray() {}
        ray(const point3& origin, const vec3& direction)
            : orig(origin), dir(direction)
        {}

        point3 origin() const  { return orig; } // retorna o ponto de origem
        vec3 direction() const { return dir; } // retorna o vetor de direção

        point3 at(double t) const { // retorna o ponto em que o raio esta depois de percorrer t tempo
            return orig + t*dir;
        }

    public:
        point3 orig;
        vec3 dir;
};

#endif