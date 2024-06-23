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
#include "BTree.h"
#include "Funciones.h"
#include "Utilidades.h"
#include "Constantes.h"
#include "Serialization.h"

BTree generarCiudadanosYBTree(int poblacionSize, const std::string& ciudadanosFileName, const std::string& btreeFileName) {
    // Crear archivo para ciudadanos
    std::ofstream ofsCiudadanos(ciudadanosFileName, std::ios::binary);

    // Crear el BTree
    BTree btree = BTree(GRADO_MINIMO);

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

        // Insertar en la btree
        btree.insert(ciudadano.getId(), memoryAddress);
    }

    // Serializar la tabla hash
    SerializeBTree(btree, btreeFileName);
    return btree;
}

BTree cargarBTree(const std::string& btreeFileName) {
    BTree btree = DeserializeBTree(btreeFileName, GRADO_MINIMO);
    return btree;
}

BTree cargarDatos(const std::string& ciudadanosFileName, const std::string& btreeFileName) {
    std::ifstream fileStream(btreeFileName, std::ios::binary);

    if (!fileStream.is_open()) {
        return generarCiudadanosYBTree(POBLACION, ciudadanosFileName, btreeFileName);
    }
    else {
        return cargarBTree(btreeFileName);
    }
}

void insertarCiudadanoEnBinario(Ciudadano& nuevoCiudadano, const std::string& ciudadanosFileName, BTree& btree) {
    // Abrir archivo en modo append
    std::ofstream ofsCiudadanos(ciudadanosFileName, std::ios::binary | std::ios::app);
    if (!ofsCiudadanos.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo de ciudadanos para escribir");
    }

    // Serializar el nuevo ciudadano
    std::stringstream ss;
    {
        boost::archive::binary_oarchive oa(ss);
        oa << nuevoCiudadano;
    }
    std::string ciudadanoData = ss.str();
    size_t ciudadanoSize = ciudadanoData.size();

    // Escribir el tamaño y los datos del nuevo ciudadano en el archivo
    ofsCiudadanos.write(reinterpret_cast<const char*>(&ciudadanoSize), sizeof(size_t));
    ofsCiudadanos.write(ciudadanoData.c_str(), ciudadanoSize);

    // Validar la posición después de escribir
    size_t memoryAddressAfterWrite = ofsCiudadanos.tellp();

    // Obtener la posición de memoria actual (al final del archivo)
    size_t memoryAddress = memoryAddressAfterWrite - (ciudadanoSize + sizeof(size_t));

    // Insertar en la btree
    btree.insert(nuevoCiudadano.getId(), memoryAddress);

    ofsCiudadanos.close();
}

