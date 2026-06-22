cmake_minimum_required(VERSION 3.16)

# Charger le système de build ESP-IDF
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Déclarer ton projet
project(TAKATRIS)

# Option backend graphique
option(USE_FRAMEBUFFER "Use framebuffer backend" ON)
