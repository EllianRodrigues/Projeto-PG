#ifndef CYLINDER_H
#define CYLINDER_H

#include "hittable.h"
#include "material.h"

class cylinder: public hitable {
public:
  vec3 base_center;
  vec3 axis;
  float radius;
  float height;
  vec3 cor;
  Material material;

  cylinder() {}

  cylinder(vec3 base, vec3 ax, float r, float h, vec3 cor, const Material& mat)
    : base_center(base), axis(unit_vector(ax)), radius(r), height(h), cor(cor), material(mat) {}

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

      if (temp < t_max && temp > t_min && check_hit(temp)) {
        rec.t = temp;
        rec.p = r.at(rec.t);
        vec3 p_proj = rec.p - base_center - dot(rec.p - base_center, axis_dir) * axis_dir;
        rec.normal = unit_vector(p_proj);
        rec.cor = cor;
        rec.objeto = "cilindro";
        rec.material = material;
        return true;
      }

      temp = (-b + sqrt_discriminant) / (2.0f * a);
      if (temp < t_max && temp > t_min && check_hit(temp)) {
        rec.t = temp;
        rec.p = r.at(rec.t);
        vec3 p_proj = rec.p - base_center - dot(rec.p - base_center, axis_dir) * axis_dir;
        rec.normal = unit_vector(p_proj);
        rec.cor = cor;
        rec.objeto = "cilindro";
        rec.material = material;
        printf("oi\n");
        return true;
      }
    }
    return false;
  }
};

#endif