#ifndef MATERIAL_H
#define MATERIAL_H

struct Material {
    float difuso;        // Coeficiente difuso
    float especular;     // Coeficiente especular
    float ambiental;     // Coeficiente ambiental
    float reflexao;      // Coeficiente de reflexão
    float transmissao;   // Coeficiente de transmissão
    float rugosidade;    // Coeficiente de rugosidade
    float refracao;     // Coeficiente de refração (entrega 4)
    vec3 color;          // Cor do material (nova adição)

    Material() 
        : difuso(0.5), especular(0.5), ambiental(0.1), reflexao(0.2), 
          transmissao(0.0), rugosidade(0.5), refracao(0.1), color(vec3(1.0, 1.0, 1.0)) {}

    Material(float d, float e, float a, float r, float t, float rug, float rf, vec3 col)
        : difuso(d), especular(e), ambiental(a), reflexao(r), transmissao(t),
          rugosidade(rug), refracao(rf), color(col) {}
};

#endif
