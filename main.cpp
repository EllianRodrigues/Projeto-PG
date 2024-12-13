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


float hit_sphere(const vec3&center , float radius, const ray& r){ //esfera
    vec3 oc = r.origin() - center;
    float a= dot(r.direction(),r.direction());
    float b=2.0 *dot(oc, r.direction());
    float c = dot(oc, oc) - radius*radius;
    float discriminant = b*b -4*a*c;
    if (discriminant <0)
        return -1.0;
    else 
        return (-b - sqrt(discriminant))/(2.0*a);
} 

vec3 color(const ray& r){
    float t = hit_sphere(vec3(0,0,-1), 0.5, r);
    if(t > 0.0) {
        vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
        return 0.5*vec3(N.x()+1,N.y()+1,N.z()+1);
    }
    vec3 unit_direct = unit_vector(r.direction());
    t = 0.5*(unit_direct.y() + 1.0);
    return (1.0-t)*vec3(1.0,1.0,1.0) + t*vec3(0.5, 0.7,1.0); 
}

vec3 color2(const ray& r,hitable *world){
    hit_record rec;
    if (world->hit(r, 0.0, FLT_MAX, rec)) {
        return 0.5*vec3(rec.normal.x()+1, rec.normal.y()+1, rec.normal.z()+1);
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0-t)*vec3(1.0,1.0,1.0) + t*vec3(0.5, 0.7, 1.0);
    }

}

void rgpImage(){
    int nx = 200;  
    int ny = 100;  
    std::ofstream imageFile("rgb.ppm");
    imageFile << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny - 1; j >= 0; j--) {  
        for (int i = 0; i < nx; i++) {
            float r = float(i) / float(nx);
            float g = float(j) / float(ny);
            float b = 0.2;
            int ir = int(255.99 * r);
            int ig = int(255.99 * g);
            int ib = int(255.99 * b);
            imageFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    std::cerr << "\nFeito.\n";
    imageFile.close();
}


void image(){

    int nx = 200;  
    int ny = 100;  
    std::ofstream imageFile("image.ppm");
    imageFile << "P3\n" << nx << " " << ny << "\n255\n";
    vec3 lower_left_corner(-2.0,-1.0,-1.0);
    vec3 horizontal (4.0,0.0,0.0);
    vec3 vertical(0.0,2.0,0.0);
    vec3 origin(0.0, 0.0, 0.0);
    for (int j = ny - 1; j >= 0; j--) {  
        for (int i = 0; i < nx; i++) {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            ray r(origin,lower_left_corner + u*horizontal + v*vertical);
            vec3 col = color(r);
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

    colormap cmap("inputs/cubo.mtl");
    objReader obj("inputs/cubo.obj", cmap);

    obj.print_faces();
   
    // // Gerar imagem RGB em PPM
    // rgpImage();
    

    // // Camera com fundo
    // image();


    int nx = 200;  
    int ny = 100;  
    std::ofstream imageFile("chao.ppm");
    imageFile << "P3\n" << nx << " " << ny << "\n255\n";
    vec3 lower_left_corner(-2.0,-1.0,-1.0);
    vec3 horizontal (4.0,0.0,0.0);
    vec3 vertical(0.0,2.0,0.0);
    vec3 origin(0.0, 0.0, 0.0);
    hitable *list[2];
    list[0] = new sphere(vec3(0,0,-1), 0.5);
    // list[1] = new sphere(vec3(0,-100.5,-1), 100);
    list[1] = new plane(vec3(0, -0.5, 0), vec3(0, 1, 0)); 
    hitable *world = new hitable_list(list, 2);
    for (int j = ny - 1; j >= 0; j--) {  
        for (int i = 0; i < nx; i++) {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);
            ray r(origin,lower_left_corner + u*horizontal + v*vertical);
            vec3 p = r.at(2.0);
            vec3 col = color2(r, world);
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            imageFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    std::cerr << "\nFeito.\n";
    imageFile.close();




    return 0;
}