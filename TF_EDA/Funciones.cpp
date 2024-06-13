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
#include "Utilidades.cpp"

extern std::string toLowerCase(const std::string& str)
{
    std::string resultado = str;
    for (size_t i = 0; i < resultado.length(); ++i)
    {
        resultado[i] = tolower(static_cast<unsigned char>(resultado[i]));
    }
    return resultado;
}

extern int getRandom(std::vector<std::string> arr)
{
    int random = rand() % arr.size();
    return random;
}

extern int getRandomNacionality(std::vector<std::string> arr, const double probabilidad = 0.9609) // Tasa de innimgrantes (2023) = 3.91%
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    double valorAleatorio = dist(gen);

    if (valorAleatorio < probabilidad)
    {
        return 0;
    }
    else
    {
        int indiceAleatorio = rand() % (arr.size() - 1) + 1;
        return indiceAleatorio;
    }
}

extern std::string generarDNI(int id) {
    std::stringstream ss;
    ss << std::setw(8) << std::setfill('0') << id;
    return ss.str();
}

extern std::string generarTelefono()
{
    std::string tel = "9";
    for (int i = 0; i < 8; i++)
    {
        int num = rand() % 10;
        tel += static_cast<char>(num + '0');
    }
    return tel;
}

extern std::string generarEmail(const std::string& nombre, const std::string& apellido)
{
    std::string email = std::string(1, tolower(static_cast<unsigned char>(nombre[0]))) + "." + toLowerCase(apellido) + extensionEmail_lst[getRandom(extensionEmail_lst)];
    return email;
}

extern std::pair<std::vector<std::string>, std::string> generarInformacionBasica(const std::string& nacionalidad)
{
    std::string lugarNacimiento;
    std::vector<std::string> apellidos;
    if (nacionalidad == nacionalidades_lst[0])
    {
        Departamento dptPeruano = departamentosPeru_lst[getRandomDepartament(departamentosPeru_lst)];

        apellidos = { apellidosPeruanos[getRandom(apellidosPeruanos)], apellidosPeruanos[getRandom(apellidosPeruanos)] };

        lugarNacimiento = dptPeruano.ciudades[getRandom(dptPeruano.ciudades)] + ", " + dptPeruano.nombre + ", " + paises_lst[0];
    }
    else if (nacionalidad == nacionalidades_lst[1])
    {
        Departamento dptVenezolano = departamentosVenezuela_lst[getRandomDepartament(departamentosVenezuela_lst)];
        apellidos = { apellidosVenezolanos[getRandom(apellidosVenezolanos)], apellidosVenezolanos[getRandom(apellidosVenezolanos)] };
        lugarNacimiento = dptVenezolano.ciudades[getRandom(dptVenezolano.ciudades)] + ", " + dptVenezolano.nombre + ", " + paises_lst[1];
    }
    else
    {
        Departamento dptColombiano = departamentosColombia_lst[getRandomDepartament(departamentosColombia_lst)];
        apellidos = { apellidosColombianos[getRandom(apellidosColombianos)], apellidosColombianos[getRandom(apellidosColombianos)] };
        lugarNacimiento = dptColombiano.ciudades[getRandom(dptColombiano.ciudades)] + ", " + dptColombiano.nombre + ", " + paises_lst[2];
    }

    return std::pair<std::vector<std::string>, std::string>(apellidos, lugarNacimiento);
}