#ifndef LIGHT_H
#define LIGHT_H

#include "vec3.h"  // Certifique-se de incluir a definição do vetor 3D (vec3)

class light {
public:
    vec3 position;  // Posição da luz no espaço 3D
    vec3 color;     // Cor da luz (RGB)
    float intensity; // Intensidade da luz

    // Construtor para inicializar a luz
    light(const vec3& pos, const vec3& col, float inten)
        : position(pos), color(col), intensity(inten) {}
};

#endif
