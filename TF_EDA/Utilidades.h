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

struct Departamento
{
    std::string nombre;
    std::vector<std::string> ciudades;
};

int getRandomDepartament(std::vector<Departamento> arr);

extern const std::vector<std::string> estadosCiviles_lst;
extern const std::vector<std::string> nombres_lst;
extern const std::vector<std::string> apellidosPeruanos;
extern const std::vector<std::string> apellidosVenezolanos;
extern const std::vector<std::string> apellidosColombianos;
extern const std::vector<std::string> extensionEmail_lst;
extern const std::vector<std::string> nacionalidades_lst;
extern const std::vector<std::string> paises_lst;
extern const std::vector<Departamento> departamentosPeru_lst;
extern const std::vector<Departamento> departamentosVenezuela_lst;
extern const std::vector<Departamento> departamentosColombia_lst;
extern const std::vector<std::string> direcciones_lst;
