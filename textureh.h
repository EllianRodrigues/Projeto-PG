#ifndef TEXTUREH
#define TEXTUREH
#define STB_IMAGE_IMPLEMENTATION

#include <bits/stdc++.h>
#include <iostream>
#include "stb_image.h"

using namespace std;

class Texture {
public:
    string name, path;
    int width, height, channels;
    unsigned char* image = nullptr;

    Texture(const string& name) : name(name) {
        if(name != "none" && name != ""){
            path = "textures/" + name + ".jpg";
            image = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb);
            if (!image) {cerr << "Não foi possivel achar o arquivo: " << path << endl;}
        }
    }
    
};

#endif