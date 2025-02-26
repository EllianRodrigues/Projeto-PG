#include <iostream>
#include "src/ObjReader.cpp"
#include <fstream> 
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

vec3 color2(const ray& r, hitable *world) {
    hit_record rec; // Estrutura para armazenar as informações sobre o ponto de interseção
    if (world->hit(r, 0.0, FLT_MAX, rec)) { // Verifica se o raio atinge algum objeto no mundo
        // Se o raio atingir um objeto, a cor será baseada no vetor normal do ponto de interseção
        // return 0.5 * vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1); // Normalizada para a faixa [0, 1]
        // // return vec3(1.0, 0.0, 0.0); // Vermelho puro (RGB: 1.0, 0.0, 0.0)
        // if (rec.objeto == "esfera") {  // Use '==' para comparação
        //     return vec3(1.0, 0.0, 0.0); // Cor vermelha
        // }
        return rec.cor;
        return 0.5 * vec3(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1); // Normalizada para a faixa [0, 1]

    }
    else {
        // Se o raio não atingir nenhum objeto, gera uma cor de fundo (gradiente de azul)
        vec3 unit_direction = unit_vector(r.direction()); // Normaliza a direção do raio
        float t = 0.5 * (unit_direction.y() + 1.0); // Calcula um valor 't' baseado na direção Y
        // Retorna um gradiente entre branco (1.0, 1.0, 1.0) e azul claro (0.5, 0.7, 1.0)
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

// void rgpImage(){
//     // Dimensões da imagem (largura e altura)
//     int nx = 100;  // largura
//     int ny = 50;  // altura 
//     std::ofstream imageFile("rgb.ppm");
//     // Cabeçalho do arquivo PPM
//     imageFile << "P3\n" << nx << " " << ny << "\n255\n";
//     // Iteramos de cima para baixo na imagem (j começa em ny-1 e vai até 0)
//     for (int j = ny - 1; j >= 0; j--) {  
//         // Para cada linha (j), iteramos em cada coluna (i)
//         for (int i = 0; i < nx; i++) {
//             // Calcula os componentes de cor RGB como valores normalizados (entre 0 e 1)
//             float r = float(i) / float(nx);  // Variação de vermelho com base na posição horizontal
//             float g = float(j) / float(ny);  // Variação de verde com base na posição vertical
//             float b = 0.2;                   // Azul fixo em 20% de intensidade
//             // Converte os valores normalizados para o intervalo [0, 255]
//             int ir = int(255.99 * r);  // Multiplicamos por 255.99 para evitar arredondamentos baixos
//             int ig = int(255.99 * g);
//             int ib = int(255.99 * b);
//             imageFile << ir << " " << ig << " " << ib << "\n";
//         }
//     }
//     std::cerr << "\nFeito.\n"; 
//     imageFile.close(); 
// }


// void image() {
//     int nx = 200;  // Resolução horizontal
//     int ny = 100;  // Resolução vertical
//     // Parâmetros da câmera
//     vec3 lookfrom(0, 0, 0);       // Posição da câmera
//     vec3 lookat(0, 0, -1);        // Ponto onde a câmera está olhando
//     vec3 vup(0, 1, 0);            // Vetor "para cima"
//     double distance = 1.0;        // Distância entre câmera e tela
//     double screen_height = 2.0;   // Altura da tela
//     double screen_width = 4.0;    // Largura da tela
//     camera cam(lookfrom, lookat, vup, distance, screen_height, screen_width, nx);
//     std::ofstream imageFile("image.ppm");
//     imageFile << "P3\n" << nx << " " << ny << "\n255\n";
//     // Gerar pixels
//     for (int j = ny - 1; j >= 0; j--) {
//         for (int i = 0; i < nx; i++) {
//             double u = double(i) / double(nx);
//             double v = double(j) / double(ny);
//             ray r = cam.get_ray(u, v);
//             vec3 col = color(r);
//             int ir = int(255.99 * col[0]);
//             int ig = int(255.99 * col[1]);
//             int ib = int(255.99 * col[2]);
//             imageFile << ir << " " << ig << " " << ib << "\n";
//         }
//     }
//     imageFile.close();
// }

// void spheraAndPlane(){
//     int nx = 200;  // Número de colunas de pixels da imagem
//     int ny = 100;  // Número de linhas de pixels da imagem
//     std::ofstream imageFile("chao.ppm"); // Abre o arquivo para salvar a imagem no formato PPM
//     imageFile << "P3\n" << nx << " " << ny << "\n255\n"; // Cabeçalho do arquivo PPM: "P3" para cores RGB, tamanho da imagem, valor máximo de cor
//     // Parâmetros da câmera
//     vec3 lookfrom(0, 0, 0);       // Posição da câmera
//     vec3 lookat(0, 0, -1);        // Ponto onde a câmera está olhando
//     vec3 vup(0, 1, 0);            // Vetor "para cima" que define a orientação da câmera
//     double distance = 1.0;        // Distância entre a câmera e a tela (foco da lente)
//     double screen_height = 2.0;   // Altura da tela de visualização
//     double screen_width = 4.0;    // Largura da tela de visualização
//     camera cam(lookfrom, lookat, vup, distance, screen_height, screen_width, nx);
//     hitable *list[2];
//     list[0] = new sphere(vec3(0,0,-1), 0.5, vec3(1.0,0.0,0.0));
//     list[1] = new plane(vec3(0,  -0.5, 0), vec3(0, 1, 0), vec3(0, 0, 1));  // Plano 1 (cor cinza)
//     hitable *world = new hitable_list(list, 2);
//     for (int j = ny - 1; j >= 0; j--) { 
//         for (int i = 0; i < nx; i++) { 
//             float u = float(i) / float(nx);  
//             float v = float(j) / float(ny);
//             // Gera o raio para cada pixel, usando a câmera
//             ray r = cam.get_ray(u, v);
//             // Calcula a cor para o ponto onde o raio atinge
//             vec3 col = color2(r, world);
//             int ir = int(255.99 * col[0]); 
//             int ig = int(255.99 * col[1]); 
//             int ib = int(255.99 * col[2]); 
//             imageFile << ir << " " << ig << " " << ib << "\n";
//         }
//     }
//     std::cerr << "\nFeito.\n";
//     imageFile.close();
// }
vec3 color_with_shadows(const ray& r, hitable *world, const std::vector<light>& lights, const vec3& ambient_light) {
    hit_record rec;
    float shadow_attenuation = 0.5;
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
        }

        return color;
    } else {
        // Se não houver interseção, retorna a cor do fundo
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}


vec3 color_shadow(const ray& r, hitable *world, const std::vector<light>& lights, const vec3& ambient_light) {
    hit_record rec;
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

            // Iluminação Difusa (Luz incidente)
            float diff = std::max(dot(normal, light_dir), 0.0);
            vec3 diffuse = diff * l.intensity * rec.material.color;

            // Iluminação Especular
            vec3 view_dir = normalize(r.origin() - rec.p); // Direção do observador (raio)
            vec3 reflect_dir = reflect(-light_dir, normal); // Vetor refletido
            float spec = pow(std::max(dot(view_dir, reflect_dir), 0.0), rec.material.shininess);
            vec3 specular = spec * l.intensity * vec3(1.0, 1.0, 1.0); // Cor da luz especular (branca)

            // Acumulando a cor final
            color += diffuse + specular;
        }

        return color;
    }
    else {
        // Se não houver interseção, retorna a cor do fundo
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}



