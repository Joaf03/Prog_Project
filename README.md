# Prog_Project

João Afonso Santos-up202108805
Gustavo Lourenço-up202306578
Tomás Pereira-up202305651

<h1>Overview</h1> <br>
This project implements a comprehensive SVG to PNG conversion tool. The primary aim was to read SVG files, parse various SVG elements, apply transformations, and then render these elements into PNG images. The key transformations supported include translation, rotation, and scaling of the SVG elements, enabling manipulation and accurate rendering of the graphical data.

<h1>Implementation Summary</h1> <br>
To achieve the objectives outlined in the project guide, we created a robust framework for handling SVG elements. The core classes implemented include SVGElement and its derived classes: Ellipse, Circle, Polyline, Line, Polygon, Rect, and Group. Each class provides implementations for essential transformations such as translate, rotate, and scale. This ensures that any element can be moved, rotated, or resized with respect to a given point, allowing for flexible manipulation.

For parsing SVG files, we utilized the TinyXML2 library to read and interpret the XML structure of SVG documents. The readSVG function handles this process, extracting relevant attributes and creating corresponding SVG element objects. These objects are then stored in a vector for further processing. The convert function integrates all these components, translating SVG elements into their PNG representations by leveraging the PNGImage class, which provides drawing functionalities like draw_ellipse, draw_line, and draw_polygon.

This project showcases the integration of geometric transformations with graphical rendering, providing a versatile tool for converting vector graphics into raster images.
