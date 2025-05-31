#include "test_common.h"
#include "mock_ComplexClass.hpp"

GMOCK_TEST_ATTRIBUTES

// Test basic constructor and destructor
TEST(ComplexClassTest, ConstructorAndDestructor) {
    MockComplexClass mock;
    EXPECT_TRUE(true); // Basic test to ensure mock compiles
}

// Test basic member functions
TEST(ComplexClassTest, BasicMemberFunctions) {
    MockComplexClass mock;
    
    EXPECT_CALL(mock, getValue())
        .WillOnce(Return(42));
    
    EXPECT_CALL(mock, setValue(_))
        .Times(1);
    
    EXPECT_CALL(mock, getString())
        .WillOnce(Return("test string"));
    
    EXPECT_CALL(mock, setString(_))
        .Times(1);
    
    EXPECT_EQ(mock.getValue(), 42);
    mock.setValue(100);
    EXPECT_EQ(mock.getString(), "test string");
    mock.setString("new string");
}

// Test virtual functions
TEST(ComplexClassTest, VirtualFunctions) {
    MockComplexClass mock;
    
    EXPECT_CALL(mock, virtualFunction())
        .Times(1);
    
    EXPECT_CALL(mock, virtualFunctionWithReturn())
        .WillOnce(Return(84));
    
    mock.virtualFunction();
    EXPECT_EQ(mock.virtualFunctionWithReturn(), 84);
}

// Test static functions (these call the real implementation)
TEST(ComplexClassTest, StaticFunctions) {
    // Note: Static functions are not mocked, they call the real implementation
    EXPECT_EQ(TestNamespace::ComplexClass::staticFunction(), 42);
    EXPECT_EQ(TestNamespace::ComplexClass::staticStringFunction(), "static string");
}

// Test const member functions
TEST(ComplexClassTest, ConstMemberFunctions) {
    MockComplexClass mock;
    
    EXPECT_CALL(mock, isPositive())
        .WillOnce(Return(true))
        .WillOnce(Return(false));
    
    EXPECT_CALL(mock, getVectorSize())
        .WillOnce(Return(5));
    
    EXPECT_TRUE(mock.isPositive());
    EXPECT_FALSE(mock.isPositive());
    EXPECT_EQ(mock.getVectorSize(), 5);
}

// Test reference and pointer functions
TEST(ComplexClassTest, ReferenceAndPointerFunctions) {
    MockComplexClass mock;
    int testValue = 42;
    
    EXPECT_CALL(mock, getValueRef())
        .WillOnce(ReturnRef(testValue));
    
    EXPECT_CALL(mock, getValueConstRef())
        .WillOnce(ReturnRef(testValue));
    
    EXPECT_CALL(mock, getValuePtr())
        .WillOnce(Return(&testValue));
    
    EXPECT_CALL(mock, getValueConstPtr())
        .WillOnce(Return(&testValue));
    
    int& ref = mock.getValueRef();
    const int& constRef = mock.getValueConstRef();
    int* ptr = mock.getValuePtr();
    const int* constPtr = mock.getValueConstPtr();
    
    EXPECT_EQ(ref, 42);
    EXPECT_EQ(constRef, 42);
    EXPECT_EQ(*ptr, 42);
    EXPECT_EQ(*constPtr, 42);
}

// Test functions with default parameters
TEST(ComplexClassTest, FunctionsWithDefaultParameters) {
    MockComplexClass mock;
    
    EXPECT_CALL(mock, functionWithDefault(_, _, _))
        .Times(1);
    
    mock.functionWithDefault(1, 2, "test");
}

// Test functions with initializer_list
TEST(ComplexClassTest, FunctionsWithInitializerList) {
    MockComplexClass mock;
    
    EXPECT_CALL(mock, functionWithInitializerList(_))
        .Times(1);
    
    mock.functionWithInitializerList({1, 2, 3, 4, 5});
}

// Test functions with callbacks
TEST(ComplexClassTest, FunctionsWithCallbacks) {
    MockComplexClass mock;
    
    EXPECT_CALL(mock, functionWithCallback(_))
        .Times(1);
    
    mock.functionWithCallback([](int val) { /* callback */ });
}

// Test free functions
TEST(ComplexClassTest, FreeFunctions) {
    // Note: Free functions are not mocked, they call the real implementation
    EXPECT_EQ(TestNamespace::freeFunction(21), 42);
    EXPECT_EQ(TestNamespace::freeStringFunction("test"), "Processed: test");
    TestNamespace::freeVoidFunction(); // Should not crash
}

// Test pure virtual function (should not be called directly)
TEST(ComplexClassTest, PureVirtualFunction) {
    MockComplexClass mock;
    
    EXPECT_CALL(mock, pureVirtualFunction())
        .Times(1);
    
    mock.pureVirtualFunction();
}

// Test accessor functions
TEST(ComplexClassTest, AccessorFunctions) {
    MockComplexClass mock;
    
    EXPECT_CALL(mock, getPrivateValue())
        .WillOnce(Return(100));
    
    EXPECT_CALL(mock, getProtectedValue())
        .WillOnce(Return(3.14));
    
    EXPECT_EQ(mock.getPrivateValue(), 100);
    EXPECT_EQ(mock.getProtectedValue(), 3.14);
} 