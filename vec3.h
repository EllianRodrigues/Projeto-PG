#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
#include <random>

using std::sqrt;

double random_double() {
    static std::random_device rd;  
    static std::mt19937 gen(rd()); 
    std::uniform_real_distribution<> dis(0.0, 1.0);  

    return dis(gen);
}

double random_double(double min, double max) {
    std::uniform_real_distribution<> dis(min, max);  
    static std::random_device rd;   
    static std::mt19937 gen(rd());  
    return dis(gen);  
}

class vec3 { // criamos essa classe para representar tanto um vetor quanto um ponto no espaço R3
  public:
    double e[3]; // armazena os componentes x,y,z

    vec3() : e{0,0,0} {} //inicializa todos como 0
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {} // inicializa com valores específicos

    double x() const { return e[0]; } // são os componentes inicialziados antes
    double y() const { return e[1]; } 
    double z() const { return e[2]; }

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); } // retorna o vetor oposto posi troca o sinal de todos os componentes
    double operator[](int i) const { return e[i]; } // acessa algum índice ou modifica como um array
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3& v) { // soma com outro vetor 
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(double t) { // multiplica com um escalar
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(double t) { // divide com um escalar
        return *this *= 1/t;
    }

    double length() const { // retorna o tamanho
        return std::sqrt(length_squared());
    }

    double length_squared() const { // comprimento ao quadrado
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    // bool near_zero() const { // ver se é próximo de zero
    //     // Retorna verdadeiro se for
    //     auto s = 1e-8;
    //     return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    // }

    static vec3 random() {
        return vec3(random_double(), random_double(), random_double());
    }

    static vec3 random(double min, double max) {
        return vec3(random_double(min,max), random_double(min,max), random_double(min,max));
    }
};

// indicando que point3 é um ponto no espaço 3D.
using point3 = vec3;

// Vector Utility Functions
inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3& u, const vec3& v) { // produto interno entre dois vetores
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) { // produto vetorial entre dois vetores, para encontrar o perpendicular aos dois
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(const vec3& v) { //cria um vetor unitario a partir do original (serve apenas para fazer a direção do vetor sem se importar com seu tamanho)
    return v / v.length();
}

inline vec3 random_in_unit_disk() {
    while (true) {
        auto p = vec3(random_double(-1,1), random_double(-1,1), 0);
        if (p.length_squared() < 1)
            return p;
    }
}

inline vec3 random_unit_vector() {
    while (true) {
        auto p = vec3::random(-1,1);
        auto lensq = p.length_squared();
        if (1e-160 < lensq && lensq <= 1.0)
            return p / sqrt(lensq);
    }
}

inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

// inline vec3 reflect(const vec3& v, const vec3& n) {
//     return v - 2*dot(v,n)*n;
// }

// inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
//     auto cos_theta = std::fmin(dot(-uv, n), 1.0);
//     vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
//     vec3 r_out_parallel = -std::sqrt(std::fabs(1.0 - r_out_perp.length_squared())) * n;
//     return r_out_perp + r_out_parallel;
// }


#endif