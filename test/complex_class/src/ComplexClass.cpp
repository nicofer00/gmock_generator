#include "ComplexClass.hpp"
#include <iostream>
#include <algorithm>

namespace TestNamespace {

// Constructors
ComplexClass::ComplexClass() : privateValue(0), privateString(""), protectedValue(0.0) {
    std::cout << "ComplexClass default constructor" << std::endl;
}

ComplexClass::ComplexClass(int value) : privateValue(value), privateString(""), protectedValue(0.0) {
    std::cout << "ComplexClass constructor with value: " << value << std::endl;
}

ComplexClass::ComplexClass(const std::string& str) : privateValue(0), privateString(str), protectedValue(0.0) {
    std::cout << "ComplexClass constructor with string: " << str << std::endl;
}

ComplexClass::ComplexClass(const ComplexClass& other) 
    : privateValue(other.privateValue), privateString(other.privateString), 
      privateVector(other.privateVector), protectedValue(other.protectedValue), 
      protectedMap(other.protectedMap) {
    std::cout << "ComplexClass copy constructor" << std::endl;
}

ComplexClass::ComplexClass(ComplexClass&& other) noexcept
    : privateValue(std::move(other.privateValue)), privateString(std::move(other.privateString)),
      privateVector(std::move(other.privateVector)), protectedValue(std::move(other.protectedValue)),
      protectedMap(std::move(other.protectedMap)) {
    std::cout << "ComplexClass move constructor" << std::endl;
}

// Destructor
ComplexClass::~ComplexClass() {
    std::cout << "ComplexClass destructor" << std::endl;
}

// Assignment operators
ComplexClass& ComplexClass::operator=(const ComplexClass& other) {
    if (this != &other) {
        privateValue = other.privateValue;
        privateString = other.privateString;
        privateVector = other.privateVector;
        protectedValue = other.protectedValue;
        protectedMap = other.protectedMap;
    }
    std::cout << "ComplexClass copy assignment" << std::endl;
    return *this;
}

ComplexClass& ComplexClass::operator=(ComplexClass&& other) {
    if (this != &other) {
        privateValue = std::move(other.privateValue);
        privateString = std::move(other.privateString);
        privateVector = std::move(other.privateVector);
        protectedValue = std::move(other.protectedValue);
        protectedMap = std::move(other.protectedMap);
    }
    std::cout << "ComplexClass move assignment" << std::endl;
    return *this;
}

// Basic member functions
int ComplexClass::getValue() const {
    return privateValue;
}

void ComplexClass::setValue(int value) {
    privateValue = value;
}

std::string ComplexClass::getString() const {
    return privateString;
}

void ComplexClass::setString(const std::string& str) {
    privateString = str;
}

// Virtual functions
void ComplexClass::virtualFunction() {
    std::cout << "ComplexClass virtualFunction called" << std::endl;
}

int ComplexClass::virtualFunctionWithReturn() const {
    return privateValue * 2;
}

// Static functions
int ComplexClass::staticFunction() {
    return 42;
}

std::string ComplexClass::staticStringFunction() {
    return "static string";
}

// Const member functions
bool ComplexClass::isPositive() const {
    return privateValue > 0;
}

size_t ComplexClass::getVectorSize() const {
    return privateVector.size();
}

// Reference and pointer functions
int& ComplexClass::getValueRef() {
    return privateValue;
}

const int& ComplexClass::getValueConstRef() const {
    return privateValue;
}

int* ComplexClass::getValuePtr() {
    return &privateValue;
}

const int* ComplexClass::getValueConstPtr() const {
    return &privateValue;
}

// Functions with default parameters
void ComplexClass::functionWithDefault(int param1, int param2, std::string param3) {
    std::cout << "functionWithDefault: " << param1 << ", " << param2 << ", " << param3 << std::endl;
}

// Functions with variadic parameters (std::initializer_list)
void ComplexClass::functionWithInitializerList(std::initializer_list<int> values) {
    std::cout << "functionWithInitializerList called with " << values.size() << " values" << std::endl;
    for (int value : values) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}

// Functions with function pointers
void ComplexClass::functionWithCallback(std::function<void(int)> callback) {
    if (callback) {
        callback(privateValue);
    }
}

// Free functions
int freeFunction(int param) {
    return param * 2;
}

std::string freeStringFunction(const std::string& input) {
    return "Processed: " + input;
}

void freeVoidFunction() {
    std::cout << "freeVoidFunction called" << std::endl;
}

} // namespace TestNamespace 