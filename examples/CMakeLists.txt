
macro(ncnn_add_example name)
    add_executable(${name} ${name}.cpp)
    target_compile_options(${name} PRIVATE -std=c++11)
    target_link_libraries(${name} PRIVATE ncnn ${OpenCV_LIBS})

    # add test to a virtual project group
    set_property(TARGET ${name} PROPERTY FOLDER "examples")
endmacro()

find_package(OpenCV QUIET COMPONENTS core highgui imgproc imgcodecs)
if(NOT OpenCV_FOUND)
    find_package(OpenCV QUIET COMPONENTS core highgui imgproc)
endif()

if(OpenCV_FOUND)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)
    include_directories(${CMAKE_CURRENT_BINARY_DIR}/../src)
    ncnn_add_example(hgpose)

else()
    message(WARNING "OpenCV not found, examples won't be built")
endif()
