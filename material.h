#ifndef MATERIAL_H
#define MATERIAL_H

struct Material {
    float difuso;        // Coeficiente difuso
    float especular;     // Coeficiente especular
    float ambiental;     // Coeficiente ambiental
    float reflexao;      // Coeficiente de reflexão
    float transmissao;   // Coeficiente de transmissão
    float rugosidade;    // Coeficiente de rugosidade
    float shininess;     // Coeficiente de brilho especular (novo)
    vec3 color;          // Cor do material (nova adição)
    float ior; // Índice de refração

    Material() 
        : difuso(0.5), especular(0.5), ambiental(0.1), reflexao(0.2), 
          transmissao(0.0), rugosidade(0.5), shininess(32.0f), color(vec3(1.0, 1.0, 1.0)) {}

    Material(float d, float e, float a, float r, float t, float rug, float s, vec3 col,  float ior_val = 1.0f)
        : difuso(d), especular(e), ambiental(a), reflexao(r), transmissao(t),
          rugosidade(rug), shininess(s), color(col), ior(ior_val) {}
};

#endif
