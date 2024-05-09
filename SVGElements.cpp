#include "SVGElements.hpp"
#include <vector>//! included vector

namespace svg
{
    // These must be defined!
    SVGElement::SVGElement() {}
    SVGElement::~SVGElement() {}

    // Ellipse (initial code provided)
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
    // @todo provide the implementation of SVGElement derived classes
    
    //! Circle
    //!knowing circle is a subclass of ellipse, circle
    //!will have again a radius, center and a fill variable
    //! and will have : Ellipse(fill,center,radius), since these attributes come from the Ellipse
    Circle::Circle(const Color &fill,
                  const Point &center,
                  const Point &radius)
                  : Ellipse(fill,center,radius){}
    void Circle::draw(PNGImage &img) const{
        img.draw_ellipse(center,radius,fill);//! and the draw function
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
                       :fill(fill), points(points){}
    void polyline::draw(PNGImage &img) const{
        for (size_t i = 0; i < points.size() - 1; i++ ){
            img.draw_line(points[i],points[i+1],fill); //! the function draw to each point
        }
    }

    //! line implementation
    //! the line will contain a start and end point
    //! we can create a temporary vector of points to store the start and end points
    //! after that we make the draw fucntion, which will have as arguments the start and end point and the fill

    line::line(const Color &fill,
               const Point &start,
               const Point &end)
               :polyline(fill,std::vector<Point> {start,end}){}

    void line::draw(PNGImage &img) const{
        img.draw_line(start,end,fill);
    }

    //! polygon
    //! will have the fill stroke and a vector of points
    //! and the function draw, here the for loop won't be needed like the polyline
    //! because the draw_polygon class already takes a vector of points as a argument, unlike the draw_line function

    polygon::polygon(const Color &fill,
                     const std::vector<Point> &points)
                     :fill(fill),points(points){}
    void polygon::draw(PNGImage &img) const{
        img.draw_polygon(points,fill);
    }

    //!rectangle

    rect::rect(const Point &upper_left_corner, 
               const int &width, 
               const int &height)
               :polygon(fill, std::vector<Point>{
                upper_left_corner,
                Point(upper_left_corner.x + width, upper_left_corner.y), 
                Point(upper_left_corner.x + width, upper_left_corner.y + height), 
                Point(upper_left_corner.x, upper_left_corner.y + height)
               }) {}
    void rect::draw(PNGImage &img) const{
        img.draw_polygon(points,fill);
    }
}