vec3 color5(const ray& r, hitable *world, const std::vector<light>& lights, const vec3& ambient_light) {
    hit_record rec;
    if (world->hit(r, 0.0, FLT_MAX, rec)) {
        vec3 color = vec3(0.0, 0.0, 0.0);

        // Componente de Iluminação Ambiente
        color += ambient_light * rec.material.ambiental * rec.material.color; // Luz ambiente multiplicada pelo coeficiente ambiental do material

        // Calculando a iluminação difusa e especular
        for (const auto& l : lights) {
            // Direção da luz
            vec3 light_dir = normalize(l.position - rec.p);
            // Normal no ponto de interseção
            vec3 normal = rec.normal;

            // Iluminação Difusa (Luz incidente)
            float diff = std::max(dot(normal, light_dir), 0.0);
            vec3 diffuse = diff * l.intensity * rec.material.difuso * rec.material.color; // Difusa com coeficiente difuso do material

            // Iluminação Especular
            vec3 view_dir = normalize(r.origin() - rec.p); // Direção do observador (raio)
            vec3 reflect_dir = reflect(-light_dir, normal); // Vetor refletido
            float spec = pow(std::max(dot(view_dir, reflect_dir), 0.0), rec.material.rugosidade);
            vec3 specular = spec * l.intensity * rec.material.especular * vec3(1.0, 1.0, 1.0); // Cor da luz especular (branca)

            // Acumulando a cor final
            color += diffuse + specular;
        }

        return color;
    }
    else {
        // Se não houver interseção, retorna a cor do fundo
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}



vec3 color4(const ray& r, hitable *world, const std::vector<light>& lights) {
    hit_record rec; // Estrutura para armazenar as informações sobre o ponto de interseção
    if (world->hit(r, 0.0, FLT_MAX, rec)) { // Verifica se o raio atinge algum objeto no mundo
        // Se o raio atinge um objeto, vamos calcular a cor do ponto de interseção
        vec3 color = rec.cor;  // Cor base do objeto (assumindo que o objeto tem uma cor própria)

        // Para cada luz na cena, calculamos a iluminação
        for (const auto& l : lights) {
            // Vetor que vai da superfície até a posição da luz
            vec3 light_dir = normalize(l.position - rec.p); // Posição da luz - Ponto de interseção

            // Produto escalar entre o vetor normal e a direção da luz (Lambertian)
            float diff = std::max(0.0f, static_cast<float>(dot(rec.normal, light_dir)));

            // Intensidade da luz na cor do objeto
            color += diff * l.intensity * l.color; // Modificar a cor do objeto pela intensidade da luz
        }

        // Retorna a cor final após iluminação
        return color;
    }
    else {
        // Se o raio não atingir nenhum objeto, gera uma cor de fundo (gradiente de azul)
        vec3 unit_direction = unit_vector(r.direction()); // Normaliza a direção do raio
        float t = 0.5 * (unit_direction.y() + 1.0); // Calcula um valor 't' baseado na direção Y
        // Retorna um gradiente entre branco (1.0, 1.0, 1.0) e azul claro (0.5, 0.7, 1.0)
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

    Material materialPersonalizado_amarelo(0.6f, 0.8f, 0.1f, 0.3f, 0.0f, 0.7f,1.0f, vec3(1.0, 1.0, 0.0));
    Material materialPersonalizado_vermelho(0.6f, 0.8f, 0.1f, 0.3f, 0.0f, 0.7f,1.0f, vec3(1.0, 0.0, 0.0));
    Material materialPersonalizado_verde(0.6f, 0.8f, 0.1f, 0.3f, 0.0f, 0.7f,1.0f, vec3(0.0, 1.0, 0.0));
    Material materialPersonalizado_azul(0.6f, 0.8f, 0.1f, 0.3f, 0.0f, 0.7f, 1.0f, vec3(0.0, 0.0, 1.0));
    // Criando fontes de luz
    light l1(vec3(5, 5, 5), vec3(1.0, 1.0, 1.0), 1.0); // Luz branca
    light l2(vec3(-5, 5, 5), vec3(1.0, 0.0, 0.0), 0.0); // Luz vermelha
    std::vector<light> lights = {l1, l2}; // Lista de luzes
    // Planos
    list[0] = new plane(vec3(0, -0.5, 0), vec3(0, 1, 0), vec3(1.0, 1.0, 0.0), materialPersonalizado_amarelo);  // Plano (chão) amarelo

    // Esferas com cores e posições específicas
    list[1] = new sphere(vec3(0, -0.5, 3.3), 0.6, vec3(1.0, 0.0, 0.0), materialPersonalizado_vermelho);  // Esfera vermelha (centro)
    list[2] = new sphere(vec3(-1.3, 0.2, 2.3), 0.6, vec3(0.0, 1.0, 0.0), materialPersonalizado_verde); // Esfera verde (esquerda)
    list[3] = new sphere(vec3(1.3, 0.2, 2.3), 0.6, vec3(0.0, 0.0, 1.0), materialPersonalizado_azul);  // Esfera azul (direita)
  

    hitable *world = new hitable_list(list, 4);  // Agora temos 9 objetos (planos e esferas)

    // Laço para gerar os pixels da imagem
    for (int j = ny - 1; j >= 0; j--) { 
        for (int i = 0; i < nx; i++) { 
            float u = float(i) / float(nx);  
            float v = float(j) / float(ny);
            // Gera o raio para cada pixel, usando a câmera
            ray r = cam.get_ray(u, v);
            // Calcula a cor para o ponto onde o raio atinge
            vec3 col = color_with_shadows(r, world, lights,  vec3(0.0, 0.0, 0.0));
            int ir = int(255.99 * col[0]); 
            int ig = int(255.99 * col[1]); 
            int ib = int(255.99 * col[2]); 
            imageFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    std::cerr << "\nFeito.\n";
    imageFile.close();
}

// void triangulo(){

//     int nx = 500;  // Número de colunas de pixels da imagem
//     int ny = 500;  // Número de linhas de pixels da imagem
//     std::ofstream imageFile("desafioTriangulos.ppm"); // Abre o arquivo para salvar a imagem no formato PPM
//     imageFile << "P3\n" << nx << " " << ny << "\n255\n"; // Cabeçalho do arquivo PPM: "P3" para cores RGB, tamanho da imagem, valor máximo de cor
    
//     // Parâmetros da câmera
//     vec3 lookfrom(0, 0, 5);       // Posição da câmera
//     vec3 lookat(0, 0, 0);         // Ponto onde a câmera está olhando
//     vec3 vup(0, 1, 0);            // Vetor "para cima" que define a orientação da câmera
//     double distance = 1.0;        // Distância entre a câmera e a tela (foco da lente)
//     double screen_height = 2.0;   // Altura da tela de visualização
//     double screen_width = 2.0;    // Largura da tela de visualização
//     camera cam(lookfrom, lookat, vup, distance, screen_height, screen_width, nx);
    
//     // Criando triângulos
//     hitable *list[3];  // Suponhamos que estamos usando 3 triângulos
    
//     // Triângulos
//     list[0] = new triangle(vec3(0, 3, -2), vec3(1, 1, -2), vec3(-1, 1, -2), vec3(0.0, 1.0, 0.0)); // Triângulo 1 (cor Verde)
//     list[1] = new triangle(vec3(1, -1, -3), vec3(2, 1, -3), vec3(0, 1, -3), vec3(1.0, 0.0, 0.0)); // Triângulo 2 (cor Vermelho)
//     list[2] = new triangle(vec3(-1, -1, -4), vec3(0, 1, -4), vec3(-2, 1, -4), vec3(0.0, 0.0, 1.0)); // Triângulo 3 (cor Azul)

//     hitable *world = new hitable_list(list, 3);  // Agora temos 3 triângulos no mundo

//     // Laço para gerar os pixels da imagem
//     for (int j = ny - 1; j >= 0; j--) { 
//         for (int i = 0; i < nx; i++) { 
//             float u = float(i) / float(nx);  
//             float v = float(j) / float(ny);
//             // Gera o raio para cada pixel, usando a câmera
//             ray r = cam.get_ray(u, v);
//             // Calcula a cor para o ponto onde o raio atinge
//             vec3 col = color2(r, world);
//             int ir = int(255.99 * col[0]); 
//             int ig = int(255.99 * col[1]); 
//             int ib = int(255.99 * col[2]); 
//             imageFile << ir << " " << ig << " " << ib << "\n";
//         }
//     }

//     std::cerr << "\nFeito.\n";
//     imageFile.close();
// }



vec3 color3(const ray& r, hitable *world, const std::vector<light>& lights, const vec3& ambiente) {
    hit_record rec;
    if (world->hit(r, 0.0, FLT_MAX, rec)) {
        vec3 col(0.0, 0.0, 0.0);  // Cor inicial

        // Luz ambiente: adicione uma pequena quantidade de luz ambiente ao final
        col += ambiente;  // A cor ambiente é aplicada a toda a cena

        // Iluminação direta: cálculo da contribuição das luzes
        for (const auto& l : lights) {
            // Direção da luz e intensidade
            vec3 light_dir = normalize(l.position - rec.p);
            float diff = std::max(0.0, dot(rec.normal, light_dir)); // Luz difusa
            col += diff * l.intensity * l.color;  // Adiciona a cor da luz ponderada pela intensidade e difusa
        }

        return col;
    }
    else {
        // Se o raio não atingir nenhum objeto, cor de fundo com gradiente
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
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
            vec3 col = color_with_shadows(r, world, lights, vec3(0.5, 0.0, 0.0));
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



// void transformacoes(){
//     int nx = 500;  // Número de colunas de pixels da imagem
//     int ny = 500;  // Número de linhas de pixels da imagem
//     std::ofstream imageFile("transform_rotation.ppm"); // Abre o arquivo para salvar a imagem no formato PPM
//     imageFile << "P3\n" << nx << " " << ny << "\n255\n"; // Cabeçalho do arquivo PPM: "P3" para cores RGB, tamanho da imagem, valor máximo de cor
    
//     // Parâmetros da câmera
//     vec3 lookfrom(0, 0, 5);       // Posição da câmera
//     vec3 lookat(0, 0, 0);         // Ponto onde a câmera está olhando
//     vec3 vup(0, 1, 0);            // Vetor "para cima" que define a orientação da câmera
//     double distance = 1.0;        // Distância entre a câmera e a tela (foco da lente)
//     double screen_height = 2.0;   // Altura da tela de visualização
//     double screen_width = 2.0;    // Largura da tela de visualização
//     camera cam(lookfrom, lookat, vup, distance, screen_height, screen_width, nx);
    
//     hitable *list[2];  // Agora temos apenas dois objetos: um plano e uma esfera
    
//     // Definir o plano com nova posição e normal
//     vec3 plane_position(0, -1, 0);  // Posição do plano
//     vec3 plane_normal(0, 1, 0);    // Normal do plano (plano horizontal)
//     vec3 cor(0.0, 0.0, 1.0);  // Vetor "para cima" para o plano
//     list[0] = new plane(plane_position, plane_normal, cor);  // Plano (cor Verde) 
//     // Definir a esfera (para manter a estrutura)
//     list[1] = new sphere(vec3(0, 1, -2), 1.0, vec3(1.0, 0.0, 0.0));  // Esfera vermelha

    
//     // Aqui aplicamos a rotação ao plano
//     double angle = 45.0 * (M_PI / 180.0); // Converte para radianos
//     mat4 rotation = rotation_matrix_z(angle); // Matriz de rotação
    
//     // Aplicar rotação à posição e à normal do plano
//     plane_position = rotation * plane_position;
//     plane_normal = rotation * plane_normal;  // Rotacionar a normal do plano

//     // Atualiza a posição e a normal do plano no mundo
//     list[0] = new plane(plane_position, plane_normal, cor);  // Atualiza plano com a nova posição e normal

//     // Definir o mundo com o plano e a esfera
//     hitable *world = new hitable_list(list, 2);


//     // Laço para gerar os pixels da imagem
//     for (int j = ny - 1; j >= 0; j--) { 
//         for (int i = 0; i < nx; i++) { 
//             float u = float(i) / float(nx);  
//             float v = float(j) / float(ny);
//             // Gera o raio para cada pixel, usando a câmera
//             ray r = cam.get_ray(u, v);
//             // Calcula a cor para o ponto onde o raio atinge
//             vec3 col = color2(r, world);
//             int ir = int(255.99 * col[0]); 
//             int ig = int(255.99 * col[1]); 
//             int ib = int(255.99 * col[2]); 
//             imageFile << ir << " " << ig << " " << ib << "\n";
//         }
//     }

//     std::cerr << "\nFeito.\n";
//     imageFile.close();
// }



// void testar_translacao() {
//     int nx = 500, ny = 500;
//     std::ofstream imageFile("translacao.ppm");
//     imageFile << "P3\n" << nx << " " << ny << "\n255\n";

//     // Parâmetros da câmera
//     vec3 lookfrom(0, 0, 5);  // Posição da câmera
//     vec3 lookat(0, 0, 0);    // Ponto para onde a câmera está olhando
//     vec3 vup(0, 1, 0);       // Vetor para "cima"
//     double distance = 1.0;   // Distância entre a câmera e a tela
//     double screen_height = 2.0;
//     double screen_width = 2.0;
//     camera cam(lookfrom, lookat, vup, distance, screen_height, screen_width, nx);

//     // Criar o plano e a esfera
//     hitable *list[2];

//     // Definir a posição e a normal do plano
//     vec3 plane_position(0, -1, 0);  // Posição inicial do plano
//     vec3 plane_normal(0, 1, 0);     // Normal do plano (não deve ser afetada pela translação)

//     // Definir a translação (por exemplo, mover o plano 1 unidade no eixo X e 1 unidade no eixo Y)
//     vec3 translation_vector(2.0, 3.0, 0.0);  
//     mat4 translation = translation_matrix(translation_vector);

//     // Aplicar a translação à posição do plano
//     plane_position = translation * plane_position;
//     // plane_normal = translation * plane_normal;
//     // Criar o plano com a nova posição (e normal não alterada)
//     list[0] = new plane(plane_position, plane_normal, vec3(0.0, 0.0, 1.0));  // Cor do plano (verde)

//     // Definir a esfera (para manter a estrutura)
//     list[1] = new sphere(vec3(0, 1, -2), 1.0, vec3(1.0, 0.0, 0.0));  // Esfera vermelha

//     // Criar o mundo com os objetos
//     hitable *world = new hitable_list(list, 2);

   

//     // Laço para gerar os pixels da imagem
//     for (int j = ny - 1; j >= 0; j--) {
//         for (int i = 0; i < nx; i++) {
//             float u = float(i) / float(nx);
//             float v = float(j) / float(ny);
//             ray r = cam.get_ray(u, v);
//             vec3 col = color2(r, world);
//             int ir = int(255.99 * col[0]);
//             int ig = int(255.99 * col[1]);
//             int ib = int(255.99 * col[2]);
//             imageFile << ir << " " << ig << " " << ib << "\n";
//         }
//     }

//     std::cerr << "\nFeito.\n";
//     imageFile.close();
// }



// void escalonamento(){


//     int nx = 500;
//     int ny = 500;
//     std::ofstream imageFile("escaling.ppm");
//     imageFile << "P3\n" << nx << " " << ny << "\n255\n";

//     // Definir os parâmetros da câmera
//     vec3 lookfrom(0, 0, 5);
//     vec3 lookat(0, 0, 0);
//     vec3 vup(0, 1, 0);
//     double distance = 1.0;
//     double screen_height = 2.0;
//     double screen_width = 2.0;
//     camera cam(lookfrom, lookat, vup, distance, screen_height, screen_width, nx);

//     // Definir o plano e a esfera
//     hitable *list[2];
//     list[0] = new plane(vec3(0, -1, 0), vec3(0, 1, 0), vec3(0.0, 0.0, 1.0));  // Plano
//     list[1] = new sphere(vec3(0, 1, -2), 1.0, vec3(1.0, 0.0, 0.0));  // Esfera vermelha

//     double scale_factor = 2.0;
//     mat4 scale_matrix = scaling_matrix(scale_factor);

//     // Acessar a esfera e escalonar
//     sphere* s = dynamic_cast<sphere*>(list[1]);  // Garantindo que é uma esfera
//     if (s) {
//         // Escalonando o centro da esfera corretamente
//         s->center = vec3(
//             scale_factor * s->center.x(),
//             scale_factor * s->center.y(),
//             scale_factor * s->center.z()
//         );

//         // Escalonando o raio
//         s->radius *= scale_factor;
//     }


//     hitable *world = new hitable_list(list, 2);

//     // Laço para gerar os pixels da imagem
//     for (int j = ny - 1; j >= 0; j--) {
//         for (int i = 0; i < nx; i++) {
//             float u = float(i) / float(nx);
//             float v = float(j) / float(ny);
//             ray r = cam.get_ray(u, v);
//             vec3 col = color2(r, world);
//             int ir = int(255.99 * col[0]);
//             int ig = int(255.99 * col[1]);
//             int ib = int(255.99 * col[2]);
//             imageFile << ir << " " << ig << " " << ib << "\n";
//         }
//     }

//     std::cerr << "\nFeito.\n";
//     imageFile.close();

// }

int main(){

    objReader obj("inputs/untitled.obj");
    obj.print_faces();
   
    
    // Obtém os pontos das faces
    std::vector<std::vector<point>> faces = obj.getFacePoints();

    // Lista para armazenar os triângulos
    std::vector<triangle*> triangles;

    

    mat4 rotation = rotation_matrix_y(M_PI / 6);  
    mat4 scale = scaling_matrix(0.8);
    // Itera sobre cada face
    for (const auto& face : faces) {
        if (face.size() >= 3) { 
            vec3 randomColor(
                static_cast<float>(rand()) / RAND_MAX, // Valor aleatório entre 0 e 1
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX
            );
            Material materialPersonalizado(0.6f, 0.8f, 0.1f, 0.3f, 0.0f, 0.7f, 1.0f ,vec3(0.0, 1.0, 0.0));
            // Aplica a rotação no eixo X a cada vértice
            vec3 v1_rotated = rotation * vec3(face[0].x, face[0].y, face[0].z);
            vec3 v2_rotated = rotation * vec3(face[1].x, face[1].y, face[1].z);
            vec3 v3_rotated = rotation * vec3(face[2].x, face[2].y, face[2].z);

            triangles.push_back(new triangle(
                // vec3(face[0].x, face[0].y, face[0].z),
                // vec3(face[1].x, face[1].y, face[1].z),
                // vec3(face[2].x, face[2].y, face[2].z),
                v1_rotated*2,
                v2_rotated*2,
                v3_rotated*2,
                randomColor,
                materialPersonalizado
            ));
        }
    }

    // Verificação (exemplo)
    std::cout << "Total de triângulos armazenados: " << triangles.size() << std::endl;

    triangulo_auto(triangles); 

    // desafio1();

    return 0;
}