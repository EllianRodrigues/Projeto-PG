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


vec3 color_with_shadowsteste(const ray& r, hitable *world, const std::vector<light>& lights, vec3 ambient_light, int depth = 4) {
    if (depth <= 0) return vec3(0, 0, 0);  // Evita chamadas infinitas

    hit_record rec;
    if (world->hit(r, 0.001f, FLT_MAX, rec)) {
        vec3 final_color(0, 0, 0);
        vec3 normal = rec.normal;
        vec3 view_dir = normalize(-r.direction());  // Direção do observador
        Material material = rec.material;

        // Reflexão e refração com Fresnel
        float fresnel = pow(1.0f - fabs(dot(view_dir, normal)), 5.0f);  // Calculo de Fresnel
        float reflectance = material.reflexao + (1.0f - material.reflexao) * fresnel;

        vec3 reflected_color(0, 0, 0);
        vec3 refracted_color(0, 0, 0);

        // Reflexão
        if (material.reflexao > 0.0f) {
            vec3 reflected_dir = reflect(r.direction(), normal);
            ray reflected_ray(rec.p + reflected_dir * 0.001f, reflected_dir);
            reflected_color = color_with_shadowsteste(reflected_ray, world, lights, ambient_light, depth - 1);
        }

        // Refração
        if (material.transmissao > 0.0f) {
            vec3 refracted_dir;
            float ni_over_nt = (dot(view_dir, normal) > 0) ? material.ior : (1.0f / material.ior);
        

            vec3 outward_normal;
            if (dot(r.direction(), rec.normal) > 0) {
                // O raio está saindo da esfera
                outward_normal = -rec.normal;
                ni_over_nt = material.ior; // Índice de refração do material
            } else {
                // O raio está entrando na esfera
                outward_normal = rec.normal;
                ni_over_nt = 1.0f / material.ior; // Índice de refração do ar para o material
            }
            if (refract(r.direction(), outward_normal, ni_over_nt, refracted_dir)) {
                ray refracted_ray(rec.p + refracted_dir * 0.000000000000001f, refracted_dir);
                refracted_color = color_with_shadowsteste(refracted_ray, world, lights, ambient_light, depth - 1);
            } else {
                // Reflexão total interna
                refracted_color = vec3(0, 0, 0);
            }
        }

        // Mistura entre reflexão e refração (transparente)
        final_color += material.transmissao * refracted_color + material.reflexao * reflected_color;

        // Iluminação direta (modelo de Phong)
        for (const auto& light : lights) {
            vec3 light_dir = normalize(light.position - rec.p);
            vec3 half_vector = normalize(light_dir + view_dir);
            float diff = std::max(float(dot(normal, light_dir)), 0.0f);
            float spec = pow(std::max(float(dot(normal, half_vector)), 0.0f), material.shininess);
            
            vec3 diffuse = material.color * diff * material.difuso * light.color;
            vec3 specular = vec3(1.0, 1.0, 1.0) * spec * material.especular * light.color;
            
            final_color += diffuse + specular;
        }

        // Adiciona luz ambiente
        final_color += ambient_light * material.color * material.ambiental;

        return final_color;
    }
    return vec3(0, 0, 0);
}



#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif



