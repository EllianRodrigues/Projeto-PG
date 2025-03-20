#include <iostream>
#include "src/ObjReader.cpp"
#include <fstream> 
#include <cmath>
#include <algorithm>
#include "vec3.h"
#include "ray.h"
#include "hitablelist.h"
#include "sphereh.h"
#include "float.h"
#include <cfloat>
#include "plane.h"
#include "camera.h"
#include "material.h"
#include "light.h"  // Incluindo o arquivo de cabeçalho da classe light
#include "triangle.h"  // Inclui a classe triangle


float hit_sphere(const vec3& center, float radius, const ray& r) {
    vec3 oc = r.origin() - center;   // Vetor de origem do raio ao centro da esfera
    float a = dot(r.direction(), r.direction());  // a = |direção do raio|^2
    float b = 2.0 * dot(oc, r.direction());      // b = 2 * (oc . direção do raio)
    float c = dot(oc, oc) - radius * radius;     // c = |oc|^2 - raio^2
    float discriminant = b * b - 4 * a * c;      // Discriminante da equação quadrática
    if (discriminant < 0)  // Se o discriminante for negativo, não há interseção
        return -1.0;
    else
        return (-b - sqrt(discriminant)) / (2.0 * a);  // Retorna a solução t mais próxima
}


vec3 color(const ray& r) {
    float t = hit_sphere(vec3(0, 0, -1), 0.5, r);  // Verifica se o raio atinge a esfera
    if (t > 0.0) {  // Se houver interseção
        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));  // Calcula a normal na interseção
        return 0.5 * vec3(N.x() + 1, N.y() + 1, N.z() + 1);  // Mapeia a normal para a cor
    }
    vec3 unit_direct = unit_vector(r.direction());  // Normaliza a direção do raio
    t = 0.5 * (unit_direct.y() + 1.0);  // Mapeia o valor y para o intervalo [0, 1]
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);  // Cor de fundo
}


vec3 refract(const vec3& d, const vec3& n, float rf) {
    float cos_theta = std::min(dot(-d, n), 1.0);
    float sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    
    // Verificar reflexão total interna
    bool total_reflection = rf * sin_theta > 1.0f;
    if (total_reflection) {
        return vec3(0.0, 0.0, 0.0); // Não há refração, apenas reflexão total
    }

    // Índice de refração: ior é o índice do material do ponto, 1.0 é o do ar
    float r0 = (1.0f - rf) / (1.0 + rf);
    r0 = r0 * r0;
    float refractive_ratio = 1.0 / rf;

    // Refração com a fórmula de Snell
    float cos_theta_t = sqrt(1.0 - sin_theta * sin_theta);
    vec3 refracted = refractive_ratio * d + (refractive_ratio * cos_theta - cos_theta_t) * n;

    return refracted;
}



