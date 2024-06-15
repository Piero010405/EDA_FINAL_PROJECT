#pragma once
#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <utility>
#include <random>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/utility.hpp>

namespace boost {
    namespace serialization {
        template<class Archive>
        void serialize(Archive& ar, std::pair<int, size_t>& p, const unsigned int version) {
            ar& p.first;
            ar& p.second;
        }
    }
}

class CuckooHashTable {
private:
    std::vector<std::vector<std::pair<int, size_t>>> tables;
    int numTables;
    int size;
    int numElements;
    std::vector<std::vector<int>> hashSeeds;
    double umbralredimensionamiento = 0.7;
    int numberHashesPerTable = 4;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        ar& tables& numTables& size& numElements& hashSeeds& umbralredimensionamiento& numberHashesPerTable;
    }

public:
    CuckooHashTable() = default;
    CuckooHashTable(int numTables, int size);
    int hashFunction(int clave, int tableIndex, int hashSeedIndex);
    double factordecarga() const;
    void validarredimensionar();
    void redimensionar();
    bool insertarEnNuevaTabla(std::vector<std::vector<std::pair<int, size_t>>>& newTables, std::pair<int, size_t> tupla);
    void insertar(int clave, size_t memoryAdress);
    void insertarRecursivo(std::pair<int, size_t> tupla, int depth);
    void insertarEnOtraTabla(int clave, size_t memoryAdress, std::vector<std::vector<std::pair<int, size_t>>>& tables);
    void insertarRecursivoEnOtraTabla(std::pair<int, size_t> tupla, int depth, std::vector<std::vector<std::pair<int, size_t>>>& tables);
    std::pair<int, size_t> buscar(int clave);
    bool existe(int clave);
    void eliminar(int clave);
    void mostrar();
    ~CuckooHashTable();
};