void bezier(const std::vector<triangle*>& triangles) {
    int nx = 500;  // Número de colunas de pixels da imagem
    int ny = 500;  // Número de linhas de pixels da imagem
    std::ofstream imageFile("bezier.ppm"); // Abre o arquivo para salvar a imagem no formato PPM
    imageFile << "P3\n" << nx << " " << ny << "\n255\n"; // Cabeçalho do arquivo PPM: "P3" para cores RGB, tamanho da imagem, valor máximo de cor
    
    // Parâmetros da câmera
    vec3 lookfrom(0, 2, 5);       // Posição da câmera (elevada para visualizar o chão)
    vec3 lookat(0, 0, 0);         // Ponto onde a câmera está olhando
    vec3 vup(0, 1, 0);            // Vetor "para cima" que define a orientação da câmera
    double distance = 1.0;        // Distância entre a câmera e a tela (foco da lente)
    double screen_height = 2.0;   // Altura da tela de visualização
    double screen_width = 2.0;    // Largura da tela de visualização
    camera cam(lookfrom, lookat, vup, distance, screen_height, screen_width, nx);
    
    // Criando o mundo de objetos
    hitable** list = new hitable*[triangles.size()];  // Aloca espaço para os triângulos + 1 (para o chão)

    // Copia os triângulos para o array
    for (size_t i = 0; i < triangles.size(); i++) {
        list[i] = triangles[i];
    }
    // Cria o objeto hitable_list com os triângulos e o chão
    hitable* world = new hitable_list(list, triangles.size());

    // Criando fontes de luz
    light l1(vec3(0, 5, 5), vec3(1.0, 1.0, 1.0), 1.0); // Luz branca
    // light l2(vec3(0, 5, 0), vec3(0.0, 0.0, 0.0), 1.0); // Luz verde
    light l3(vec3(-5, 5, 5), vec3(1.0, 0.0, 0.0), 0.8); // Luz vermelha
    std::vector<light> lights = {l1,l3}; // Lista de luzes

    // Laço para gerar os pixels da imagem
    for (int j = ny - 1; j >= 0; j--) { 
        for (int i = 0; i < nx; i++) { 
            float u = float(i) / float(nx);  
            float v = float(j) / float(ny);
            // Gera o raio para cada pixel, usando a câmera
            ray r = cam.get_ray(u, v);
            // Calcula a cor para o ponto onde o raio atinge
            vec3 col = color_with_shadowsteste(r, world, lights, vec3(0.0, 0.0, 0.0));
            int ir = int(255.99 * col[0]); 
            int ig = int(255.99 * col[1]); 
            int ib = int(255.99 * col[2]); 
            imageFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    std::cerr << "\nFeito.\n";
    imageFile.close();

    delete[] list;
}



int binomial_coefficient(int n, int k) {
    if (k == 0 || k == n) return 1;
    return binomial_coefficient(n - 1, k) + binomial_coefficient(n - 1, k);
}

float bernstein(int i, int n, float t) {
    return binomial_coefficient(n, i) * pow(t, i) * pow(1 - t, n - i);
}

vec3 bezier_surface_point(const std::vector<std::vector<vec3>>& control_points, float u, float v) {
    int n = control_points.size() - 1;       // u
    int m = control_points[0].size() - 1;   //  v

    vec3 point(0, 0, 0);
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= m; j++) {
            float bernstein_u = bernstein(i, n, u);
            float bernstein_v = bernstein(j, m, v);
            point += bernstein_u * bernstein_v * control_points[i][j];
        }
    }
    return point;
}


void generate_bezier_surface(const std::vector<std::vector<vec3>>& control_points, int resolution, std::vector<triangle*>& triangles) {
    std::vector<std::vector<vec3>> surface_points;

    // Gerar os pontos na superfície
    for (int i = 0; i <= resolution; i++) {
        float u = float(i) / resolution;
        surface_points.push_back({});
        for (int j = 0; j <= resolution; j++) {
            float v = float(j) / resolution;
            surface_points[i].push_back(bezier_surface_point(control_points, u, v));
        }
    }

    // Gerar triângulos conectando os pontos
    for (int i = 0; i < resolution; i++) {
        for (int j = 0; j < resolution; j++) {
            vec3 p1 = surface_points[i][j];
            vec3 p2 = surface_points[i + 1][j];
            vec3 p3 = surface_points[i][j + 1];
            vec3 p4 = surface_points[i + 1][j + 1];

            Material materialPersonalizado(0.6f, 0.8f, 1.1f, 0.0f, 0.0f, 0.7f, 1.0f ,vec3(0.0, 1.0, 0.0), 1.0f);

            // Criar dois triângulos para cada quadrado
            triangles.push_back(new triangle(p1, p2, p3, vec3(1.0, 0.0, 0.0), materialPersonalizado)); 
            triangles.push_back(new triangle(p2, p4, p3, vec3(1.0, 0.0, 0.0), materialPersonalizado)); 
        }
    }
}

void generate_revolution_solid_from_bezier_surface(const std::vector<triangle*>& input_triangles, std::vector<triangle*>& output_triangles, int num_segments) {
    float angle_step = 2 * M_PI / num_segments;

    for (const auto& tri : input_triangles) {
        vec3 vertices[3] = {tri->vertex1, tri->vertex2, tri->vertex3};

        for (int i = 0; i < num_segments; i++) {
            float angle1 = i * angle_step;
            float angle2 = (i + 1) * angle_step;

            mat4 rotation1 = rotation_matrix_y(angle1);
            mat4 rotation2 = rotation_matrix_y(angle2);

            vec3 rotated_vertices1[3];
            vec3 rotated_vertices2[3];

            for (int j = 0; j < 3; j++) {
                rotated_vertices1[j] = rotation1 * vertices[j];
                rotated_vertices2[j] = rotation2 * vertices[j];
            }

            Material material = tri->material;

            // Create two triangles for each segment
            output_triangles.push_back(new triangle(rotated_vertices1[0], rotated_vertices2[0], rotated_vertices1[1], tri->cor, material));
            output_triangles.push_back(new triangle(rotated_vertices1[1], rotated_vertices2[0], rotated_vertices2[1], tri->cor, material));

            output_triangles.push_back(new triangle(rotated_vertices1[1], rotated_vertices2[1], rotated_vertices1[2], tri->cor, material));
            output_triangles.push_back(new triangle(rotated_vertices1[2], rotated_vertices2[1], rotated_vertices2[2], tri->cor, material));
        }
    }
}

