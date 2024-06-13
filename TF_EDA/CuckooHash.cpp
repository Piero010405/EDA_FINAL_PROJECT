#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <utility>
#include <random>
#include "CuckooHash.h"

CuckooHashTable::CuckooHashTable(int numTables, int size)
{
    this->numTables = numTables;
    this->size = size;
    this->tables.resize(numTables);
    this->hashSeeds.resize(numTables);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10000);

    for (int i = 0; i < numTables; i++)
    {
        this->tables.at(i).resize(size, { -1, 0 });
        this->hashSeeds[i] = dis(gen);
    }
    this->numElements = 0;
}

int CuckooHashTable::hashFunction(int clave, int tableIndex)
{
    return (clave ^ hashSeeds[tableIndex]) % size;
}

double CuckooHashTable::factordecarga() const
{
    return static_cast<double>(numElements) / (numTables * size);
}

void CuckooHashTable::validarredimensionar()
{
    if (factordecarga() > umbralredimensionamiento)
    {
        redimensionar();
    }
}

void CuckooHashTable::redimensionar()
{
    int newSize = size * 2;

    std::vector<std::vector<std::pair<int, size_t>>> newTables(numTables);
    for (int i = 0; i < numTables; i++)
    {
        newTables.at(i).resize(newSize, { -1, 0 });
    }

    for (int i = 0; i < numTables; i++)
    {
        for (int j = 0; j < size; ++j)
        {
            if (tables.at(i)[j].first != -1)
            {
                insertarEnNuevaTabla(newTables, tables.at(i)[j]);
            }
        }
    }

    tables = move(newTables);
    size = newSize;
}

void CuckooHashTable::insertarEnNuevaTabla(std::vector<std::vector<std::pair<int, size_t>>>& newTables, std::pair<int, size_t> tupla)
{
    for (int i = 0; i < numTables; i++)
    {
        int pos = hashFunction(tupla.first, i);
        if (newTables.at(i)[pos].first == -1)
        {
            newTables.at(i)[pos] = tupla;
            return;
        }
    }
    throw std::overflow_error("No se pudo insertar en una tabla redimensionada");
}

void CuckooHashTable::insertar(int clave, size_t memoryAdress)
{
    std::pair<int, size_t> tupla = { clave, memoryAdress };
    insertarRecursivo(tupla, 0);
    numElements++;
    validarredimensionar();
}

void CuckooHashTable::insertarRecursivo(std::pair<int, size_t> tupla, int depth)
{
    if (depth > log2(size) * numTables)
    {
        redimensionar();
        insertar(tupla.first, tupla.second);
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, numTables - 1);

    for (int attempt = 0; attempt < numTables; ++attempt)
    {
        int i = dis(gen);
        int pos = hashFunction(tupla.first, i);
        if (tables.at(i)[pos].first == -1)
        {
            tables.at(i)[pos] = tupla;
            return;
        }
        else
        {
            std::pair<int, size_t> temp = tables.at(i)[pos];
            tables.at(i)[pos] = tupla;
            tupla = temp;
        }
    }
    insertarRecursivo(tupla, depth + 1);
}

std::pair<int, size_t> CuckooHashTable::buscar(int clave)
{
    for (int i = 0; i < numTables; i++)
    {
        int pos = hashFunction(clave, i);
        if (tables.at(i)[pos].first != -1 && tables.at(i)[pos].first == clave)
        {
            return tables.at(i)[pos];
        }
    }
    return { -1, 0 };
}

bool CuckooHashTable::existe(int clave)
{
    for (int i = 0; i < numTables; i++)
    {
        int pos = hashFunction(clave, i);
        if (tables.at(i)[pos].first != -1 && tables.at(i)[pos].first == clave)
        {
            return true;
        }
    }
    return false;
}

void CuckooHashTable::eliminar(int clave)
{
    for (int i = 0; i < numTables; i++)
    {
        int pos = hashFunction(clave, i);
        if (tables.at(i)[pos].first != -1 && tables.at(i)[pos].first == clave)
        {
            tables.at(i)[pos] = { -1, 0 };
            numElements--;
            return;
        }
    }
}

void CuckooHashTable::mostrar()
{
    for (int i = 0; i < this->numTables; i++)
    {
        std::cout << "| Tabla " << i + 1 << " |" << std::endl;
        for (int j = 0; j < size; j++)
        {
            std::cout << "| Posicion " << j << ": ";
            if (tables.at(i)[j].first != -1)
                std::cout << tables.at(i)[j].first;
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

CuckooHashTable::~CuckooHashTable()
{
    for (int i = 0; i < numTables; i++)
    {
        for (int j = 0; j < size; ++j)
        {
            if (tables.at(i)[j].first != -1)
            {
                tables.at(i)[j] = { -1, 0 };
            }
        }
    }
}
