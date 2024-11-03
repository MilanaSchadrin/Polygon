#ifndef UMFORMER_H
#define UMFORMER_H

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "rapidjson-1.1.0/include/rapidjson/filereadstream.h"
#include "rapidjson-1.1.0/include/rapidjson/filewritestream.h"
#include "rapidjson-1.1.0/include/rapidjson/document.h"
#include "rapidjson-1.1.0/include/rapidjson/prettywriter.h"
#include "rapidjson-1.1.0/include/rapidjson/stringbuffer.h"
#include "rapidjson-1.1.0/include/rapidjson/error/en.h"

using namespace std; 
using namespace rapidjson;

// Структура координат
class Point {
    public:
    double x;
    double y;
    std::unordered_map<std::string, double> ravel() const
    {
        unordered_map<std::string, double> po =
        {
            {"x",this->x},
            {"y",this->y}
        };
        return po;
    };

Point(double _x, double _y) {
		x = _x;
		y= _y;
}
};
class Hole
{
    std::vector<Point> vertices;
    public:
    void append(const Point& point)
    {
        this->vertices.push_back(point);
    };
    const std::vector<Point>& get_vertices() const
    {
        return this->vertices;
    };
};

// Структура для описания полигона
class Polygon {
    public:
    Polygon() {};
    std::vector<Point> coords;
    std::vector<Hole> holes;
    void add_hole(const Hole& hole){
        this->holes.push_back(hole);
    };
    void append(const Point& point)
    {
        this->coords.push_back(point);
    };
    const std::vector<Point>& get_vertices() const
    {
        return this->coords;
    };
    const Point& operator[](size_t index) const
    {
        if (index >= coords.size()) 
        {
            throw std::out_of_range("Индекс выходит за границы вектора.");
        }
        return coords[index];
    };
    const std::vector<Hole>& get_holes() const
    {
        return this->holes;
    };

};

// Структура для описания слоя
class Layer {
    
    std::string name;
    std::vector<Polygon> polygons;
    public:
    const std::string& get_name() const
    {
        return this->name;
    };
    void append(const Polygon& polygon)
    {
        this->polygons.push_back(polygon);
    };
    const std::vector<Polygon>& get_polygons() const
    {
        return this->polygons;
    };
    const Polygon& operator[](size_t index) const
    {
        if (index >= polygons.size()) 
        {
            throw std::out_of_range("Индекс выходит за границы вектора.");
        }
        return polygons[index];
    };
    Layer(string names) 
    {
        name = names;
    }
};

class LayerPack {
public:
    std::vector<Layer> layers;
    public:
    void append_layer(const Layer& layer)
    {
        this->layers.push_back(layer);
    };
    const std::vector<Layer>& get_layers() const
    {
        return this->layers;
    };
    const Layer& operator[](size_t index) const
    {
        if (index >= layers.size()) 
        {
            throw std::out_of_range("Индекс выходит за границы вектора.");
        }
        return layers[index];
    };
};

// Класс для разбора JSON-файла
class Converter {
    LayerPack layerpack;
public:
    // Метод для загрузки и разбора JSON
    Converter() {};
    void loadJson(const std::string& filename);
    void saveToJson(const std::string& filename);
    LayerPack& getLayerPack();
};

#endif 