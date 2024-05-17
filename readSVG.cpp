#include <iostream>
#include <sstream>
#include "SVGElements.hpp"
#include "external/tinyxml2/tinyxml2.h"
#include "Color.hpp"
#include <string>
#include <map>

using namespace std;
using namespace tinyxml2;

namespace svg
{
    void readSVG(const string &svg_file, Point &dimensions, vector<SVGElement *> &svg_elements)
    {
        XMLDocument doc;
        XMLError r = doc.LoadFile(svg_file.c_str());
        if (r != XML_SUCCESS)
        {
            throw runtime_error("Unable to load " + svg_file);
        }
        XMLElement *xml_elem = doc.RootElement();

        dimensions.x = xml_elem->IntAttribute("width");
        dimensions.y = xml_elem->IntAttribute("height");

        // Dar traverse aos XML child nodes
        XMLElement *child = xml_elem->FirstChildElement();
        while (child != nullptr)
        {
            

            //Declarar o default de rotate, scale e translate para ser usado depois
            int rotate = 0;
            Point translate = {0,0};
            int scale = 1;
            Point transform_origin = {0,0};

            // Criar SVGElement de acordo com o nome do node
            const char *name = child->Name();
            // Verificar se existe transform
            map<string, string> atributes_map; // Um mapa de strings, as chaves serão os nomes dos atributos e os valores serão
            // os valores dos atributos
            //este código criará um mapa onde as chaves serão os nomes dos atributos e os valores serão os valores dos atributos
            for (const XMLAttribute *attr = child->FirstAttribute(); attr != nullptr; attr = attr->Next()) { // itera sobre o mapa até não haver mais atributos
                atributes_map.insert({attr->Name(), attr->Value()}); // a forma para inserir os atributos, nomes e valores
            }
            if (atributes_map.find("transform") != atributes_map.end()) {
                string transform = child->Attribute("transform");

                //rotate

                string new_substr = transform.substr(0,transform.find("(", 0)); //crio nova substring "geral" da transformação que vai do indíce 0 até o primeiro "("
                if(new_substr == "rotate"){
                    string values = transform.substr(transform.find("(", 0) + 1, transform.find(")", 0));//obter os valores dentro dos parênteses, por exemplo
                    //se fôr rotate(180), a partir da substring conseguimos o 180
                    rotate = stoi(values); //converter a string do número para inteiro

                } else if(new_substr == "translate"){
                    string value_x = transform.substr(transform.find("(", 0) + 1, transform.find(" ", 0));
                    string value_y = transform.substr(transform.find(" ", 0) + 1, transform.find(")",0));
                    int x = stoi(value_x);
                    int y = stoi(value_y);
                    translate = {x, y};
                   
                } else if(new_substr == "scale"){
                    string values = transform.substr(transform.find("(", 0) + 1, transform.find(")", 0));//obter os valores dentro dos parênteses
                    scale = stoi(values); //converter a string do número para int
                }
            }
            if (atributes_map.find("transform_origin") != atributes_map.end()) { // para o caso de encontrar o transform_origin

                string transform = child->Attribute("transform_origin"); //obtém o valor de transform_origin dos nodes
                string value_x = transform.substr(transform.find("(", 0) + 1, transform.find(" ", 0));//o valor de x
                string value_y = transform.substr(transform.find(" ", 0) + 1, transform.find(")",0));//o valor de y
                int x = stoi(value_x);
                int y = stoi(value_y);
                transform_origin = {x, y}; //a forma final do transform_origin
            }

            if (strcmp(name, "rect") == 0)
            {
                // Dar parse aos atributos do retângulo e criar rect
                int x = child->IntAttribute("x");
                int y = child->IntAttribute("y");
                int width = child->IntAttribute("width");
                int height = child->IntAttribute("height");
                string fill = child->Attribute("fill");
                Color color = parse_color(fill);
                svg_elements.push_back(new rect(color, {x, y}, width, height));
                svg_elements.back()->rotate(transform_origin, rotate);
                svg_elements.back()->translate(translate);
                svg_elements.back()->scale(transform_origin, scale);
            }
            else if (strcmp(name, "circle") == 0)
            {
                // Dar parse aos atributos do círculo e criar Circle
                int cx = child->IntAttribute("cx");
                int cy = child->IntAttribute("cy");
                int r = child->IntAttribute("r");
                string fill = child->Attribute("fill");
                Color color = parse_color(fill);
                svg_elements.push_back(new Circle(color, {cx, cy}, {r, r}));
                svg_elements.back()->rotate(transform_origin, rotate);
                svg_elements.back()->translate(translate);
                svg_elements.back()->scale(transform_origin, scale);
            }
            else if (strcmp(name, "line") == 0)
            {
                // Dar parse aos atributos da linha e criar line
                int x1 = child->IntAttribute("x1");
                int y1 = child->IntAttribute("y1");
                int x2 = child->IntAttribute("x2");
                int y2 = child->IntAttribute("y2");
                string stroke = child->Attribute("stroke");
                Color color = parse_color(stroke);
                svg_elements.push_back(new line({x1, y1}, {x2, y2}, color));
                svg_elements.back()->rotate(transform_origin, rotate);
                svg_elements.back()->translate(translate);
                svg_elements.back()->scale(transform_origin, scale);
            }
            else if (strcmp(name, "ellipse") == 0)
            {
                // Dar parse aos atributos da elipse e criar Ellipse
                int cx = child->IntAttribute("cx");
                int cy = child->IntAttribute("cy");
                int rx = child->IntAttribute("rx");
                int ry = child->IntAttribute("ry");
                string fill = child->Attribute("fill");
                Color color = parse_color(fill);
                svg_elements.push_back(new Ellipse(color, {cx, cy}, {rx, ry}));
                svg_elements.back()->rotate(transform_origin, rotate);
                svg_elements.back()->translate(translate);
                svg_elements.back()->scale(transform_origin, scale);
            }

            else if (strcmp(name, "polyline") == 0)
            {
                // Dar parse aos atributos do polyline e criar polyline
                string points_str = child->Attribute("points");
                vector<Point> points = {{0,0}, {10,10}};
                istringstream iss(points_str); // points="0,150 0,200 50,200"
                int x, y;
                char comma;
                while (iss >> x >> comma >> y)
                {
                    Point p;
                    p.x = x;
                    p.y = y;
                    points.push_back(p);
                }
                string fill = child->Attribute("fill");
                Color color = parse_color(fill);
                svg_elements.push_back(new polyline(color, points));
                svg_elements.back()->rotate(transform_origin, rotate);
                svg_elements.back()->translate(translate);
                svg_elements.back()->scale(transform_origin, scale);
            }

            if (strcmp(name, "polygon") == 0)
            {
                
                // Dar parse aos atributos do polígono e criar polygon
                string points_str = child->Attribute("points");

                int x;
                int y;
                
                size_t pos = 0;

                std::vector<Point> points;

                while (pos < points_str.length()) {

                    // Encontrar x, sabe-se que ',' separa o x e o y
                    size_t end = points_str.find(',', pos);
                    if (pos != 0) { //não estamos no início da string
                        x = stoi(points_str.substr(pos + 1, end - pos)); //extrai o x
                    } else { //estamos no início da string
                        x = stoi(points_str.substr(pos, end - pos));
                    }
                    pos = end; //atualizamos o pos, para começar depois da vírgula

                    // Encontrar y, sabe-se que o y está entre a vírgula e o espaço
                    end = points_str.find(' ', pos);
                    if (end == std::string::npos) { // Verificar se o final da string foi encontrada
                        end = points_str.length();
                    }
                    y = stoi(points_str.substr(pos + 1, end - pos));

                    // Adicionar o ponto ao vetor
                    points.push_back({x, y});
                    pos = end; //atualizar novamente a variável
                }

                string fill = child->Attribute("fill");
                Color color = parse_color(fill);
                svg_elements.push_back(new polygon(color, points));
                svg_elements.back()->rotate(transform_origin, rotate);
                svg_elements.back()->translate(translate);
                svg_elements.back()->scale(transform_origin, scale);
            }

            else if (strcmp(name, "g") == 0)
            {
                const char *id = child->Attribute("id");
                vector<SVGElement *> group_elements;

                XMLElement *group_child = child->FirstChildElement();

                while (group_child != nullptr)
                {
                    const char *group_child_name = group_child->Name();

                    if (strcmp(group_child_name, "rect") == 0)
                    {
                        // Dar parse aos atributos do retângulo e criar rect
                        int x = child->IntAttribute("x");
                        int y = child->IntAttribute("y");
                        int width = child->IntAttribute("width");
                        int height = child->IntAttribute("height");
                        string fill = child->Attribute("fill");
                        Color color = parse_color(fill);
                        group_elements.push_back(new rect(color, {x, y}, width, height));
                        svg_elements.back()->rotate(transform_origin, rotate);
                        svg_elements.back()->translate(translate);
                        svg_elements.back()->scale(transform_origin, scale);
                    }
                    else if (strcmp(group_child_name, "circle") == 0)
                    {
                        // Dar parse aos atributos do círculo e criar Circle
                        int cx = child->IntAttribute("cx");
                        int cy = child->IntAttribute("cy");
                        int r = child->IntAttribute("r");
                        string fill = child->Attribute("fill");
                        Color color = parse_color(fill);
                        group_elements.push_back(new Circle(color, {cx, cy}, {r, r}));
                        svg_elements.back()->rotate(transform_origin, rotate);
                        svg_elements.back()->translate(translate);
                        svg_elements.back()->scale(transform_origin, scale);
                    }
                    else if (strcmp(group_child_name, "line") == 0)
                    {
                        // Dar parse aos atributos da linha e criar line
                        int x1 = child->IntAttribute("x1");
                        int y1 = child->IntAttribute("y1");
                        int x2 = child->IntAttribute("x2");
                        int y2 = child->IntAttribute("y2");
                        string stroke = child->Attribute("stroke");
                        Color color = parse_color(stroke);
                        group_elements.push_back(new line({x1, y1}, {x2, y2}, color));
                        svg_elements.back()->rotate(transform_origin, rotate);
                        svg_elements.back()->translate(translate);
                        svg_elements.back()->scale(transform_origin, scale);
                    }
                    else if (strcmp(group_child_name, "ellipse") == 0)
                    {
                        // Dar parse aos atributos da elipse e criar Ellipse
                        int cx = child->IntAttribute("cx");
                        int cy = child->IntAttribute("cy");
                        int rx = child->IntAttribute("rx");
                        int ry = child->IntAttribute("ry");
                        string fill = child->Attribute("fill");
                        Color color = parse_color(fill);
                        group_elements.push_back(new Ellipse(color, {cx, cy}, {rx, ry}));
                        svg_elements.back()->rotate(transform_origin, rotate);
                        svg_elements.back()->translate(translate);
                        svg_elements.back()->scale(transform_origin, scale);
                    }

                    else if (strcmp(group_child_name, "polyline") == 0)
                    {
                        // Dar parse aos atributos do polyline e criar polyline
                        string points_str = child->Attribute("points");
                        vector<Point> points;
                        istringstream iss(points_str);
                        int x, y;
                        char comma;
                        while (iss >> x >> comma >> y)
                        {
                            Point p;
                            p.x = x;
                            p.y = y;
                            points.push_back(p);
                        }
                        string fill = child->Attribute("fill");
                        Color color = parse_color(fill);
                        group_elements.push_back(new polyline(color, points));
                        svg_elements.back()->rotate(transform_origin, rotate);
                        svg_elements.back()->translate(translate);
                        svg_elements.back()->scale(transform_origin, scale);
                    }

                    if (strcmp(group_child_name, "polygon") == 0)
                    {
                        // Dar parse aos atributos do polígono e criar polygon
                        string points_str = child->Attribute("points");
                        vector<Point> points;
                        istringstream iss(points_str);
                        int x, y;
                        char comma;
                        while (iss >> x >> comma >> y)
                        {
                            Point p;
                            p.x = x;
                            p.y = y;
                            points.push_back(p);
                        }
                        string fill = child->Attribute("fill");
                        Color color = parse_color(fill);
                        group_elements.push_back(new polygon(color, points));
                        svg_elements.back()->rotate(transform_origin, rotate);
                        svg_elements.back()->translate(translate);
                        svg_elements.back()->scale(transform_origin, scale);
                    }
                    group_child = group_child->NextSiblingElement();
                }
                svg_elements.push_back(new Group(group_elements, id));
                svg_elements.back()->rotate(transform_origin, rotate);
                svg_elements.back()->translate(translate);
                svg_elements.back()->scale(transform_origin, scale);
            }
            // Avançar para o próximo child node
            child = child->NextSiblingElement();
        }
    }
}