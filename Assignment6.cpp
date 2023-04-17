// PHYS 30762 Programming in C++
// Assignment 6
// Kiran Jonathan 23/4/21

/*  
Classes for 3D and 2D shapes with member functions demonstrated. All formulae 
are exact unless stated otherwise. Unparameterised constructors simply set all 
lengths to zero - not demonstrated for conciseness (except for prism as shape_2d
pointer for base shape required specific handling which is demonstrated). Cuboid 
does not derive from prism for simplicity of data access and method functions.
3D shapes are assumed to have no perimeter, perimeter function was only included
to calculate the surface area of a prism. 
*/

#include<iostream>
#include<iomanip>
#include<cmath>
#include<vector>

class shape
{
    public:
        virtual ~shape() {};
        // Returns 2D area for 2D shapes and surface area for 3D shapes
        virtual double area() const = 0;
        // Returns volume for 3D shapes and 0 for 2D shapes
        virtual double volume() const = 0;
        // Returns perimeter for 2D shapes and 0 for 3D shapes
        virtual double perimeter() const = 0;
        // Prints shape's type and dimensions to std::cout
        virtual void show() const = 0;
};

class shape_2d : public shape 
{
    public:
        virtual ~shape_2d() {};
        double volume() const 
        {
            return 0; // 2D shapes have zero volume
        }
};

class rectangle : public shape_2d
{
    protected:
        double length_a{};
        double length_b{};
    public:
        rectangle() = default;
        rectangle(double side_a, double side_b) 
        {
            length_a = side_a;
            length_b = side_b;
        }
        ~rectangle(){};
        double area() const
        {
            return length_a * length_b;
        }
        double perimeter() const
        {
            return (2.0 * length_a) + (2.0 * length_b);
        }
        void show() const
        {
            std::cout << "Rectangle with side lengths " << length_a << " and " << length_b << std::endl;
        }
};

class square : public rectangle
{
    public:
        square() = default;
        square(double side_length) : rectangle(side_length, side_length) {};
        ~square(){};
        void show() const
        {
            std::cout << "Square with side length " << length_a << std::endl;
        }
};

class ellipse : public shape_2d
{
    protected:
        double axis_a{};
        double axis_b{};

    public:
        ellipse() = default;
        ellipse(double ax_a, double ax_b)
        {
            axis_a = ax_a;
            axis_b = ax_b;
        }
        ~ellipse(){};
        double area() const
        {
            return M_PI * axis_a * axis_b;
        }
        double perimeter() const
        {
            double h{ std::pow(axis_a - axis_b, 2) / std::pow(axis_a + axis_b, 2) };
            return M_PI * (axis_a + axis_b) * ( 1.0 + ( (3.0 * h) / (10.0 + std::pow(4.0 - 3.0 * h, 0.5)) ) ); 
            // Ramanujan's 2nd approximation: https://en.wikipedia.org/wiki/Ellipse#Circumference
        }
        void show() const
        {
            std::cout << "Ellipse with axes " << axis_a << " and " << axis_b << std::endl;
        }
};

class circle : public ellipse
{
    public:
        circle() = default;
        circle(double radius) : ellipse(radius, radius) {};
        ~circle(){};
        void show() const
        {
            std::cout << "Circle with radius " << axis_a << std::endl;
        }
};

class shape_3d : public shape 
{
    public:
        virtual ~shape_3d() {};
        double perimeter() const 
        {
            return 0; // 3D shapes perimeters are not calculated - assumed to be zero
        }
};

class cuboid : public shape_3d
{
    protected:
        double length_a{};
        double length_b{};
        double length_c{};
    public:
        cuboid() = default;
        cuboid(double side_a, double side_b, double side_c)
        { 
            length_a = side_a;
            length_b = side_b;
            length_c = side_c;
        }
        ~cuboid(){};
        double area() const
        {
            return 2.0 * ((length_a * length_b) + (length_a * length_c) + (length_b * length_c));
        }
        double volume() const
        {
            return length_a * length_b * length_c;
        }
        void show() const
        {
            std::cout << "Cuboid with side lengths " << length_a << " and " << length_b << " and " << length_c << std::endl;
        }
};

