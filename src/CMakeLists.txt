add_library(MyLib algorithm.hpp state.hpp utils.cpp couplings.cpp node.hpp)

target_include_directories (MyLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories (MyLib PUBLIC "/usr/local/include")

find_library (FFTW fftw3)
target_link_libraries (MyLib "${FFTW}")
add_executable (RECA reca.cpp)
target_link_libraries(RECA MyLib)

install(TARGETS RECA RUNTIME DESTINATION "${CMAKE_SOURCE_DIR}")
