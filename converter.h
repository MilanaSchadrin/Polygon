#ifndef CONVERTER_H
#define CONVERTER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
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
class Coordinate {
    float x;
    float y;
};

// Структура для описания полигона
class Polygon {
    std::vector<Coordinate> coords;
    std::vector<std::vector<Coordinate>> holes;
};

// Структура для описания слоя
class Layer {
    std::string name;
    int level;
    std::vector<Polygon> polygons;
};

class LayerPack {
public:
    std::vector<Layer> layers;

    std::unordered_map<std::string, VariantType> ravel() {
        // Реализация метода ravel
        // Возвращает словарь: названия полей - значения полей
    }
}
// Класс для разбора JSON-файла
class Converter {
public:
    std::vector<Layer> layers;

    // Метод для загрузки и разбора JSON
    void loadJSON(const std::string& filename);
    void saveToJson(const std::string& filename);
};

#endif 