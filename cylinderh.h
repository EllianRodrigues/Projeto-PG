#ifndef CYLINDER_H
#define CYLINDER_H

#include "hittable.h"
#include "material.h"
#include "textureh.h" // Include the texture header

class cylinder: public hitable {
public:
  vec3 base_center;
  vec3 axis;
  float radius;
  float height;
  vec3 cor;
  Material material;
  Texture* texture;

  cylinder() : texture(nullptr) {}

  cylinder(vec3 base, vec3 ax, float r, float h, const Material& mat, Texture* tex = nullptr)
    : base_center(base), axis(unit_vector(ax)), radius(r), height(h), material(mat), texture(tex) {}

  virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override {
    vec3 oc = r.origin() - base_center;
    vec3 axis_dir = unit_vector(axis);

    // Project ray direction and origin onto the plane perpendicular to the cylinder's axis
    vec3 d_proj = r.direction() - dot(r.direction(), axis_dir) * axis_dir;
    vec3 oc_proj = oc - dot(oc, axis_dir) * axis_dir;

    float a = dot(d_proj, d_proj);
    float b = 2.0f * dot(oc_proj, d_proj);
    float c = dot(oc_proj, oc_proj) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    if (discriminant > 0) {
      float sqrt_discriminant = sqrt(discriminant);
      float temp = (-b - sqrt_discriminant) / (2.0f * a);

      auto check_hit = [&](float t) -> bool {
        vec3 p = r.at(t);
        float proj_length = dot(p - base_center, axis_dir);
        return proj_length >= 0 && proj_length <= height;
      };

      auto setup_record = [&](float t) -> void {
        rec.t = t;
        rec.p = r.at(rec.t);
        vec3 p_proj = rec.p - base_center - dot(rec.p - base_center, axis_dir) * axis_dir;
        rec.normal = unit_vector(p_proj);
        
        if (texture && texture->image) {
          // Convert 3D point to texture coordinates
          float phi = atan2(p_proj.z(), p_proj.x()) + M_PI; // angle around axis [0, 2π]
          float v = dot(rec.p - base_center, axis_dir) / height; // height position [0, 1]
          
          // Map to texture coordinates
          int tx = static_cast<int>((phi / (2 * M_PI)) * texture->width) % texture->width;
          int ty = static_cast<int>(v * texture->height) % texture->height;
          
          // Get color from texture
          if (tx >= 0 && tx < texture->width && ty >= 0 && ty < texture->height) {
            int idx = (ty * texture->width + tx) * 3;
            rec.cor = vec3(
              texture->image[idx] / 255.0f,
              texture->image[idx + 1] / 255.0f,
              texture->image[idx + 2] / 255.0f
            );
          } else {
            rec.cor = cor;
          }
        } else {
          rec.cor = cor;
        }
        

        // Set the color in the material object
        rec.material = material;
        rec.material.color = rec.cor;
        // printf("cilindro Color: %f %f %f\n", cor[0], cor[1], cor[2]);
        rec.objeto = "cilindro";
      };

      if (temp < t_max && temp > t_min && check_hit(temp)) {
        // printf("color 3\n");
        setup_record(temp);
        return true;
      }

      temp = (-b + sqrt_discriminant) / (2.0f * a);
      if (temp < t_max && temp > t_min && check_hit(temp)) {
        // printf("color 3\n");
        setup_record(temp);
        return true;
      }
    }
    return false;
  }
};

#endif