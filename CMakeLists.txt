cmake_minimum_required(VERSION 3.21)
project(J03Curator VERSION 0.6 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(Qt6 REQUIRED COMPONENTS Core Sql)
add_executable(J03Curator main.cpp)
target_link_libraries(J03Curator PRIVATE Qt6::Core Qt6::Sql)
install(TARGETS J03Curator RUNTIME DESTINATION bin)

# Flathub desktop integration
install(FILES org.oneal.J03Curator.desktop
        DESTINATION share/applications)
install(FILES org.oneal.J03Curator.metainfo.xml
        DESTINATION share/metainfo)
install(FILES org.oneal.J03Curator.svg
        DESTINATION share/icons/hicolor/scalable/apps)

install(FILES LICENSE DESTINATION share/doc/j03curator)