// bezier_curve
void generate_bezier_curve(const std::vector<vec3>& control_points, int resolution, std::vector<vec3>& curve_points) {
    for (int i = 0; i <= resolution; i++) {
        float t = float(i) / resolution;
        vec3 point(0, 0, 0);
        int n = control_points.size() - 1;
        for (int j = 0; j <= n; j++) {
            float bernstein_value = bernstein(j, n, t);
            point += bernstein_value * control_points[j];
        }
        curve_points.push_back(point);
    }
}

void generate_revolution_solid_from_curve(const std::vector<vec3>& curve_points, std::vector<triangle*>& output_triangles, int num_segments) {
    float angle_step = 2 * M_PI / num_segments;

    for (size_t i = 0; i < curve_points.size() - 1; i++) {
        vec3 p1 = curve_points[i];
        vec3 p2 = curve_points[i + 1];

        for (int j = 0; j < num_segments; j++) {
            float angle1 = j * angle_step;
            float angle2 = (j + 1) * angle_step;

            mat4 rotation1 = rotation_matrix_y(angle1);
            mat4 rotation2 = rotation_matrix_y(angle2);

            vec3 p1_rotated1 = rotation1 * p1;
            vec3 p2_rotated1 = rotation1 * p2;
            vec3 p1_rotated2 = rotation2 * p1;
            vec3 p2_rotated2 = rotation2 * p2;

            Material materialPersonalizado(0.6f, 0.8f, 1.1f, 0.0f, 0.0f, 0.7f, 1.0f, vec3(0.0, 1.0, 0.0), 1.0f);

            // Create two triangles for each segment
            output_triangles.push_back(new triangle(p1_rotated1, p1_rotated2, p2_rotated1, vec3(1.0, 0.0, 0.0), materialPersonalizado));
            output_triangles.push_back(new triangle(p2_rotated1, p1_rotated2, p2_rotated2, vec3(1.0, 0.0, 0.0), materialPersonalizado));
        }
    }
}

