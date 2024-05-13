//! @file shape.hpp
#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "Point.hpp"
#include "PNGImage.hpp"
#include <vector>

namespace svg
{
    class SVGElement
    {

    public:
        SVGElement();
        virtual ~SVGElement();
        virtual void draw(PNGImage &img) const = 0;
    };

    // Declaration of namespace functions
    // readSVG -> implement it in readSVG.cpp
    // convert -> already given (DO NOT CHANGE) in convert.cpp

    void readSVG(const std::string &svg_file,
                 Point &dimensions,
                 std::vector<SVGElement *> &svg_elements);
    void convert(const std::string &svg_file,
                 const std::string &png_file);

    class Ellipse : public SVGElement
    {
    public:
        Ellipse(const Color &fill, const Point &center, const Point &radius);
        void draw(PNGImage &img) const override;

    protected://change from private to protected since we are likely to use these attributes again
        Color fill;
        Point center;
        Point radius;
    };

//!Now circle will be a subclass of the ellipse class
//!we can view circle as a specific case of an ellipse, which has only one radius, not two unlike the ellipse
    class Circle : public Ellipse{
    public:
        Circle (const Color &fill, const Point &center, const Point &radius) 
            : Ellipse(fill,center,radius){}//!circle will share fill, center, and radius with the ellipse
        void draw(PNGImage &img) const override{}

//!in this case is not necessary to declare fill, radius and center again since ellipse is the "super class" and
//!circle will natively keep those variables, so there is no need to declare them again
//!also something to note, the variables x and y are used in struct Point, therefore we use for example
//!Point &radius to show the radius of the circle
    };

    //!in this case we know that polyline consists of a sequence of points, therefore we can use a vector
    //!to store those points like: vector<Point> &points, a vector of type Point, which contains a number of points
    class polyline : public SVGElement{
        public:
            polyline (const Color &fill, const std::vector<Point> &points) {}
            void draw(PNGImage &img) const override {}
        protected:
            Color fill;
            std::vector<Point> points;//!we declare the vector of points of type Point
    };

    class line : public polyline{
        public:
        line(const Point &start, const Point &end, const Color &fill) : 
                                            polyline(fill, std::vector<Point> {start,end}), start(start), end(end), fill(fill){}
        void draw(PNGImage &img) const override {}//!we again override the draw function


        protected:
            Color fill;//!the stroke
            Point start;//!the starting point with x1 and y1
            Point end;//!the end point with x2 and y2

    };

    //! polygon class a subclass of SVGElement
    //! the polygon subclass will have a sequence of points : vector<Points> points which are separated by a comma
    //! and will also have a stroke color, the fill parameter
    class polygon : public SVGElement{
        public:
            polygon(const Color &fill, const std::vector<Point> &points) :
                                        fill(fill), points(points){}
            void draw(PNGImage &img) const override{}
        protected:
            Color fill;
            std::vector<Point> points;
    };

    //! the class rect which is a subclass of polygon
    //! in the case of the rect class we use the upper point to get the other 4 points of the rectangle
    //! for example the upper right point is the x coordinate of the upper left + the width of the rectangle
    //! and the y coordinate will remain the same since both points have the same height
    //! therefore the vector of points will consist of the upper left point and all the points we can get by summing the
    //! width and height to where it is needed

    class rect : public polygon{
        public:
            rect(const Color &fill,const Point &upper_left_corner, const int &width, const int &height) :
                                    polygon(fill, std::vector<Point> {upper_left_corner, Point(upper_left_corner.x + width, upper_left_corner.y),
                                                                         Point(upper_left_corner.x + width, upper_left_corner.y + height),
                                                                         Point(upper_left_corner.x, upper_left_corner.y + height)}),
                                                                         upper_left_corner(upper_left_corner),
                                                                         width(width),
                                                                         height(height) {}
            void draw(PNGImage &img) const override{}
        protected:

            Point upper_left_corner;
            int width;
            int height;
    };

};
#endif
