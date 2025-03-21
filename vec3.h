#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>
#include <random>
#include <cmath>

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


class mat4 {
    public:
        double m[4][4];
    
        mat4() {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    m[i][j] = 0;
        }
    
        mat4(double m00, double m01, double m02, double m03,
             double m10, double m11, double m12, double m13,
             double m20, double m21, double m22, double m23,
             double m30, double m31, double m32, double m33) {
            m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
            m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
            m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
            m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
        }
    
        // Multiplicação de matriz 4x4 por vetor 3D
        vec3 operator*(const vec3& v) const {
            return vec3(
                m[0][0] * v.x() + m[0][1] * v.y() + m[0][2] * v.z() + m[0][3],
                m[1][0] * v.x() + m[1][1] * v.y() + m[1][2] * v.z() + m[1][3],
                m[2][0] * v.x() + m[2][1] * v.y() + m[2][2] * v.z() + m[2][3]
            );
        }
    };
    
    

   // Função de rotação no eixo Z (em 3D)
    mat4 rotation_matrix_z(double angle) {
        double cos_angle = cos(angle);
        double sin_angle = sin(angle);
        return mat4(
            cos_angle, -sin_angle, 0, 0,
            sin_angle, cos_angle,  0, 0,
            0,          0,         1, 0,
            0,          0,         0, 1
        );
    }

    // Função de rotação no eixo X (em 3D)
    mat4 rotation_matrix_x(double angle) {
        double cos_angle = cos(angle);
        double sin_angle = sin(angle);
        return mat4(
            1, 0, 0, 0,
            0, cos_angle, -sin_angle, 0,
            0, sin_angle, cos_angle, 0,
            0, 0, 0, 1
        );
    }

    // Função de rotação no eixo Y (em 3D)
    mat4 rotation_matrix_y(double angle) {
        double cos_angle = cos(angle);
        double sin_angle = sin(angle);
        return mat4(
            cos_angle, 0, sin_angle, 0,
            0, 1, 0, 0,
            -sin_angle, 0, cos_angle, 0,
            0, 0, 0, 1
        );
    }

    // Função de translação 3D
    mat4 translation_matrix(const vec3& t) {
        return mat4(
            1, 0, 0, t.x(),
            0, 1, 0, t.y(),
            0, 0, 1, t.z(),
            0, 0, 0, 1
        );
    }

    // Função de escala 3D
    mat4 scaling_matrix(double s) {
        return mat4(
            s, 0, 0, 0,
            0, s, 0, 0,
            0, 0, s, 0,
            0, 0, 0, 1
        );
    }

    
    


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

vec3 normalize(const vec3& v) {
    float len = v.length();  // Calcula o comprimento do vetor
    if (len == 0) return v;  // Se o comprimento for zero, não normaliza
    return v / len;  // Retorna o vetor normalizado
}

// float length(const vec3& v) {
//     return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
// }

inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}

// vec3 refract(const vec3& v, const vec3& n, float eta) {
//     float cos_theta = dot(-v, n);
//     vec3 r_out_perp = eta * (v + cos_theta * n);
//     vec3 r_out_parallel = -sqrt(fabs(1.0 - dot(r_out_perp, r_out_perp))) * n;
//     return r_out_perp + r_out_parallel;
// }

bool refract (const vec3& v, const vec3& n, float ni_over_nt, vec3& refracted) {
    vec3 uv = unit_vector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
    if (discriminant > 0) {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    } else {
        return false;
    }
}   

float length(const vec3& v) {
    return std::sqrt(v.x() * v.x() + v.y() * v.y() + v.z() * v.z());
}


#endif