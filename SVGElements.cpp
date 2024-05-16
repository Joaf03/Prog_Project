#include "SVGElements.hpp"
#include <vector>//! included vector
#include <cmath>

namespace svg
{

    // These must be defined!
    SVGElement::SVGElement()  {} //!the constructor
    SVGElement::~SVGElement() {} //! the destructor



    //! now we implement the translate, rotate and scale functions
    //! in all classes

    // Ellipse (initial code provided)
    //! added the translate, scale and rotate functions
    //! in this functions the center and radius of the ellipse will change
    //! for example the radius only on the scale function
    //! and the center always changes

    Ellipse::Ellipse(const Color &fill,
                     const Point &center,
                     const Point &radius)
        : fill(fill), center(center), radius(radius)
    {
    }
    void Ellipse::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
    }

    void Ellipse::translate(const Point &dir) {
        center.x += dir.x;
        center.y += dir.y; //the center will move
    }
    void Ellipse::rotate(const Point &origin, int degrees) {
        //Translate to the origin, rotate, then translate back
        float radianAngle = degrees * 3.1415926 / 180.0; // Convert degrees to radians
        float cosTheta = cos(radianAngle);
        float sinTheta = sin(radianAngle);
        
        // Translate center of ellipse to origin
        Point translatedCenter = {this->center.x - origin.x, this->center.y - origin.y};

        // Perform rotation
        float rotatedX = translatedCenter.x * cosTheta - translatedCenter.y * sinTheta;
        float rotatedY = translatedCenter.x * sinTheta + translatedCenter.y * cosTheta;

        // Translate back
        this->center.x = rotatedX + origin.x;
        this->center.y = rotatedY + origin.y;
    }

    void Ellipse::scale(const Point &origin, int factor) {
        radius.x *= factor;
        radius.y *= factor;
    }
    
    //! Circle implementation

    //!knowing circle is a subclass of ellipse, circle
    //!will have again a radius, center and a fill variable
    //! and will have : Ellipse(fill,center,radius), since these attributes come from the Ellipse
    //! the same comments about the translate, rotate and scale functions
    //! will apply here, since the circle is a special case of the ellipse
    Circle::Circle(const Color &fill,
                  const Point &center,
                  const Point &radius)
                  : Ellipse(fill,center,radius){};
    void Circle::draw(PNGImage &img) const{
        img.draw_ellipse(center,radius,fill);//! and the draw function
    }

    void Circle::translate(const Point &dir) {
        center.x += dir.x;
        center.y += dir.y; 
    }
    void Circle::rotate(const Point &origin, int degrees) {
        center = center.rotate(origin, degrees);
    }
    void Circle::scale(const Point &origin, int factor) {
        radius.x *= factor;
        radius.y *= factor;
    }

    //! Polyline implementation

    //! the implementation is similar as circle
    //! we have the fill and a vector of points
    //! however in the draw function we need to
    //! draw a line between a series of points
    //!therefore we use a for loop to iterate between all the points
    //! and draw a line between each one

    polyline::polyline(const Color &fill,
                       const std::vector<Point> &points)
                       :fill(fill), points(points){};

    void polyline::draw(PNGImage &img) const{
        for (size_t i = 0; i < points.size() - 1; i++ ){
            img.draw_line(points[i],points[i+1],fill); //! the function draw to each point
        }
    }

    void polyline::translate(const Point &dir) {
        for(Point &point : points){
            point.x += dir.x;
            point.y += dir.y;
        }
    }
    void polyline::rotate(const Point &origin, int degrees) {

        // Translate each point of the polyline to the origin, rotate, and then translate back
        float radianAngle = degrees * 3.1415926 / 180.0; // Convert degrees to radians
        float cosTheta = cos(radianAngle);
        float sinTheta = sin(radianAngle);

        for (Point &point : points)
        {
            // Translate to the origin
            float translatedX = point.x - origin.x;
            float translatedY = point.y - origin.y;

            // Perform rotation
            float rotatedX = translatedX * cosTheta - translatedY * sinTheta;
            float rotatedY = translatedX * sinTheta + translatedY * cosTheta;

            // Translate back
            point.x = rotatedX + origin.x;
            point.y = rotatedY + origin.y;
        }
    }

    //! int the scale operation we get the distance of the point form the origin along the axis by (point- origin)
    //! then we multiply it by the factor
    //! and then sum that to the origin
    void polyline::scale(const Point &origin, int factor) {
        //! Scale each point of the polyline
        for (Point &point : points){
            point.x *= factor; //origin.x + (point.x - origin.x) * factor;
            point.y *= factor; //origin.y + (point.y - origin.y) * factor;
        }
    }

    //! line implementation
    //! the line will contain a start and end point
    //! we can create a temporary vector of points to store the start and end points
    //! after that we make the draw fucntion, which will have as arguments the start and end point and the fill

    line::line(const Point &start,
               const Point &end,
               const Color &fill)
               :polyline(fill,std::vector<Point> {start,end}), start(start), end(end) {};

    void line::draw(PNGImage &img) const{
        img.draw_line(start,end,fill);
    }
    
    void line::translate(const Point &dir) {
        start.x += dir.x;
        start.y += dir.y;
        end.x += dir.x;
        end.y += dir.y;
    }
    void line::rotate(const Point &origin, int degrees) {
        
    }
    void line::scale(const Point &origin, int factor) {
        start.x = origin.x + (start.x - origin.x) * factor;
        start.y = origin.y + (start.y - origin.y) * factor;
        end.x = origin.x + (end.x - origin.x) * factor;
        end.y = origin.y + (end.y - origin.y) * factor;
    }

    //! polygon
    //! will have the fill stroke and a vector of points
    //! and the function draw, here the for loop won't be needed like the polyline
    //! because the draw_polygon class already takes a vector of points as a argument, unlike the draw_line function

    polygon::polygon(const Color &fill,
                     const std::vector<Point> &points)
                     :fill(fill),points(points){};
    void polygon::draw(PNGImage &img) const{
        img.draw_polygon(points,fill);
    }
    
    void polygon::translate(const Point &dir) {
        for(Point &point : points){
            point.x += dir.x;
            point.y += dir.y;
        }
    }
    void polygon::rotate(const Point &origin, int degrees) {

        // Translate each point of the polygon to the origin, rotate, and then translate back

        float radianAngle = degrees * 3.1415926 / 180.0; // Convert degrees to radians
        float cosTheta = cos(radianAngle);
        float sinTheta = sin(radianAngle);

        for (Point &point : points)
        {
            // Translate to the origin
            float translatedX = point.x - origin.x;
            float translatedY = point.y - origin.y;

            // Perform rotation
            float rotatedX = translatedX * cosTheta - translatedY * sinTheta;
            float rotatedY = translatedX * sinTheta + translatedY * cosTheta;

            // Translate back
            point.x = rotatedX + origin.x;
            point.y = rotatedY + origin.y;
        }
    }
    void polygon::scale(const Point &origin, int factor) {
        for(Point &point : points){
            point.x *= factor;//origin.x + (point.x - origin.x) * factor;
            point.y *= factor; //origin.y + (point.y - origin.y) * factor;
        }
    }

    //!rectangle implementation

    rect::rect(const Color &fill, const Point &upper_left_corner, 
               const int &width, 
               const int &height)
               :polygon(fill, std::vector<Point>{
                upper_left_corner,
                {upper_left_corner.x + width, upper_left_corner.y}, 
                {upper_left_corner.x + width, upper_left_corner.y + height}, 
                {upper_left_corner.x, upper_left_corner.y + height}
               }) {};
    void rect::draw(PNGImage &img) const{
        img.draw_polygon(points,fill);
    }
    //! we can do this way in the rectangle since it will go over a vector of points
    //! just like in the case of the polygon
    void rect::translate(const Point &dir) {
        upper_left_corner.x += dir.x;
        upper_left_corner.y += dir.y;
        for(auto &point : points){
            point.x += dir.x;
            point.y += dir.y;
        }
    }
    void rect::rotate(const Point &origin, int degrees) {
        float radians = degrees * 3.1415926 / 180;
        for (auto &point : points){
            float x_new = origin.x + (point.x - origin.x) * cos(radians) - (point.y - origin.y) * sin(radians);
            float y_new = origin.y + (point.x - origin.x) * sin(radians) + (point.y - origin.y) * cos(radians);
            point.x = x_new;
            point.y = y_new;  
        }
    }
    void rect::scale(const Point &origin, int factor) {
        for(auto &point : points){
            point.x = upper_left_corner.x + (point.x - upper_left_corner.x) * factor;
            point.y = upper_left_corner.y + (point.y - upper_left_corner.y) * factor;
        }
    }


    Group::Group(const std::vector<SVGElement*> &elements, const std::string &id)
        : elements(elements), id_(id)
    {
    };

    void Group::draw(PNGImage &img) const{
        for(const SVGElement *element: elements){
            element->draw(img);
        }
    }

    void Group::translate(const Point &dir) {
        //!Translate each element in the group
        for(SVGElement *element: elements){
            element->translate(dir);
        }
    }

    void Group::rotate(const Point &origin, int degrees) {
        //!Rotate each element in the group
        for(SVGElement *element: elements){
            element->rotate(origin, degrees);
        }
    }
    void Group::scale(const Point &origin, int factor) {
        //!Scale each element in the group
        for(SVGElement *element: elements){
            element->scale(origin, factor);
        }
    }
    
}
