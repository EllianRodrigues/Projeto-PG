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

void rgpImage(){
    // Dimensões da imagem (largura e altura)
    int nx = 100;  // largura
    int ny = 50;  // altura 
    std::ofstream imageFile("rgb.ppm");
    // Cabeçalho do arquivo PPM
    imageFile << "P3\n" << nx << " " << ny << "\n255\n";
    // Iteramos de cima para baixo na imagem (j começa em ny-1 e vai até 0)
    for (int j = ny - 1; j >= 0; j--) {  
        // Para cada linha (j), iteramos em cada coluna (i)
        for (int i = 0; i < nx; i++) {
            // Calcula os componentes de cor RGB como valores normalizados (entre 0 e 1)
            float r = float(i) / float(nx);  // Variação de vermelho com base na posição horizontal
            float g = float(j) / float(ny);  // Variação de verde com base na posição vertical
            float b = 0.2;                   // Azul fixo em 20% de intensidade
            // Converte os valores normalizados para o intervalo [0, 255]
            int ir = int(255.99 * r);  // Multiplicamos por 255.99 para evitar arredondamentos baixos
            int ig = int(255.99 * g);
            int ib = int(255.99 * b);
            imageFile << ir << " " << ig << " " << ib << "\n";
        }
    }
    std::cerr << "\nFeito.\n"; 
    imageFile.close(); 
}


void image() {
    int nx = 200;  // Resolução horizontal
    int ny = 100;  // Resolução vertical
    // Parâmetros da câmera
    vec3 lookfrom(0, 0, 0);       // Posição da câmera
    vec3 lookat(0, 0, -1);        // Ponto onde a câmera está olhando
    vec3 vup(0, 1, 0);            // Vetor "para cima"
    double distance = 1.0;        // Distância entre câmera e tela
    double screen_height = 2.0;   // Altura da tela
    double screen_width = 4.0;    // Largura da tela
    camera cam(lookfrom, lookat, vup, distance, screen_height, screen_width, nx);
    std::ofstream imageFile("image.ppm");
    imageFile << "P3\n" << nx << " " << ny << "\n255\n";
    // Gerar pixels
    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            double u = double(i) / double(nx);
            double v = double(j) / double(ny);
            ray r = cam.get_ray(u, v);
            vec3 col = color(r);
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            imageFile << ir << " " << ig << " " << ib << "\n";
        }
    }
    imageFile.close();
}

void spheraAndPlane(){
     int nx = 200;  // Número de colunas de pixels da imagem
    int ny = 100;  // Número de linhas de pixels da imagem
    std::ofstream imageFile("chao.ppm"); // Abre o arquivo para salvar a imagem no formato PPM
    imageFile << "P3\n" << nx << " " << ny << "\n255\n"; // Cabeçalho do arquivo PPM: "P3" para cores RGB, tamanho da imagem, valor máximo de cor
    // Parâmetros da câmera
    vec3 lookfrom(0, 0, 0);       // Posição da câmera
    vec3 lookat(0, 0, -1);        // Ponto onde a câmera está olhando
    vec3 vup(0, 1, 0);            // Vetor "para cima" que define a orientação da câmera
    double distance = 1.0;        // Distância entre a câmera e a tela (foco da lente)
    double screen_height = 2.0;   // Altura da tela de visualização
    double screen_width = 4.0;    // Largura da tela de visualização
    camera cam(lookfrom, lookat, vup, distance, screen_height, screen_width, nx);
    hitable *list[2];
    list[0] = new sphere(vec3(0,0,-1), 0.5);
    list[1] = new plane(vec3(0, -0.5, 0), vec3(0, 1, 0));
    hitable *world = new hitable_list(list, 2);
    for (int j = ny - 1; j >= 0; j--) { 
        for (int i = 0; i < nx; i++) { 
            float u = float(i) / float(nx);  
            float v = float(j) / float(ny);
            // Gera o raio para cada pixel, usando a câmera
            ray r = cam.get_ray(u, v);
            // Calcula a cor para o ponto onde o raio atinge
            vec3 col = color2(r, world);
            int ir = int(255.99 * col[0]); 
            int ig = int(255.99 * col[1]); 
            int ib = int(255.99 * col[2]); 
            imageFile << ir << " " << ig << " " << ib << "\n";
        }
    }
    std::cerr << "\nFeito.\n";
    imageFile.close();
}

int main(){

    objReader obj("inputs/cubo.obj");
    obj.print_faces();
   
    // Gerar imagem RGB em PPM
    rgpImage();
    // Camera com fundo e uma esfera
    image();

    // esfera e plano
    spheraAndPlane();
   
    return 0;
}