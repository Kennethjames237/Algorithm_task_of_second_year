#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <stdexcept>
#include <string>

const double MATH_PI = 3.141592653589793;

class Shape {
public:
    virtual double computeArea() const = 0;
    virtual double computePerimeter() const = 0;
    virtual std::string getDescription() const = 0;
    virtual ~Shape() {}
};

class Circle : public Shape {
    double radius;
public:
    Circle(double r) : radius(r) {
        if (r <= 0) throw std::invalid_argument("Radius must be positive");
    }

    double computeArea() const override {
        return MATH_PI * radius * radius;
    }

    double computePerimeter() const override {
        return 2 * MATH_PI * radius;
    }

    std::string getDescription() const override {
        return "Circle (r=" + std::to_string(radius) + ")";
    }
};

class Rectangle : public Shape {
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) {
        if (w <= 0 || h <= 0) throw std::invalid_argument("Dimensions must be positive");
    }

    double computeArea() const override {
        return width * height;
    }

    double computePerimeter() const override {
        return 2 * (width + height);
    }

    std::string getDescription() const override {
        return "Rectangle (" + std::to_string(width) + "x" + std::to_string(height) + ")";
    }
};

class Triangle : public Shape {
protected:
    double a, b, c;
public:
    Triangle(double s1, double s2, double s3) : a(s1), b(s2), c(s3) {
        if (!isValid()) throw std::invalid_argument("Invalid triangle sides");
    }

    bool isValid() const {
        return (a + b > c) && (a + c > b) && (b + c > a) &&
            (a > 0) && (b > 0) && (c > 0);
    }

    double computeArea() const override {
        double s = (a + b + c) / 2;
        return std::sqrt(s * (s - a) * (s - b) * (s - c));
    }

    double computePerimeter() const override {
        return a + b + c;
    }

    std::string getDescription() const override {
        return "Triangle (" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ")";
    }
};

void printShapeDetails(const Shape& shape) {
    std::cout << "Shape: " << shape.getDescription() << std::endl;
    std::cout << "  Area: " << shape.computeArea() << std::endl;
    std::cout << "  Perimeter: " << shape.computePerimeter() << std::endl;
}

int main() {
    try {
        std::vector<std::unique_ptr<Shape>> shapes;
        shapes.push_back(std::make_unique<Circle>(5.0));
        shapes.push_back(std::make_unique<Rectangle>(4.0, 6.0));
        shapes.push_back(std::make_unique<Triangle>(3.0, 4.0, 5.0));

        for (const auto& shape : shapes) {
            printShapeDetails(*shape);
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}