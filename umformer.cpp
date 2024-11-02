#include "umformer.h"


void Converter::loadJson(const std::string& filename) {
    // Чтение JSON-файла и создание объектов
    /*std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: Не удалось открыть файл " << filename << std::endl;
        return;
    }*/
    FILE* fp = fopen(filename.c_str(), "rb"); 
    if (!fp){
        std::cerr << "Error. Can't open a file " << filename << std::endl;
        return;
    }
     /*std::stringstream buffer;
    buffer << fp.rdbuf();
    std::string jsonString = buffer.str();*/
    char readBuffer[65536]; 
    rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
    rapidjson::Document document; 
    document.ParseStream(is);
    if (document.HasParseError()) {
        std::cerr << "Parse error JSON: " << rapidjson::GetParseError_En(document.GetParseError()) << std::endl;
        return;
    }
    // Check layers
    assert(document.IsObject());
    const Value& b = document["layers"];
    assert(b.IsArray());
    //layers
    for (SizeType i = 0; i < b.Size(); i++){
        assert(b[i].IsObject());
        //layername
        assert(b[i].HasMember("name"));
        cout<< b[i]["name"].GetString();
        Layer layer{b[i]["name"].GetString()};//object layer with name
        //polygons
        assert(b[i].HasMember("polygons"));
        const Value& pol = b[i]["polygons"];
        assert(pol.IsArray());
        Polygon polygon;
        //Add if size zero
        for(SizeType j = 0; j < pol.Size(); j++){
            assert(pol[j].IsObject());
            //coordinates
            assert(pol[j].HasMember("cords"));
            const Value& cordar = pol[j]["cords"];
            /*
            for (SizeType k = 0; k < cordar.Size(); k++){
                const Value& coor = cordar[k];
                for (SizeType l = 0; l < coor.Size(); l++){
                    cout<<coor[l].GetDouble()<<endl;
                }
            }*/
            assert(cordar.IsArray());
            for (const Value& cord : cordar.GetArray()) {
                Point dot{cord[0].GetDouble(),cord[1].GetDouble()};
                std::cout << cord[0].GetDouble() << ", " << cord[1].GetDouble() << std::endl;
                polygon.append(dot);
            }
            //holes
            assert(pol[j].HasMember("holes"));
            const Value& holear = pol[j]["holes"];
            //Add if Size zero
            assert(holear.IsArray());
            for (const Value& holee : holear.GetArray()) {
                for (const Value& pt : holee.GetArray()) {
                    Point point{pt[0].GetDouble(),pt[1].GetDouble()};
                    Hole hole;
                    hole.append(point);
                    std::cout << pt[0].GetDouble() << ", " << pt[1].GetDouble() << std::endl;
                    polygon.add_hole(hole);
                    }
            }
            layer.append(polygon);
        }
    this->layerpack.append_layer(layer);
    }
    fclose(fp);   
};
void Converter::saveToJson(const std::string& filename){
    // Write to file, if no file - make one
    rapidjson::Document document;
    document.SetObject();
    Value layersObject(kArrayType);//empty array for layers
    //unordered_map<std::string, Layer> layerMap =  this->layerpack.get_layers_by_name();//key name - layer
    vector<Layer> layerIndexs = this->layerpack.get_layers();//vector of layers
    for(int i=0; i<layerIndexs.size();i++)//to add all layers
    {
        const Layer& layer = layerIndexs[i];//layer with index i
        string names = layer.get_name();//layer name with index i
        Value layerObject(kObjectType);//making layer
        layerObject.AddMember("name",Value(names.c_str(), document.GetAllocator()),document.GetAllocator());
        layerObject.AddMember("level",Value(to_string(i).c_str(), document.GetAllocator()),document.GetAllocator());//maybewrong
        vector<Polygon> polygonIndex = layer.get_polygons();//polygons in layer i
        Value polygonsArray(rapidjson::kArrayType);//array to store polygons
        for (int j=0;j<polygonIndex.size();j++)
        {
            const Polygon& polygon = polygonIndex[j];//polygon in layer
            //polygon.ravel() VarientType
            vector<Point> pointIndex = polygon.get_vertices();
            for (int k=0;k<pointIndex.size();k++)
            {
                const Point& point = pointIndex[k];//coord of polygon
                unordered_map<string,double> pointCoords = point.ravel();//"x"-x,"y"-y
                double x = pointCoords.at("x");//add as x
                double y = pointCoords.at("y");// add as y
            }
            vector<Hole> holeIndex = polygon.get_holes();//holes in polygon
            for (int k=0;k<holeIndex.size();k++)
            {
                const Hole& hole = holeIndex.at(k);
                vector<Point> pointHole = hole.get_vertices();
                for(int m=0; m<pointHole.size();m++)
                {
                    const Point& point = pointHole[m];//coord of hole
                    unordered_map<string,double> holeCoords = point.ravel();//"x"-x,"y"-y
                    double x = holeCoords.at("x");//add as x
                    double y = holeCoords.at("y");
                }
            };
        }
        
    }
    /*{
    "layers": [

        {"layer_name_1": {

            "name": layer_name_1,

            "polygons": [...]

        }},

        {"layer_name_2": {

            "name": layer_name_2,

            "polygons": [...]

        }},

        ...

    ]}*/
    rapidjson::Document document;
    document.SetObject();
    };
LayerPack& Converter::getLayerPack(){
        return this->layerpack;
    }