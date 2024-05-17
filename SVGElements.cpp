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
        : fill(fill), center(center), radius(radius){};

    void Ellipse::draw(PNGImage &img) const
    {
        img.draw_ellipse(center, radius, fill);
    }

    void Ellipse::translate(const Point &dir) {
        center = center.translate(dir);
    }
    void Ellipse::rotate(const Point &origin, int degrees) {
        center = center.rotate(origin,degrees);
    }

    void Ellipse::scale(const Point &origin, int factor) {
        center = center.scale(origin, factor);
        radius = {radius.x * factor, radius.y * factor};
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
        img.draw_ellipse(center,radius,fill); //! and the draw function
    }

    void Circle::translate(const Point &dir) {
        center = center.translate(dir);
    }
    void Circle::rotate(const Point &origin, int degrees) {
        center = center.rotate(origin, degrees);
    }
    void Circle::scale(const Point &origin, int factor) {
        center = center.scale(origin, factor);
        radius = {radius.x * factor, radius.y * factor};
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
        for (size_t i = 0; i < points.size() - 1; i++) {
            Point start = points[i];
            Point end = points[i + 1];
            img.draw_line(start, end, fill);
        }
    }

    void polyline::translate(const Point &dir) {
        for(Point &point : points){
            point = point.translate(dir);
        }
    }
    void polyline::rotate(const Point &origin, int degrees) {
        for (Point &point : points)
        {
            point = point.rotate(origin,degrees);
        }
    }

    //! we use the scale function already given for the polyline scale
    void polyline::scale(const Point &origin, int factor) {
        //! Scale each point of the polyline
        for (Point &point : points){
            point = point.scale(origin,factor);
        }
    }

    //! line implementation
    //! the line will contain a start and end point
    //! we can create a temporary vector of points to store the start and end points
    //! after that we make the draw function, which will have as arguments the start and end point and the fill

    line::line(const Point &start,
               const Point &end,
               const Color &fill)
               :start(start), end(end), fill(fill)
    {
    }
    void line::draw(PNGImage &img) const{
        img.draw_line(start,end,fill);
    }
    
    void line::translate(const Point &dir) {
        start = start.translate(dir);
        end = end.translate(dir);
    }
    void line::rotate(const Point &origin, int degrees) {
        start = start.rotate(origin,degrees);
        end = end.rotate(origin,degrees);
    }
    void line::scale(const Point &origin, int factor) {
        start =start.scale(origin,factor);
        end = end.scale(origin,factor);
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
            point = point.translate(dir);
        }
    }
    void polygon::rotate(const Point &origin, int degrees) {
        for (Point &point : points){
            point = point.rotate(origin,degrees);
        }
    }
    void polygon::scale(const Point &origin, int factor) {
        for(Point &point : points){
            point = point.scale(origin,factor);
        }
    }

    //!rectangle implementation
    //!we subtract 1 because, without it the rectangle will have 1 more pixel

    rect::rect(const Color &fill, const Point &upper_left_corner, 
               const int &width, 
               const int &height)
               :polygon(fill, std::vector<Point>{
                upper_left_corner,
                {upper_left_corner.x + width - 1, upper_left_corner.y}, 
                {upper_left_corner.x + width - 1, upper_left_corner.y + height - 1}, 
                {upper_left_corner.x, upper_left_corner.y + height - 1}
               }) {};
    void rect::draw(PNGImage &img) const{
        img.draw_polygon(points,fill);
    }
    //! we can do this way in the rectangle since it will go over a vector of points
    //! just like in the case of the polygon
    void rect::translate(const Point &dir) {
        for(Point &point : points){
            point = point.translate(dir);
        }
    }
    void rect::rotate(const Point &origin, int degrees) {
        for (Point &point : points){
            point = point.rotate(origin,degrees);  
        }
    }
    void rect::scale(const Point &origin, int factor) {
        for(Point &point : points){
            point = point.scale(origin,factor);
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