void bezier_curve(const std::vector<vec3>& curve_points) {
    int nx = 500;  // Image width
    int ny = 500;  // Image height
    std::ofstream imageFile("bezier_curve.ppm");
    imageFile << "P3\n" << nx << " " << ny << "\n255\n";

    // Initialize the image with a black background
    std::vector<std::vector<vec3>> image(ny, std::vector<vec3>(nx, vec3(0, 0, 0)));

    // Draw the curve points
    for (const auto& point : curve_points) {
        int x = int((point[0] + 1) * nx / 2);  // Map x from [-1, 1] to [0, nx]
        int y = int((point[1] + 1) * ny / 2);  // Map y from [-1, 1] to [0, ny]
        if (x >= 0 && x < nx && y >= 0 && y < ny) {
            image[ny - y - 1][x] = vec3(1, 0, 0);  // Red color for the curve
        }
    }

    // Write the image to the file
    for (int j = 0; j < ny; j++) {
        for (int i = 0; i < nx; i++) {
            vec3 color = image[j][i];
            int ir = int(255.99 * color[0]);
            int ig = int(255.99 * color[1]);
            int ib = int(255.99 * color[2]);
            imageFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    std::cerr << "\nBezier curve generated.\n";
    imageFile.close();
}


void translate_triangles(std::vector<triangle*>& triangles, const vec3& translation) {
    for (auto& tri : triangles) {
        tri->vertex1 += translation;
        tri->vertex2 += translation;
        tri->vertex3 += translation;
    }
}

void scale_triangles(std::vector<triangle*>& triangles, float scale_factor) {
    for (auto& tri : triangles) {
        tri->vertex1 *= scale_factor;
        tri->vertex2 *= scale_factor;
        tri->vertex3 *= scale_factor;
    }
}

void rotate_triangles(std::vector<triangle*>& triangles, const mat4& rotation_matrix) {
    for (auto& tri : triangles) {
        tri->vertex1 = rotation_matrix * tri->vertex1;
        tri->vertex2 = rotation_matrix * tri->vertex2;
        tri->vertex3 = rotation_matrix * tri->vertex3;
    }
}



void render_bezier_surface() {
    // pontos da superfície de Bézier
    // std::vector<std::vector<vec3>> control_points = {
    //     {vec3(-1, 0, -1), vec3(-0.5, 1, -1), vec3(0.5, 1, -1), vec3(1, 0, -1)},
    //     {vec3(-1, 0, -0.5), vec3(-0.5, 1, -0.5), vec3(0.5, 1, -0.5), vec3(1, 0, -0.5)},
    //     {vec3(-1, 0, 0.5), vec3(-0.5, 1, 0.5), vec3(0.5, 1, 0.5), vec3(1, 0, 0.5)},
    //     {vec3(-1, 0, 1), vec3(-0.5, 1, 1), vec3(0.5, 1, 1), vec3(1, 0, 1)}
    // };

    // int resolution = 20;


    // std::vector<triangle*> triangles;
    // generate_bezier_surface(control_points, resolution, triangles);

    // float scale_factor = 2.5;
    // scale_triangles(triangles, scale_factor);

    // float angle = M_PI / 9; 
    // mat4 rotation = rotation_matrix_y(angle);
    // rotate_triangles(triangles, rotation);

    // vec3 translation(0.0, 2.0, 0.0); // Move 2 unidades para cima no eixo Y
    // translate_triangles(triangles, translation);

    // Cria um novo vetor para armazenar os triângulos de revolução
    // std::vector<triangle*> revolution_triangles;
    // int num_segments = 36; // Número de segmentos para a revolução
    // generate_revolution_solid_from_bezier_surface(triangles, revolution_triangles, num_segments);

    // bezier(triangles);

    // CURVA HALF CORACAO:
    // std::vector<vec3> control_points = {
    //     vec3(0.0, -3.0, 0.0),
    //     vec3(-3.0, 0.0, 0.0),
    //     vec3(-1.5, 1.5, 0.0),
    //     vec3(0.0, 0.0, 0.0)
    // }; 

    // curva cuscuzeira:
    std::vector<vec3> control_points = {
        vec3(-1.0, -1.0, 0.0),
        vec3(-0.5, 1.0, 0.0),
        vec3(0.5, -1.0, 0.0),
        vec3(1.0, 1.0, 0.0)
    };



    int resolution = 100;
    std::vector<vec3> curve_points;

    generate_bezier_curve(control_points, resolution, curve_points);
    bezier_curve(curve_points);

    std::vector<triangle*> revolution_triangles;
    int num_segments = 36; // Number of segments for the revolution
    generate_revolution_solid_from_curve(curve_points, revolution_triangles, num_segments);
    bezier(revolution_triangles);
}



int main(){

    objReader obj("inputs/untitled.obj");
    obj.print_faces();
   
    std::vector<std::vector<point>> faces = obj.getFacePoints();

    std::vector<triangle*> triangles;

    mat4 rotation = rotation_matrix_y(M_PI / 8);  
    mat4 scale = scaling_matrix(0.0);

    for (const auto& face : faces) {
        if (face.size() >= 3) { 
            vec3 randomColor(
                static_cast<float>(rand()) / RAND_MAX, // Valor aleatório entre 0 e 1
                static_cast<float>(rand()) / RAND_MAX,
                static_cast<float>(rand()) / RAND_MAX
            );
            Material materialPersonalizado(0.6f, 0.8f, 1.1f, 0.0f, 0.0f, 0.7f, 1.0f ,vec3(0.0, 1.0, 0.0), 1.0f);
            // Aplica a rotação no eixo X a cada vértice
            vec3 v1_rotated = rotation * vec3(face[0].x, face[0].y, face[0].z);
            vec3 v2_rotated = rotation * vec3(face[1].x, face[1].y, face[1].z);
            vec3 v3_rotated = rotation * vec3(face[2].x, face[2].y, face[2].z);

            

            triangles.push_back(new triangle(
                // vec3(face[0].x, face[0].y, face[0].z),
                // vec3(face[1].x, face[1].y, face[1].z),
                // vec3(face[2].x, face[2].y, face[2].z),
                v1_rotated*1.5,
                v2_rotated*1.5,
                v3_rotated*1.5,
                randomColor*1.5,
                materialPersonalizado
            ));
        }
    }

    std::cout << "Total de triângulos armazenados: " << triangles.size() << std::endl;

    // triangulo_auto(triangles); 
    // desafio1();
    render_bezier_surface(); 


    return 0;
}