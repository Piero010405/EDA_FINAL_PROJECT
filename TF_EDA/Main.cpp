#include <iostream>
#include <sstream>
#include <string>
#include <memory>
#include <thread>
#include <vector>
#include <fstream>
#include <cmath>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include "Ciudadano.h"
#include "CuckooHash.h"


const std::string POSITIVO = "si";
const int MAX_POBLACION = 99999999;
const int POBLACION = 100000;
const int NUM_TABLAS_CUCKOO = 3;

CuckooHashTable generarCiudadanosYTablaHash(int poblacionSize, const std::string& ciudadanosFileName, const std::string& tablaHashFileName) {
    // Crear archivo para ciudadanos
    std::ofstream ofsCiudadanos(ciudadanosFileName, std::ios::binary);
    boost::archive::binary_oarchive oaCiudadanos(ofsCiudadanos);

    // Crear tabla hash de Cuckoo
    CuckooHashTable tablaHash(NUM_TABLAS_CUCKOO, 10);

    for (int i = 1; i <= poblacionSize; ++i) {
        Ciudadano ciudadano(i);
        // Serializar ciudadano en un stream
        std::stringstream ss;
        {
            boost::archive::binary_oarchive oa(ss);
            oa << ciudadano;
        }
        // Escribir el ciudadano en el archivo
        std::string ciudadanoData = ss.str();
        size_t memoryAddress = ofsCiudadanos.tellp();
        size_t ciudadanoSize = ciudadanoData.size();
        ofsCiudadanos.write(reinterpret_cast<const char*>(&ciudadanoSize), sizeof(size_t)); // Guardar el tamaño
        ofsCiudadanos.write(ciudadanoData.c_str(), ciudadanoData.size());

        // Insertar en la tabla hash
        tablaHash.insertar(ciudadano.getId(), memoryAddress);
    }

    // Serializar la tabla hash
    std::ofstream ofsTablaHash(tablaHashFileName, std::ios::binary);
    boost::archive::binary_oarchive oaTablaHash(ofsTablaHash);
    oaTablaHash << tablaHash;
    return tablaHash;
}

CuckooHashTable cargarTablaHash(const std::string& tablaHashFileName) {
    std::ifstream ifsTablaHash(tablaHashFileName, std::ios::binary);
    boost::archive::binary_iarchive iaTablaHash(ifsTablaHash);
    CuckooHashTable tablaHash;
    iaTablaHash >> tablaHash;
    return tablaHash;
}

int formatearDni(std::string dni) {
    int id;
    try {
        id = std::stoi(dni);
    }
    catch (const std::invalid_argument& e) {
        throw std::runtime_error("DNI inválido. Debe ser un número entero.");
    }
    return id;
}

Ciudadano buscarCiudadanoPorDNI(const std::string& dni, const std::string& ciudadanosFileName, CuckooHashTable& tablaHash) {
    int id = formatearDni(dni);

    // Buscar la direcci�n de memoria en la tabla hash
    std::pair<int, size_t> entry = tablaHash.buscar(id);
    std::cout << entry.first << std::endl;
    if (entry.first == -1) {
        throw std::runtime_error("\t\tCiudadano no encontrado...\n");
        return;
    }

    size_t memoryAddress = entry.second;

    // Cargar el ciudadano desde la dirección de memoria
    boost::interprocess::file_mapping file(ciudadanosFileName.c_str(), boost::interprocess::read_only);
    boost::interprocess::mapped_region region(file, boost::interprocess::read_only, memoryAddress, sizeof(size_t));

    size_t ciudadanoSize;
    std::memcpy(&ciudadanoSize, region.get_address(), sizeof(size_t)); // Leer el tamaño del ciudadano

    boost::interprocess::mapped_region regionCiudadano(file, boost::interprocess::read_only, memoryAddress + sizeof(size_t), ciudadanoSize);

    std::stringstream ss;
    ss.write(static_cast<const char*>(regionCiudadano.get_address()), ciudadanoSize);

    Ciudadano ciudadano;
    {
        boost::archive::binary_iarchive ia(ss);
        ia >> ciudadano;
    }

    return ciudadano;
}

