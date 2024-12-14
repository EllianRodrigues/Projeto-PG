#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "vec3.h"

class camera {
public:
    camera(
        vec3 lookfrom,       // Posição da câmera no espaço
        vec3 lookat,         // Ponto para onde a câmera está apontando
        vec3 vup,            // Vetor que aponta para "cima"
        double distance,     // Distância entre a câmera e a tela
        double screen_height,// Altura da tela (em unidades do mundo)
        double screen_width, // Largura da tela (em unidades do mundo)
        int h_res            // Resolução horizontal da tela
    ) {
        // Vetores ortonormais da câmera
        vec3 w = unit_vector(lookfrom - lookat); // Vetor que aponta "para trás"
        vec3 u = unit_vector(cross(vup, w));     // Vetor "para a direita"
        vec3 v = cross(w, u);                    // Vetor "para cima"

        // Guardando os vetores
        this->u = u;
        this->v = v;
        this->w = w;

        // Guardando a posição da câmera
        origin = lookfrom;

        // Calculando os limites da tela no espaço 3D
        lower_left_corner = origin - distance * w
                            - (screen_width / 2.0) * u
                            - (screen_height / 2.0) * v;
        horizontal = screen_width * u;
        vertical = screen_height * v;

        // Resolução
        this->h_res = h_res;
    }

    ray get_ray(double s, double t) const {
        // Retorna o raio que parte da câmera passando por um ponto (s, t) na tela
        return ray(origin, lower_left_corner + s * horizontal + t * vertical - origin);
    }

public:
    vec3 origin;             // Posição da câmera
    vec3 lower_left_corner;  // Canto inferior esquerdo da tela
    vec3 horizontal;         // Vetor horizontal da tela
    vec3 vertical;           // Vetor vertical da tela
    vec3 u, v, w;            // Vetores ortonormais
    int h_res;               // Resolução horizontal
};

#endif
