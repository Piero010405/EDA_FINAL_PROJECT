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
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include "Ciudadano.h"
#include "Funciones.cpp"
#include "Utilidades.cpp"

Ciudadano::Ciudadano(int id)
{
    this->id = id;
    std::string nacionalidad = nacionalidades_lst[getRandomNacionality(nacionalidades_lst)];
    std::string nombre = nombres_lst[getRandom(nombres_lst)];
    std::pair<std::vector<std::string>, std::string> info = generarInformacionBasica(nacionalidad);
    this->apellidos = info.first;
    this->lugarNacimiento = info.second;

    this->nacionalidad = nacionalidad;

    this->dni = generarDNI(id);

    this->nombre = nombre;

    this->telefono = generarTelefono();
    this->email = generarEmail(nombre, this->apellidos[0]);
    this->estadoCivil = estadosCiviles_lst[getRandom(estadosCiviles_lst)];
    this->direccion = direcciones_lst[getRandom(direcciones_lst)];
}

Ciudadano::Ciudadano(int id, std::string dni, std::string nombre, std::vector<std::string> apellidos, std::string nacionalidad, 
    std::string lugarNacimiento, std::string direccion, std::string telefono, std::string email, 
    std::string estadoCivil): id(id), dni(dni), nombre(nombre), apellidos(apellidos), nacionalidad(nacionalidad), lugarNacimiento(lugarNacimiento), direccion(direccion), telefono(telefono), email(email), estadoCivil(estadoCivil) {}

void Ciudadano::imprimir()
{
    std::cout << "\t\tDNI: " << this->dni << std::endl;
    std::cout << "\t\tNombres: " << this->nombre << std::endl;
    std::cout << "\t\tApellidos: " << this->apellidos[0] << " " << this->apellidos[1] << std::endl;
    std::cout << "\t\tNacionalidad: " << this->nacionalidad << std::endl;
    std::cout << "\t\tLugar de Nacimiento: " << this->lugarNacimiento << std::endl;
    std::cout << "\t\tDirección: " << this->direccion << std::endl;
    std::cout << "\t\tTeléfono: " << this->telefono << std::endl;
    std::cout << "\t\tEmail: " << this->email << std::endl;
    std::cout << "\t\tEstado Civil: " << this->estadoCivil << std::endl;
    std::cout << std::endl;
}

void Ciudadano::save(std::ostringstream& oss)
{
    boost::archive::binary_oarchive oa(oss);
    oa&* (this);
}

void Ciudadano::load(std::ostringstream& oss)
{
    std::string str_data = oss.str();
    std::istringstream iss(str_data);
    boost::archive::binary_iarchive ia(iss);
    ia&* (this);
}

size_t Ciudadano::getCiudadanoSize() {
    size_t size = 0;
    size = sizeof(this->id);
    size += this->dni.size() + 1;
    size += this->nombre.size() + 1;
    size += this->nacionalidad.size() + 1;
    size += this->lugarNacimiento.size() + 1;
    size += this->direccion.size() + 1;
    size += this->telefono.size() + 1;
    size += this->email.size() + 1;
    size += this->estadoCivil.size() + 1;
    size += sizeof(this->apellidos);
    for (const std::string& apellido : this->apellidos) {
        size += apellido.size() + 1;
    }

    return size;
}

int Ciudadano::getId() {
    return this->id;
}