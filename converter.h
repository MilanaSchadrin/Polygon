#ifndef CONVERTER_H
#define CONVERTER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include "rapidjson/filereadstream.h"
#include <unordered_map>
//include "polygon.h"
//include "layer.h"
//include "point.h"
//include "layerpack.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/error/en.h"

// Структура координат
class Point {
    double x;
    double y;
    std::unordered_map<std::string, double> ravel() const;
};

class Hole
{
    std::vector<Point> vertices;
    void append(const Point& point);
    std::unordered_map<std::string, Hole> ravel() const
};

// Структура для описания полигона
class Polygon {
    std::vector<Point> coords;
    std::vector<Hole> holes;
    std::unordered_map<std::string, Polygon> ravel() const;
    void add_hole(const Hole& hole);
    void append(const Point& point);
};

// Структура для описания слоя
class Layer {
    std::string name;
    void append(const Polygon& polygon);
    std::vector<Polygon> polygons;
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
};
// Класс для разбора JSON-файла
class Converter {
public:
    std::vector<Layer> layers;

    // Метод для загрузки и разбора JSON
    void loadJson(const std::string& filename);
    void saveToJson(const std::string& filename);
};

#endif 