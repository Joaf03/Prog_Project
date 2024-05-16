//! @file shape.hpp
#ifndef __svg_SVGElements_hpp__
#define __svg_SVGElements_hpp__

#include "Color.hpp"
#include "Point.hpp"
#include "PNGImage.hpp"
#include <vector>

namespace svg
{
    //! since we are dealing with pixels, the values of all transformations will be integers and not float types, since we cannot
    //! move pixels by a decimal value
    //! we implemented the translate, rotate and scale functions

    class SVGElement
    {

    public:
        SVGElement();
        virtual ~SVGElement();
        virtual void draw(PNGImage &img) const = 0;
        virtual void translate(const Point &dir) = 0; //! the direction it will move
        virtual void rotate(const Point &center, int angle) = 0; //!the center of rotation and the angle of rotation
        virtual void scale(const Point &center, int factor) = 0; //! sx and sy represent the scale factors in both x and y
        const std::string &getId() const {return id_;}
        virtual std::string getType() const = 0;

    protected:
        Color fill_;
        std::string id_;
    };


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
        void translate(const Point &dir) override;
        void rotate(const Point &origin, int degrees) override;
        void scale(const Point &origin, int factor) override;
        std::string getType() const override {return "Ellipse";}

    protected://change from private to protected since we are likely to use these attributes again
        Color fill;
        Point center;
        Point radius;
    };

//!Now circle will be a subclass of the ellipse class
//!we can view circle as a specific case of an ellipse, which has only one radius, not two unlike the ellipse
    class Circle : public Ellipse{
    public:
        Circle (const Color &fill, const Point &center, const Point &radius);//!circle will share fill, center, and radius with the ellipse
        void draw(PNGImage &img) const override;
        void translate(const Point &dir) override;
        void rotate(const Point &origin, int degrees) override;
        void scale(const Point &origin, int factor) override;
        std::string getType() const override {return "Circle";}

//!in this case is not necessary to declare fill, radius and center again since ellipse is the "super class" and
//!circle will natively keep those variables, so there is no need to declare them again
//!also something to note, the variables x and y are used in struct Point, therefore we use for example
//!Point &radius to show the radius of the circle
};

    //!in this case we know that polyline consists of a sequence of points, therefore we can use a vector
    //!to store those points like: vector<Point> &points, a vector of type Point, which contains a number of points
    class polyline : public SVGElement{
        public:
            polyline (const Color &fill, const std::vector<Point> &points);
            void draw(PNGImage &img) const override;
            void translate(const Point &dir) override;
            void rotate(const Point &origin, int degrees) override;
            void scale(const Point &origin, int factor) override;
            std::string getType() const override {return "polyline";}
        protected:
            Color fill;
            std::vector<Point> points;//!we declare the vector of points of type Point
    };

    class line : public polyline{
        public:
            line(const Point &start, const Point &end, const Color &fill);
            void draw(PNGImage &img) const override;//!we again override the draw function
            void translate(const Point &dir) override;
            void rotate(const Point &origin, int degrees) override;
            void scale(const Point &origin, int factor) override;
            std::string getType() const override {return "line";}
        protected:
            Point start;//!the starting point with x1 and y1
            Point end;//!the end point with x2 and y2
            Color fill; //!the stroke
    };

    //! polygon class a subclass of SVGElement
    //! the polygon subclass will have a sequence of points : vector<Points> points which are separated by a comma
    //! and will also have a stroke color, the fill parameter
    class polygon : public SVGElement{
        public:
            polygon(const Color &fill, const std::vector<Point> &points);
            void draw(PNGImage &img) const override;
            void translate(const Point &dir) override;
            void rotate(const Point &origin, int degrees) override;
            void scale(const Point &origin, int factor) override;
            std::string getType() const override {return "polygon";}
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
            rect(const Color &fill,const Point &upper_left_corner, const int &width, const int &height);
            void draw(PNGImage &img) const override;
            void translate(const Point &dir) override;
            void rotate(const Point &origin, int degrees) override;
            void scale(const Point &origin, int factor) override;
            std::string getType() const override {return "rect";}

        protected:

            Point upper_left_corner;
            int width;
            int height;
    };

    //!now we do the group class
    //!this will have a vector os elements and a string id
    //!the draw function which draws in the png file
    //!then the translate, rotate and scale functions, which are similar to the functions
    //!in the SVGElement class, Group is a subclass of SVGElement

    class Group : public SVGElement{
    public:
        Group(const std::vector<SVGElement *>&elements,const std::string &id);
        void draw(PNGImage &img) const override;
        void translate(const Point &dir) override; //! the direction of the translation, which will be of type point, an x and y value
        void rotate(const Point &origin, int degrees) override; //!the origin of rotation and the degrees of rotation
        void scale(const Point &origin, int factor) override; //!the origin of the scale and the factor which will be s int value
        std::string getType() const override {return "Group";}
    protected:
        const std::vector<SVGElement *> &elements;
        const std::string &id_;
    };
};
#endif

