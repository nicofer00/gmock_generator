#ifndef COMPLEX_CLASS_HPP
#define COMPLEX_CLASS_HPP

#include <string>
#include <vector>
#include <map>
#include <functional>

namespace TestNamespace {

class ComplexClass {
private:
    int privateValue;
    std::string privateString;
    std::vector<int> privateVector;
    
protected:
    double protectedValue;
    std::map<std::string, int> protectedMap;
    
public:
    // Constructors
    ComplexClass();
    ComplexClass(int value);
    ComplexClass(const std::string& str);
    ComplexClass(const ComplexClass& other);
    ComplexClass(ComplexClass&& other) noexcept;
    
    // Destructor
    virtual ~ComplexClass();
    
    // Assignment operators
    ComplexClass& operator=(const ComplexClass& other);
    ComplexClass& operator=(ComplexClass&& other);
    
    // Basic member functions
    int getValue() const;
    void setValue(int value);
    std::string getString() const;
    void setString(const std::string& str);
    
    // Virtual functions
    virtual void virtualFunction();
    virtual int virtualFunctionWithReturn() const;
    virtual void pureVirtualFunction() = 0;
    
    // Static functions
    static int staticFunction();
    static std::string staticStringFunction();
    
    // Const member functions
    bool isPositive() const;
    size_t getVectorSize() const;
    
    // Reference and pointer functions
    int& getValueRef();
    const int& getValueConstRef() const;
    int* getValuePtr();
    const int* getValueConstPtr() const;
    
    // Functions with default parameters
    void functionWithDefault(int param1, int param2 = 10, std::string param3 = "default");
    
    // Functions with variadic parameters (std::initializer_list)
    void functionWithInitializerList(std::initializer_list<int> values);
    
    // Functions with function pointers
    void functionWithCallback(std::function<void(int)> callback);
    
    // Private and protected accessors for testing
    int getPrivateValue() const { return privateValue; }
    double getProtectedValue() const { return protectedValue; }
};

// Free functions
int freeFunction(int param);
std::string freeStringFunction(const std::string& input);
void freeVoidFunction();

} // namespace TestNamespace

#endif // COMPLEX_CLASS_HPP 