vec3 color_with_ref2(const ray& r, hitable *world, const std::vector<light>& lights, const vec3& ambient_light, int depth) {
    hit_record rec;
    float shadow_attenuation = 0.5;

    if (depth <= 0) return vec3(0.0, 0.0, 0.0);

    if (world->hit(r, 0.0, FLT_MAX, rec)) {
        vec3 color = vec3(0.0, 0.0, 0.0);

        // Componente de Iluminação Ambiente
        color += ambient_light * rec.material.color; // A cor do material é multiplicada pela luz ambiente

        // Calculando a iluminação difusa e especular
        for (const auto& l : lights) {
            // Direção da luz
            vec3 light_dir = normalize(l.position - rec.p);
            // Normal no ponto de interseção
            vec3 normal = rec.normal;

            // Verificando se o ponto está em sombra
            // Criando um raio de sombra a partir do ponto de interseção
            vec3 shadow_origin = rec.p + normal * 0.001; // Um pequeno deslocamento para evitar que o raio se cruze com o ponto de interseção
            ray shadow_ray(shadow_origin, light_dir);

            // Verifica se o raio de sombra atinge outro objeto
            hit_record shadow_rec;
            float dist = sqrt(dot(l.position - rec.p, l.position - rec.p));
            bool in_shadow = world->hit(shadow_ray, 0.001, dist - 0.001, shadow_rec);

            // Se não estiver em sombra, calcula a iluminação difusa
            if (!in_shadow) {
                // Iluminação Difusa
                float diff = std::max(dot(normal, light_dir), 0.0);
                vec3 diffuse = diff * l.intensity * rec.material.color ;
                color += diffuse;
            } else {
                // Se estiver em sombra, aplica o fator de transparência à sombra
                float diff = std::max(dot(normal, light_dir), 0.0);
                vec3 diffuse = diff * l.intensity * rec.material.color * shadow_attenuation;
                color += diffuse; // Adiciona a sombra atenuada
            }

            // Reflexão (se o material tiver reflexão)
            if (rec.material.especular > 0.0f) {
                vec3 reflected_dir = reflect(r.direction(), rec.normal);
                ray reflected_ray(rec.p, reflected_dir);
                vec3 reflected_color = color_with_ref2(reflected_ray, world, lights, ambient_light, depth - 1);
                color += rec.material.especular * reflected_color;
            }

            // Refração (se o material tiver refração)
            if (rec.material.refracao > 1.0f) {
                vec3 refracted_dir = refract(r.direction(), rec.normal, rec.material.refracao);
                ray refracted_ray(rec.p, refracted_dir);
                vec3 refracted_color = color_with_ref2(refracted_ray, world, lights, ambient_light, depth - 1);
                color += rec.material.especular * refracted_color;
            }
        }

        return color;
    } else {
        // Se não houver interseção, retorna a cor do fundo
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
    
}



void desafio1() {
    int nx = 500;  // Número de colunas de pixels da imagem
    int ny = 500;  // Número de linhas de pixels da imagem
    std::ofstream imageFile("desafio_sombra.ppm"); // Abre o arquivo para salvar a imagem no formato PPM
    imageFile << "P3\n" << nx << " " << ny << "\n255\n"; // Cabeçalho do arquivo PPM: "P3" para cores RGB, tamanho da imagem, valor máximo de cor
    
    // Parâmetros da câmera
    vec3 lookfrom(0, 0, 5);       // Posição da câmera
    vec3 lookat(0, 0, 0);         // Ponto onde a câmera está olhando
    vec3 vup(0, 1, 0);            // Vetor "para cima" que define a orientação da câmera
    double distance = 1.0;        // Distância entre a câmera e a tela (foco da lente)
    double screen_height = 1.0;   // Altura da tela de visualização
    double screen_width = 1.0;    // Largura da tela de visualização
    camera cam(lookfrom, lookat, vup, distance, screen_height, screen_width, nx);
    
    // Criando planos (posição, vetor normal)
    hitable *list[4];

    Material material_amarelo(0.6f, 0.8f, 0.1f, 0.3f, 0.0f, 0.7f,1.0f, vec3(1.0, 1.0, 0.0));
    Material material_verde(0.6f, 0.8f, 0.1f, 0.3f, 0.0f, 0.7f,1.0f, vec3(0.0, 1.0, 0.0));
    Material material_reflexivo(0.6f, 0.8f, 0.1f, 0.3f, 0.0f, 0.7f, 1.5f, vec3(1.0, 1.0, 1.0));
    Material material_transparente(0.6f, 0.8f, 0.1f, 0.3f, 0.0f, 0.7f, 1.33f, vec3(0.5, 0.5, 0.5));
    // Criando fontes de luz
    light l1(vec3(5, 5, 5), vec3(1.0, 1.0, 1.0), 1.0); // Luz branca
    std::vector<light> lights = {l1}; // Lista de luzes
    // Planos
    list[0] = new plane(vec3(0, -0.5, 0), vec3(0, 1, 0), vec3(1.0, 1.0, 0.0), material_amarelo);  // Plano (chão) amarelo

    // Esferas com cores e posições específicas
    list[1] = new sphere(vec3(0, -0.5, 3.3), 0.6, vec3(1.0, 0.0, 0.0), material_transparente);  // Esfera vermelha (centro)
    list[2] = new sphere(vec3(-1.3, 0.2, 2.3), 0.6, vec3(0.0, 1.0, 0.0), material_verde); // Esfera verde (esquerda)
    list[3] = new sphere(vec3(1.3, 0.2, 2.3), 0.6, vec3(0.0, 0.0, 1.0), material_reflexivo);  // Esfera azul (direita)
  

    hitable *world = new hitable_list(list, 4);  // Agora temos 9 objetos (planos e esferas)

    // Laço para gerar os pixels da imagem
    for (int j = ny - 1; j >= 0; j--) { 
        for (int i = 0; i < nx; i++) { 
            float u = float(i) / float(nx);  
            float v = float(j) / float(ny);
            // Gera o raio para cada pixel, usando a câmera
            ray r = cam.get_ray(u, v);
            // Calcula a cor para o ponto onde o raio atinge
            vec3 col = color_with_ref2(r, world, lights,  vec3(0.0, 0.0, 0.0), 3);
            int ir = int(255.99 * col[0]); 
            int ig = int(255.99 * col[1]); 
            int ib = int(255.99 * col[2]); 
            imageFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    std::cerr << "\nFeito.\n";
    imageFile.close();
}
    

void triangulo_auto(const std::vector<triangle*>& triangles){

    int nx = 500;  // Número de colunas de pixels da imagem
    int ny = 500;  // Número de linhas de pixels da imagem
    std::ofstream imageFile("triangulo_auto_transformation.ppm"); // Abre o arquivo para salvar a imagem no formato PPM
    imageFile << "P3\n" << nx << " " << ny << "\n255\n"; // Cabeçalho do arquivo PPM: "P3" para cores RGB, tamanho da imagem, valor máximo de cor
    
    // Parâmetros da câmera
    vec3 lookfrom(0, 0, 5);       // Posição da câmera
    vec3 lookat(0, 0, 0);         // Ponto onde a câmera está olhando
    vec3 vup(0, 1, 0);            // Vetor "para cima" que define a orientação da câmera
    double distance = 1.0;        // Distância entre a câmera e a tela (foco da lente)
    double screen_height = 2.0;   // Altura da tela de visualização
    double screen_width = 2.0;    // Largura da tela de visualização
    camera cam(lookfrom, lookat, vup, distance, screen_height, screen_width, nx);
    
    // Criando triângulos
    // Criando o mundo de objetos
   // Aloca dinamicamente o array para os objetos hitable (triângulos)
    hitable** list = new hitable*[triangles.size() + 1];  // Aloca espaço para os triângulos + 1 (para o plano)

    // Copia os triângulos para o array
    for (size_t i = 0; i < triangles.size(); i++) {
        list[i] = triangles[i];
    }

    // Cria o material para o plano
    Material materialPersonalizado_amarelo(0.6f, 0.8f, 0.1f, 0.3f, 0.0f, 0.7f, 1.0f, vec3(1.0, 1.0, 0.0));

    // Cria o plano e coloca no índice correto do array
    list[triangles.size()] = new plane(vec3(0, -1.1, 0), vec3(0, 1, 0), vec3(1.0, 1.0, 0.0), materialPersonalizado_amarelo);

    // Cria o objeto hitable_list com o tamanho correto (triângulos + 1 plano)
    hitable *world = new hitable_list(list, triangles.size() + 1);

    // Criando fontes de luz
    light l1(vec3(5, 5, 5), vec3(1.0, 1.0, 1.0), 1.0); // Luz branca
    light l2(vec3(-5, 5, 5), vec3(1.0, 0.0, 0.0), 0.0); // Luz vermelha
    std::vector<light> lights = {l1, l2}; // Lista de luzes


    // Laço para gerar os pixels da imagem
    for (int j = ny - 1; j >= 0; j--) { 
        for (int i = 0; i < nx; i++) { 
            float u = float(i) / float(nx);  
            float v = float(j) / float(ny);
            // Gera o raio para cada pixel, usando a câmera
            ray r = cam.get_ray(u, v);
            // Calcula a cor para o ponto onde o raio atinge
            vec3 col = color_with_ref2(r, world, lights, vec3(0.5, 0.0, 0.0), 3);
            int ir = int(255.99 * col[0]); 
            int ig = int(255.99 * col[1]); 
            int ib = int(255.99 * col[2]); 
            imageFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    std::cerr << "\nFeito.\n";
    imageFile.close();

}


#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif


int main(){

    // objReader obj("inputs/cubo.obj");
    // obj.print_faces();
   
    
    // // Obtém os pontos das faces
    // std::vector<std::vector<point>> faces = obj.getFacePoints();

    // // Lista para armazenar os triângulos
    // std::vector<triangle*> triangles;

    

    // mat4 rotation = rotation_matrix_y(M_PI / 6);  
    // mat4 scale = scaling_matrix(0.8);
    // // Itera sobre cada face
    // for (const auto& face : faces) {
    //     if (face.size() >= 3) { 
    //         vec3 randomColor(
    //             static_cast<float>(rand()) / RAND_MAX, // Valor aleatório entre 0 e 1
    //             static_cast<float>(rand()) / RAND_MAX,
    //             static_cast<float>(rand()) / RAND_MAX
    //         );
    //         Material materialPersonalizado(0.6f, 0.8f, 0.1f, 0.3f, 0.0f, 0.7f, 1.0f ,vec3(0.0, 1.0, 0.0));
    //         // Aplica a rotação no eixo X a cada vértice
    //         vec3 v1_rotated = rotation * vec3(face[0].x, face[0].y, face[0].z);
    //         vec3 v2_rotated = rotation * vec3(face[1].x, face[1].y, face[1].z);
    //         vec3 v3_rotated = rotation * vec3(face[2].x, face[2].y, face[2].z);

    //         triangles.push_back(new triangle(
    //             // vec3(face[0].x, face[0].y, face[0].z),
    //             // vec3(face[1].x, face[1].y, face[1].z),
    //             // vec3(face[2].x, face[2].y, face[2].z),
    //             v1_rotated*2,
    //             v2_rotated*2,
    //             v3_rotated*2,
    //             randomColor,
    //             materialPersonalizado
    //         ));
    //     }
    // }

    // std::cout << "Total de triângulos armazenados: " << triangles.size() << std::endl;

    // triangulo_auto(triangles); 

    desafio1();

    return 0;
}