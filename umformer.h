#ifndef UMFORMER_H
#define UMFORMER_H

#include <map>
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include <unordered_map>
//include "polygon.h"
//include "layer.h"
//include "point.h"
//include "layerpack.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"
using namespace std; 
using namespace rapidjson;

// Структура координат
class Point {
    public:
    double x;
    double y;
    std::unordered_map<std::string, double> ravel() const;

Point(double _x, double _y) {
		x = _x;
		y= _y;
}
};
class Hole
{
    std::vector<Point> vertices;
    public:
    void append(const Point& point);
    const std::vector<Point>& get_vertices() const ;
    std::unordered_map<std::string, Hole> ravel() const;
};

// Структура для описания полигона
class Polygon {
    std::vector<Point> coords;
    std::vector<Hole> holes;
    public:
    std::unordered_map<std::string, Polygon> ravel() const;
    void add_hole(const Hole& hole);
    void append(const Point& point);
    const std::vector<Point>& get_vertices() const;
    const Point& operator[](size_t index) const;
    const std::vector<Hole>& get_holes() const;

};

// Структура для описания слоя
class Layer {
    public:
    std::string name;
    const std::string& get_name() const;
    void append(const Polygon& polygon);
    std::vector<Polygon> polygons;
    const std::vector<Polygon>& get_polygons() const;
    const Polygon& operator[](size_t index) const;
    std::unordered_map<std::string, Layer> ravel() const;
};

class LayerPack {
public:
    std::vector<Layer> layers;
    void append_layer(const Layer& layer);
    std::unordered_map<std::string, Layer> get_layers_by_name() const {
        // Реализация метода ravel
        // Возвращает словарь: названия полей - значения полей
    };
    const std::vector<Layer>& get_layers() const;
    const Layer& operator[](size_t index) const;
    const Layer& operator[](const std::string& name) const;
};
// Класс для разбора JSON-файла
class Converter {
public:
    LayerPack layerpack;
    // Метод для загрузки и разбора JSON
    void loadJson(const std::string& filename);
    void saveToJson(const std::string& filename);
    LayerPack& getLayerPack();
};

#endif 