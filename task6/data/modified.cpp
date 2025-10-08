#include <iostream>
#include <vector>
#include <cmath>
#include <memory>
#include <stdexcept>
#include <string>
#include <iomanip>  // Nuovo include

const double PI = 3.141592653589793;  // Nome cambiato

class Geometry {  // Classe base rinominata
public:
    virtual double area() const = 0;  // Metodo rinominato
    virtual double perimeter() const = 0;  // Metodo rinominato
    virtual std::string description() const = 0;  // Metodo rinominato
    virtual ~Geometry() {}
};

class Circle : public Geometry {
    double r;  // Variabile rinominata
public:
    Circle(double radius) : r(radius) {  // Parametro rinominato
        if (radius <= 0) throw std::runtime_error("Radius must be >0");  // Messaggio cambiato
    }

    double area() const override {
        return PI * r * r;
    }

    double perimeter() const override {
        return 2 * PI * r;
    }

    std::string description() const override {
        return "Circle [radius=" + std::to_string(r) + "]";  // Formato cambiato
    }
};

class Rectangle : public Geometry {
protected:
    double w, h;  // Variabili rinominate
public:
    Rectangle(double width, double height) : w(width), h(height) {
        if (width <= 0 || height <= 0)
            throw std::runtime_error("Dimensions must be >0");  // Messaggio modificato
    }

    double area() const override {
        return w * h;
    }

    double perimeter() const override {
        return 2 * (w + h);
    }

    std::string description() const override {
        return "Rectangle [" + std::to_string(w) + "x" + std::to_string(h) + "]";  // Formato cambiato
    }
};

class Triangle : public Geometry {
protected:
    double side1, side2, side3;  // Variabili rinominate
public:
    Triangle(double s1, double s2, double s3) : side1(s1), side2(s2), side3(s3) {
        if (!is_valid())  // Metodo rinominato
            throw std::runtime_error("Invalid triangle");  // Messaggio modificato
    }

    bool is_valid() const {  // Rinominato
        return (side1 + side2 > side3) &&
            (side1 + side3 > side2) &&
            (side2 + side3 > side1) &&
            (side1 > 0) && (side2 > 0) && (side3 > 0);
    }

    double area() const override {
        double s = (side1 + side2 + side3) / 2;
        return std::sqrt(s * (s - side1) * (s - side2) * (s - side3));
    }

    double perimeter() const override {
        return side1 + side2 + side3;
    }

    std::string description() const override {
        return "Triangle [" + std::to_string(side1) + "," + std::to_string(side2) + "," + std::to_string(side3) + "]";  // Formato cambiato
    }
};

// NUOVA CLASSE AGGIUNTA
class Square : public Rectangle {
public:
    Square(double side) : Rectangle(side, side) {}

    std::string description() const override {
        return "Square [side=" + std::to_string(w) + "]";
    }
};

void printGeometryDetails(const Geometry& geo) {  // Funzione rinominata
    std::cout << "=== GEOMETRY DETAILS ===" << std::endl;
    std::cout << "Type: " << geo.description() << std::endl;
    std::cout << "Area: " << std::fixed << std::setprecision(2) << geo.area() << std::endl;  // Formattazione migliorata
    std::cout << "Perimeter: " << geo.perimeter() << std::endl;
    std::cout << "=========================" << std::endl;
}

// NUOVA FUNZIONE
double calculateTotalArea(const std::vector<std::unique_ptr<Geometry>>& geometries) {
    double total = 0.0;
    for (const auto& geo : geometries) {
        total += geo->area();
    }
    return total;
}

int main() {
    try {
        std::vector<std::unique_ptr<Geometry>> shapes;  // Tipo cambiato
        shapes.push_back(std::make_unique<Circle>(5.5));  // Valore modificato
        shapes.push_back(std::make_unique<Rectangle>(4.0, 6.0));
        shapes.push_back(std::make_unique<Triangle>(3.0, 4.0, 5.0));
        shapes.push_back(std::make_unique<Square>(7.0));  // Nuova forma aggiunta

        // Utilizzo nuova funzione
        std::cout << "TOTAL AREA: " << calculateTotalArea(shapes) << std::endl;

        for (const auto& shape : shapes) {
            printGeometryDetails(*shape);  // Funzione cambiata
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;  // Messaggio modificato
        return 1;
    }
}