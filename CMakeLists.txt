
# Copyright (c) 2019 The Karbowanec developers
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
# Code formatting based on CS106B Style


cmake_minimum_required(VERSION 2.8.12)
set(CMAKE_BUILD_TYPE Release)
project(krbmon)

add_library(selfinfo STATIC selfinfo.cpp)

add_executable(${PROJECT_NAME} main.cpp util.cpp)
target_link_libraries(${PROJECT_NAME} selfinfo)
target_link_libraries(${PROJECT_NAME} -pthread)

install(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME} DESTINATION bin)