Ciudadano buscarCiudadanoPorDNI(const std::string& dni, const std::string& ciudadanosFileName, BTree& btree) {
    int id = formatearDni(dni);

    // Buscar la dirección de memoria en la tabla hash
    std::pair<int, size_t> entry = btree.search(id);
    if (entry.first == -1) {
        throw std::runtime_error("\t\tCiudadano no encontrado...\n");
    }

    size_t memoryAddress = entry.second;

    // Cargar el ciudadano desde la dirección de memoria
    boost::interprocess::file_mapping file(ciudadanosFileName.c_str(), boost::interprocess::read_only);
    size_t ciudadanoSize;

    // Leer el tamaño del ciudadano
    {
        boost::interprocess::mapped_region region(file, boost::interprocess::read_only, memoryAddress, sizeof(size_t));
        std::memcpy(&ciudadanoSize, region.get_address(), sizeof(size_t));
    }

    // Leer los datos del ciudadano
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


void sobrescribirBTree(const BTree& nuevoBtree, const std::string& btreeFileName) {
    SerializeBTree(nuevoBtree, btreeFileName);
}


void buscarCiudadano(BTree& btree, std::string ciudadanosFileName) {
    std::cout << "\t\t|| Buscar Ciudadano ||\n";
    std::cout << "\t\t**********************\n";

    std::string dni = obtenerDniValido();

    try {
        std::cout << "\t\tBuscando Ciudadano ........\n";
        std::cout << "\t\t**********************\n";
        Ciudadano ciudadano = buscarCiudadanoPorDNI(dni, ciudadanosFileName, btree);
        ciudadano.imprimir();
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
    system("pause");
    system("cls");
}

void eliminarCiudadano(BTree& btree, std::string ciudadanosFileName) {
    std::cout << "\t\t|| Eliminar Ciudadano ||\n";
    std::cout << "\t\t************************\n";

    std::string dni = obtenerDniValido();
    std::string rpt;

    try {
        Ciudadano ciudadano = buscarCiudadanoPorDNI(dni, ciudadanosFileName, btree);
        ciudadano.imprimir();
        std::cout << "\t\t*************************************\n";
        std::cout << "\t\tEstas seguro de eliminar al ciudadano? [si/no]: ";
        std::getline(std::cin, rpt);
        if (toLowerCase(rpt) == POSITIVO) {
            btree.Delete(formatearDni(dni));
            std::cout << "\t\tCiudadano con DNI [" << dni << "] ha sido eliminado satisfactoriamente ...\n";
        }
        else {
            std::cout << "\t\tOperacion cancelada ... \n";
        }
        system("pause");
        system("cls");
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
    }
}

void insertarCiudadanoAleatoriamente(BTree& btree, std::string& ciudadanosFileName) {
    std::cout << "\t\t|| Insertar Nuevo Ciudadano Aleatoriamente ||\n";
    std::cout << "\t\t*********************************************\n";

    bool valido = false;
    std::string dni;
    int id;
    while (!valido) {
        dni = obtenerDniValido();
        id = formatearDni(dni);
        bool existe = btree.isExisting(id);
        if (existe) {
            std::cout << "\t\tYa existe un ciudadano con el DNI ingresado... Ingresar un nuevo DNI\n";
        }
        valido = !existe;
    }

    Ciudadano obj = Ciudadano(id);
    std::cout << "\t\t*************************************\n";
    std::cout << "\t\tCiudadano generado aleatoriamente ...\n";
    obj.imprimir();
    std::cout << "\t\t*************************************\n";
    insertarCiudadanoEnBinario(obj, ciudadanosFileName, btree);
    std::cout << "\t\tCiudadano insertado exitosamente ...\n";
    system("pause");
    system("cls");
}

void insertarNuevoCiudadano(BTree& btree, std::string& ciudadanosFileName) {
    bool valido = false;
    std::string dni;
    int id;
    std::cout << "\t\t|| Insertar Nuevo Ciudadano ||\n";
    std::cout << "\t\t******************************\n";
    while (!valido) {
        dni = obtenerDniValido();
        id = formatearDni(dni);
        bool existe = btree.isExisting(id);
        if (existe) {
            std::cout << "\t\tYa existe un ciudadano con el DNI ingresado... Ingresar un nuevo DNI\n";
        }
        valido = !existe;
    }

    std::string nombre;
    std::vector<std::string> apellidos;
    std::string nacionalidad;
    std::string lugarNacimiento;
    std::string direccion;
    std::string estadoCivil;

    std::cout << "\n\t\tIngresar Nombre: "; std::getline(std::cin, nombre);
    std::cout << "\n\t\tIngresar Apellido Paterno: "; std::getline(std::cin, apellidos[0]);
    std::cout << "\n\t\tIngresar Apellido Materno: "; std::getline(std::cin, apellidos[1]);
    std::cout << "\n\t\tIngresar Nacionalidad: "; std::getline(std::cin, nacionalidad);
    std::cout << "\n\t\tIngresar Lugar de Nacimiento: "; std::getline(std::cin, lugarNacimiento);
    std::cout << "\n\t\tIngresar Direccion: "; std::getline(std::cin, direccion);
    std::string telefono = generarTelefono();
    std::string email = generarEmail(nombre, apellidos[0]);
    std::cout << "\n\t\tTelefono generado: " << telefono;
    std::cout << "\n\t\tEmail generado: " << email;
    int opt;
    do {
        std::cout << "\n\t\tEstado Civil:\n";
        std::cout << "\t\t[*] CASADO ............ [0]\n";
        std::cout << "\t\t[*] SOLTERO ........... [1]\n";
        std::cout << "\t\t[*] DIVORCIADO ........ [2]\n";
        std::cout << "\t\t[*] VIUDO ............. [3]\n";
        std::cout << "\t\tSeleccione una opcion [0-3]: ";
        std::cin >> opt;
        if (opt <= 3 && opt >= 0) {
            estadoCivil = estadosCiviles_lst[opt];
        }
        else {
            std::cout << "\n\t\tIngrese una opcion valida ..\n";
            system("pause");
        }
        system("cls");
    } while (opt > 3);
    Ciudadano obj = Ciudadano(id, dni, nombre, apellidos, nacionalidad, lugarNacimiento, direccion, telefono, email, estadoCivil);
    insertarCiudadanoEnBinario(obj, ciudadanosFileName, btree);
    std::cout << "\t\tUsuario creado e insertado exitosamente...\n";
    system("pause");
    system("cls");
}

void salir() {
    std::cout << "\n\n\t\tSalida del sistema....\n";
    system("pause");
    system("cls");
}

void insertarCiudadano(BTree& btree, std::string ciudadanosFileName) {
    int opt;
    do {
        std::cout << "\t\t|| Insertar Nuevo Ciudadano ||\n";
        std::cout << "\t\t******************************\n";
        std::cout << "\t\t[*] Insertar nuevo Ciudadano Aleatoriamente .. [1]\n";
        std::cout << "\t\t[*] Insertar nuevo Ciudadano ................. [2]\n";
        std::cout << "\t\t[*] Salir .................................... [3]\n";
        std::cout << "\t\tSeleccione una opcion                        [1-3]: ";
        std::cin >> opt;
        switch (opt)
        {
        case 1: system("cls"); insertarCiudadanoAleatoriamente(btree, ciudadanosFileName); break;
        case 2: system("cls"); insertarNuevoCiudadano(btree, ciudadanosFileName); break;
        case 3: salir(); break;
        default: std::cout << "\n\t\tIngrese una opcion válida entre [1-4]\n"; system("pause"); system("cls");
        }
    } while (opt != 3);

}

void menuPrincipal() {
    const std::string ciudadanosFileName = CIUDADANOS_FILE_NAME;
    const std::string btreeFileName = BTREE_FILE_NAME;

    BTree btree = cargarDatos(ciudadanosFileName, btreeFileName);

    int opt;
    do
    {
        std::cout << "\t\t|||| Sistema de RENIEC Nacional ||||\n";
        std::cout << "\t\t************************************\n";
        std::cout << "\t\t[*] Buscar Ciudadano .............. [1]\n";
        std::cout << "\t\t[*] Eliminar Ciudadano ............ [2]\n";
        std::cout << "\t\t[*] Insertar Nuevo Ciudadano ...... [3]\n";
        std::cout << "\t\t[*] Salir ......................... [4]\n";
        std::cout << "\t\tSeleccione una opcion             [1-4]: ";
        std::cin >> opt;
        switch (opt)
        {
        case 1: system("cls"); buscarCiudadano(btree, ciudadanosFileName); break;
        case 2: system("cls"); eliminarCiudadano(btree, ciudadanosFileName); break;
        case 3: system("cls"); insertarCiudadano(btree, ciudadanosFileName); break;
        case 4: salir(); sobrescribirBTree(btree, btreeFileName); exit(0); break;
        default: std::cout << "\n\t\tIngrese una opcion válida entre [1-4]\n"; system("pause"); system("cls");
        }
    } while (opt != 4);
}

int main() {
    menuPrincipal();
    return 0;
}