CuckooHashTable cargarDatos(const std::string& ciudadanosFileName, const std::string& tablaHashFileName) {
    std::ifstream fileStream(tablaHashFileName, std::ios::binary);

    if (!fileStream.is_open()) {
        return generarCiudadanosYTablaHash(POBLACION, ciudadanosFileName, tablaHashFileName);
    }
    else {
        return cargarTablaHash(tablaHashFileName);
    }
}

std::string dniValido() {
    std::string dni;
    bool esValido = false;

    while (!esValido) {
        std::cout << "Ingresar el DNI a buscar (8 dígitos): " << std::endl;
        std::cin >> dni;

        if (dni.length() == 8 && std::all_of(dni.begin(), dni.end(), ::isdigit)) {
            esValido = true;
        }
        else {
            std::cerr << "DNI inválido. Por favor ingrese un DNI con 8 dígitos." << std::endl;
        }
    }

    return dni;
}

void buscarCiudadano(CuckooHashTable cuckooTable, std::string ciudadanosFileName) {
    std::cout << "\t\t|| Buscar Ciudadano ||\n";
    std::string dni = dniValido();

    try {
        Ciudadano ciudadano = buscarCiudadanoPorDNI(dni, ciudadanosFileName, cuckooTable);
        ciudadano.imprimir();
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

void eliminarCiudadano(CuckooHashTable cuckooTable, std::string ciudadanosFileName) {
    std::cout << "\t\t|| Eliminar Ciudadano ||\n";
    std::string dni = dniValido();
    std::string rpt;

    try {
        Ciudadano ciudadano = buscarCiudadanoPorDNI(dni, ciudadanosFileName, cuckooTable);
        ciudadano.imprimir();
        std::cout << "\t\tEstas seguro de eliminar al ciudadano? [si/no]: ";
        std::cin >> rpt;
        std::transform(rpt.begin(), rpt.end(), rpt.begin(), std::tolower);
        if (rpt == POSITIVO) {
            cuckooTable.eliminar(formatearDni(dni));
            std::cout << "\t\tCiudadano con DNI [" << dni << "] ha sido eliminado satisfactoriamente .../n";
        }
        else {
            std::cout << "\t\tOperacion cancelada ... /n";
        }
        system("pause");
        system("cls");
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }

}

void insertarCiudadano() {
    std::cout << "\t\t|| Insertar Nuevo Ciudadano ||\n";

}

void salir() {
    std::cout << "\n\n\t\tSalida del sistema....\n";
}

void menuPrincipal() {
    const std::string ciudadanosFileName = "ciudadanosFile.dat";
    const std::string cuckooFileName = "cuckooFile.dat";

    // falta poner con punteros
    CuckooHashTable cuckooTable = cargarDatos(ciudadanosFileName, cuckooFileName);

    int opt = 0;
    std::cout << "\t\t|||| Sistema de RENIEC Nacional ||||\n";
    do
    {
        std::cout << "\t\t[*] Buscar Ciudadano .............. [1]\n";
        std::cout << "\t\t[*] Eliminar Ciudadano ............ [2]\n";
        std::cout << "\t\t[*] Insertar Nuevo Ciudadano ...... [3]\n";
        std::cout << "\t\t[*] Salir ......................... [4]\n";
        std::cout << "Seleccione una opcion                 [1-4]: ";
        std::cin >> opt;
        switch (opt)
        {
        case 1: system("cls"); buscarCiudadano(cuckooTable, ciudadanosFileName); break;
        case 2: system("cls"); eliminarCiudadano(cuckooTable, ciudadanosFileName); break;
        case 3: system("cls"); insertarCiudadano(); break;
        case 4: salir(); exit(0); break;
        default: std::cout << "\t\tIngrese una opcion válida entre [1-4]\n";
        }
    } while (opt != 4);
}

int main() {
    menuPrincipal();
    return 0;
}