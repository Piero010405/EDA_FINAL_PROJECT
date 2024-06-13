#pragma once
#include <iostream>
#include <vector>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>

class Ciudadano
{
    int id;
    std::string dni;
    std::string nombre;
    std::vector<std::string> apellidos;
    std::string nacionalidad;
    std::string lugarNacimiento;
    std::string direccion;
    std::string telefono;
    std::string email;
    std::string estadoCivil;

    friend class boost::serialization::access;

    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& dni& nombre& apellidos& nacionalidad& lugarNacimiento& direccion& telefono& email& estadoCivil;
    }
public:

    Ciudadano() = default;

    Ciudadano(int id);

    Ciudadano(int id, std::string dni, std::string nombre, std::vector<std::string> apellidos, std::string nacionalidad,
        std::string lugarNacimiento, std::string direccion, std::string telefono, std::string email,
        std::string estadoCivil){}

    void imprimir();

    void save(std::ostringstream& oss);

    void load(std::ostringstream& oss);

    size_t getCiudadanoSize();

    int getId();
};

BOOST_CLASS_VERSION(Ciudadano, 1);
