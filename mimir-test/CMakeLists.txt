project(mimir-tests)

set(CMAKE_CXX_STANDARD 17)

find_package(Qt5 COMPONENTS Core Test REQUIRED)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(mimir-test
    test-runner.cpp
    TestListener.cpp
    TestListener.h
    TestNeuron.cpp
    TestNeuron.h
    TestPatternFind.cpp
    TestPatternFind.h
    TestRunner.cpp
    TestRunner.h
    tst_Models.cpp
    tst_Models.h)

link_directories(../mimir)

target_link_libraries(mimir-test Qt5::Core Qt5::Test mimir)