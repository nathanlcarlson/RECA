add_library(MyLib algorithm.hpp state.hpp utils.cpp couplings.cpp node.hpp)

target_include_directories (MyLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories (MyLib PUBLIC "/usr/local/include")

find_library (FFTW fftw3)
find_library (GLFW glfw)
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
target_link_libraries (MyLib "${FFTW}")
target_link_libraries (MyLib "${GLFW}")
target_link_libraries (MyLib "${OPENGL_LIBRARIES}")
add_executable (RECA reca.cpp)
add_executable (GRAPHICS graphics.cpp)
target_link_libraries(RECA MyLib)
target_link_libraries(GRAPHICS MyLib)

install(TARGETS RECA RUNTIME DESTINATION "${CMAKE_SOURCE_DIR}")
install(TARGETS GRAPHICS RUNTIME DESTINATION "${CMAKE_SOURCE_DIR}")
