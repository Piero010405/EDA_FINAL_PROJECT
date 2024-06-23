#pragma once
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cctype>
#include <string>
#include <ctime>
#include <random>
#include <utility>
#include <fstream>
#include <sstream>
#include <iomanip>

std::string toLowerCase(const std::string& str);
int formatearDni(std::string dni);
std::string obtenerDniValido();
int getRandom(std::vector<std::string> arr);
int getRandomNacionality(std::vector<std::string> arr, const double probabilidad = 0.9609); // Tasa de innimgrantes (2023) = 3.91%
std::string generarDNI(int id);
std::string generarTelefono();
std::string generarEmail(const std::string& nombre, const std::string& apellido);
std::pair<std::vector<std::string>, std::string> generarInformacionBasica(const std::string& nacionalidad);
int generarDNIAleatorio();

#include "Utilidades.h"
