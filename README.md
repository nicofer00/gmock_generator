# Google Test C++ Unit Testing
## Setup
*NOTE:* Setup of unit tests assumes you have the appropriate setup for running googletest. 

Python and pip must be on path to install and use virtual environments for report generation. 

Run a `git submodule update --init --recursive` to ensure that the `googletest`
submodule is initialized properly to be built with the application.

## Add Test Cases
In order to add test cases, simply add a `TEST()` macro call. If adding a new 
suite / file. Add a folder for the interface ex: `interface/` for `interface_file.cpp`
and then your test file name should be your test name with `test_*.cc` ex:
`test_interface_file.cc`. If you have a test fixture be sure to use `TEST_F()` instead.

## Running Tests
From the project directory, run `make test` to run all tests available to google 
test. This will also automatically generate the coverage report as well. The 
tests will store the output in a log file in the build folder. To run the tests 
with the gtest output, run `make gtest` instead. This will produce the output 
from googletest directly with all printouts from the tests and source code.

In order to run specific tests, specify the `CASES` macro in your command.  
Set the value to be your test cases and fixtures separated by `:` for example: 

`make test CASES=TestFixture.*` - runs all of the tests inside of the fixture 
named `TestFixture`.

`make test CASES=TestFixture.TestCaseName` - runs one test named 
`TestCaseName` inside of the fixture named `TestFixture`.

`make test CASES=TestFixture.TestCaseName:TestFixture.TestCaseName` - 
runs two test cases, `TestCaseName` and `TestCaseName`, inside of the 
fixture named `TestFixture`.

`make test CASES=TestFixture1.*:TestFixture2.*` - runs all of the tests 
inside of two fixtures, `TestFixture1` and `TestFixture2`.

## Report Generation
Report generation with gcovr happens at the end of the `make test` target, but 
can be run individually with a `make report` command. Within the build 
directory, a gcov folder will contain the html output. The overall results will
be in the file `{project_name}_test.html` and be nested to expand into the results for
each source file's coverage report. Gcovr 7.0 is used in a python venv activated
within the report generating target. 

## Mock Generation
A simple gmock generator is in the scripts folder and can be invoked with the 
`make mock FILE=...` target where `FILE` is set to the path to the desired C++ 
header to mock. Once generated, the mock class is output automatically to the 
`./test/mocks` directory. Know that the script has limitations when it comes to
the nesting of namespaces and classes in C++, and that mock classes may not be
completely accurate and need to be reviewed.

## Memory Sanitization
Unit tests are compiled with the `-fsanitize=address` flag which uses 
AddressSanitizer to check for memory mismanagement. This runs on linux builds
so ensure that you have the sanatizer installed if needed. This will also fail
unit tests if memory is not handled properly in tests.