class cube : public cuboid
{
    public:
        cube() = default;
        cube(double side_length) : cuboid(side_length, side_length, side_length) {};
        ~cube(){};
        void show() const
        {
            std::cout << "Cube with side length " << length_a << std::endl;
        }
};

class ellipsoid : public shape_3d
{
    protected:
        double axis_a{};
        double axis_b{};
        double axis_c{};
    public:
        ellipsoid() = default;
        ellipsoid(double ax_a, double ax_b, double ax_c) 
        {
            axis_a = ax_a;
            axis_b = ax_b;
            axis_c = ax_c;
        }
        ~ellipsoid(){};
        double area() const
        {
            double effective_r_squared{ std::pow( (std::pow(axis_a * axis_b, 1.6) + std::pow(axis_a * axis_c, 1.6) + std::pow(axis_b * axis_c, 1.6)) / 3.0, 1/1.6) };
            return 4.0 * M_PI * effective_r_squared; 
            // Thomsen's approximation - p=1.6 chosen for computational efficiency and tolerable ~1% error: https://en.wikipedia.org/wiki/Ellipsoid#Surface_area
        }
        double volume() const
        {
            return (4.0/3.0) * M_PI * axis_a * axis_b * axis_c; 
        }
        void show() const
        {
            std::cout << "Ellipsoid with axes " << axis_a << " and " << axis_b << " and " << axis_c << std::endl;
        }
};

class sphere : public ellipsoid
{
    public:
        sphere() = default;
        sphere(double radius) : ellipsoid(radius, radius, radius) {};
        ~sphere(){};
        void show() const
        {
            std::cout << "Sphere with radius " << axis_a << std::endl;
        }
};

class prism : public shape_3d
{
    protected:
        shape_2d *base = nullptr; // Checks for nullptr in functions to avoid method calls on it
        double depth = 0;
        std::string prism_type{"Prism"}; // Used in show() to distinguish derived classes
    public:
        prism() = default;
        prism(shape_2d *base_shape, double length)
        {
            base = base_shape;
            depth = length;
        }
        ~prism()
        {
            delete base; 
        }
        double area() const
        {
            if(base == nullptr) return 0;
            return (2.0 * base->area()) + (depth * base->perimeter()); 
            // Sum of area of two bases and lateral face(s)
        }
        double volume() const
        {
            if(base == nullptr) return 0;
            return base->area() * depth;
        }
        void show() const
        {
            std::cout << "Prism with depth " << depth << " and base shape: ";
            if(base != nullptr) base->show();
            else std::cout << "No base shape present \n";
        }
};


int main()
{  
    std::cout << std::setprecision(3);

    // Demonstrate polymorphic vector and create all test shapes - dynamic memory allocation
    std::cout << "Creating test shapes... \n\n";
    std::vector<shape*> test_shapes;
    test_shapes.push_back(new rectangle{1,4}); 
    test_shapes.push_back(new square{3}); 
    test_shapes.push_back(new ellipse{1,5}); 
    test_shapes.push_back(new circle{3}); 
    test_shapes.push_back(new cuboid{2,3,5});
    test_shapes.push_back(new cube{2});
    test_shapes.push_back(new ellipsoid{2,4,7});
    test_shapes.push_back(new sphere{2}); 
    test_shapes.push_back(new prism{new ellipse{3,2}, 4});
    test_shapes.push_back(new prism{new circle{2}, 4});
    test_shapes.push_back(new prism); // Demonstrate handling of nullptr for default constructor
    std::cout << "Shape vector size: " << test_shapes.size() << std::endl << std::endl;

    // Print all shape data and free memory once done with the shape
    std::cout << "Printing all shape data: \n\n";
    for (auto shape_it{test_shapes.begin()}; shape_it < test_shapes.end(); shape_it++) {
        (*shape_it)->show();
        std::cout << "Perimeter: " << (*shape_it)->perimeter() << std::endl;
        std::cout << "Area: " << (*shape_it)->area() << std::endl;
        std::cout << "Volume: " << (*shape_it)->volume() << std::endl<< std::endl;
        delete *shape_it;
    }
    
    // Clear the vector
    std::cout << "Clearing shape vector... \n";
    test_shapes.clear();
    std::cout << "Shape vector size: " << test_shapes.size();

    return 0;
}