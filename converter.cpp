#include "converter.h"


void Converter::loadJson(const std::string& filename) {
    // Чтение JSON-файла
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: Не удалось открыть файл " << filename << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string jsonString = buffer.str();
    
    rapidjson::Document document;
    document.Parse(jsonString.c_str());

    if (document.HasParseError()) {
        std::cerr << "Ошибка разбора JSON: " << rapidjson::GetParseError_En(document.GetParseError()) << std::endl;
        return;
    }

    // Проверка, что корень - массив
    if (!document.IsArray()) {
        std::cerr << "Ошибка: Ожидался массив слоев." << std::endl;
        return;
    }

    /*for (const auto& layerValue: document.GetObject()) {
        if (!layerValue.IsArray()) {
            std::cerr << "Ошибка: Ожидался объект слоя." << std::endl;
            continue;
        }*/
       const rapidjson::Value& layersArray = document["layers"];

       for (const rapidjson::Value& layer : layersArray.GetArray()) {
        // Выводим имя слоя
        //std::cout << "Слой: " << layer.GetString() << std::endl;
        Layer layer;
        layer.name = layersArray["name"].GetString();
        
       }

        // Проверяем, есть ли полигоны
        const auto& polygonsValue = layerValue["polygons"];
        for (const auto& polygonValue : polygonsValue.GetArray()) {
            Polygon polygon;
            
            // Получаем координаты
            const auto& coordsValue = polygonValue["coords"];
            for (const auto& coordValue : coordsValue.GetArray()) {
                Point coord;
                coord.x = coordValue[0].GetInt();
                coord.y = coordValue[1].GetInt();
                polygon.coords.push_back(coord);
            }

            // Получаем дыры (если есть)
            const auto& holesValue = polygonValue["holes"];
            if (holesValue.IsArray()) {
                for (const auto& holeValue : holesValue.GetArray()) {
                    std::vector<Coordinate> hole;
                    for (const auto& coordValue : holeValue.GetArray()) {
                        Coordinate coord;
                        coord.x = coordValue[0].GetInt();
                        coord.y = coordValue[1].GetInt();
                        hole.push_back(coord);
                    }
                    polygon.holes.push_back(hole);
                }
            }

            layer.polygons.push_back(polygon);
        }

        layers.push_back(layer);
    }

    std::cout << "Файл успешно загружен. Всего слоев: " << layers.size() << std::endl;
}
void Converter::saveToJson(const std::string& filename){
    // Создание объекта JSON
        rapidjson::Document document;
        document.SetObject();

        rapidjson::Value layersArray(rapidjson::kArrayType);

        for (const auto& layer : layers) {
            rapidjson::Value layerObject(rapidjson::kObjectType);
            layerObject.AddMember("name", rapidjson::Value(layer.name.c_str(), document.GetAllocator()), document.GetAllocator());

            rapidjson::Value polygonsArray(rapidjson::kArrayType);
            for (const auto& polygon : layer.polygons) {
                rapidjson::Value polygonObject(rapidjson::kObjectType);

                // Обработка вершин
                rapidjson::Value cordsArray(rapidjson::kArrayType);
                for (const auto& vertex : polygon.vertices) {
                    rapidjson::Value pointArray(rapidjson::kArrayType);
                    pointArray.PushBack(vertex.x, document.GetAllocator());
                    pointArray.PushBack(vertex.y, document.GetAllocator());
                    cordsArray.PushBack(pointArray, document.GetAllocator());
                }
                polygonObject.AddMember("cords", cordsArray, document.GetAllocator());

                // Обработка дыр
                rapidjson::Value holesArray(rapidjson::kArrayType);
                for (const auto& hole : polygon.holes) {
                    rapidjson::Value holeArray(rapidjson::kArrayType);
                    for (const auto& point : hole) {
                        rapidjson::Value pointArray(rapidjson::kArrayType);
                        pointArray.PushBack(point.x, document.GetAllocator());
                        pointArray.PushBack(point.y, document.GetAllocator());
                        holeArray.PushBack(pointArray, document.GetAllocator());
                    }
                    holesArray.PushBack(holeArray, document.GetAllocator());
                }
                polygonObject.AddMember("holes", holesArray, document.GetAllocator());

                polygonsArray.PushBack(polygonObject, document.GetAllocator());
            }

            layerObject.AddMember("polygons", polygonsArray, document.GetAllocator());
            layersArray.PushBack(layerObject, document.GetAllocator());
        }

        document.AddMember("layers", layersArray, document.GetAllocator());

        // Запись в файл
        std::ofstream ofs(filename);
        if (!ofs.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return;
        }

        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);
        ofs << buffer.GetString();
        ofs.close();
    };