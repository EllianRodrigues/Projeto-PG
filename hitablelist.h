#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <vector>

class hitable_list: public hitable {
    public:
        hitable_list() {}
        hitable_list(hitable **l, int n) {list = l; list_size = n;}
        virtual bool hit(const ray& r, float tmin, float tmax, hit_record& rec) const;
        hitable **list;
        int list_size;
};

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    // Cria um registro temporário para armazenar as informações da interseção
    hit_record temp_rec;
    // Variável booleana para verificar se algum objeto foi atingido
    bool hit_anything = false;
    // Inicializa a distância mais próxima (closest_so_far) como t_max, 
    // que é a maior distância possível até o momento
    double closest_so_far = t_max;
    // Loop para verificar a interseção do raio com cada objeto da lista
    for (int i = 0; i < list_size; i++) {
        // Verifica se o objeto atual na lista é atingido pelo raio,
        // passando o intervalo de distâncias válidas (t_min e t_max)
        if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
            // Se o objeto for atingido, marca que algo foi atingido
            hit_anything = true;         
            // Atualiza a distância do ponto de interseção mais próximo
            closest_so_far = temp_rec.t;        
            // Atualiza o registro de interseção com as informações do objeto atingido
            rec = temp_rec;
        }
    }
    // Retorna true se algum objeto foi atingido, caso contrário, retorna false
    return hit_anything;
}


#endif