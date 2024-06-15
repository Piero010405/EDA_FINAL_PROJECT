#include "CuckooHash.h"

int hashFunction0(int clave, int size)
{
    return clave % size;
}

int hashFunction1(int clave, int hashSeed, int size)
{
    return (clave ^ hashSeed) % size;
}

int hashFunction2(int clave, int hashSeed, int size)
{
    return ((clave ^ hashSeed) / size) % size;
}

int hashFunction3(int clave, int hashSeed, int size)
{
    return abs(((clave ^ hashSeed) * hashSeed / size) % size);
}

int hashFunction4(int clave, int hashSeed, int size)
{
    return ((clave ^ (clave >> 8) ^ (clave >> 16)) ^ hashSeed) % size;
}

CuckooHashTable::CuckooHashTable(int numTables, int size) : numTables(numTables), size(size), numElements(0)
{
    tables.resize(numTables);
    hashSeeds.resize(numTables);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000000, 9999999);

    for (int i = 0; i < numTables; i++)
    {
        tables[i].resize(size, { -1, 0 });
        hashSeeds[i].resize(numberHashesPerTable);
        // Insertando n Hash Seed por cada tabla
        for (int j = 0; j < numberHashesPerTable; ++j)
        {
            hashSeeds[i][j] = dis(gen);
        }
    }
}

int CuckooHashTable::hashFunction(int clave, int tableIndex, int hashSeedIndex)
{
    switch (hashSeedIndex)
    {
    case 0:
        return hashFunction1(clave, hashSeeds[tableIndex][hashSeedIndex], size);
    case 1:
        return hashFunction2(clave, hashSeeds[tableIndex][hashSeedIndex], size);
    case 2:
        return hashFunction3(clave, hashSeeds[tableIndex][hashSeedIndex], size);
    case 3:
        return hashFunction4(clave, hashSeeds[tableIndex][hashSeedIndex], size);

    default:
        return hashFunction0(clave, size);
    }
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
        newTables[i].resize(newSize, { -1, 0 });
    }

    for (int i = 0; i < numTables; i++)
    {
        for (int j = 0; j < size; ++j)
        {
            if (tables[i][j].first != -1)
            {
                if (!insertarEnNuevaTabla(newTables, tables[i][j]))
                {
                    insertarEnOtraTabla(tables[i][j].first, tables[i][j].second, newTables);
                }
            }
        }
    }

    tables = std::move(newTables);
    size = newSize;
}

bool CuckooHashTable::insertarEnNuevaTabla(std::vector<std::vector<std::pair<int, size_t>>>& newTables, std::pair<int, size_t> tupla)
{
    for (int i = 0; i < numTables; i++)
    {
        for (int j = 0; j < numberHashesPerTable; ++j)
        {
            int pos = hashFunction(tupla.first, i, j);
            if (newTables[i][pos].first == -1)
            {
                newTables[i][pos] = tupla;
                return true;
            }
        }
    }
    return false;
    // throw std::overflow_error("No se pudo insertar en una tabla redimensionada");
}

void CuckooHashTable::insertar(int clave, size_t memoryAdress)
{
    std::pair<int, size_t> tupla = { clave, memoryAdress };
    insertarRecursivo(tupla, 0);
    numElements++;
    validarredimensionar();
}

void CuckooHashTable::insertarEnOtraTabla(int clave, size_t memoryAdress, std::vector<std::vector<std::pair<int, size_t>>>& other_tables)
{
    std::pair<int, size_t> tupla = { clave, memoryAdress };
    insertarRecursivoEnOtraTabla(tupla, 0, other_tables);
    numElements++;
    validarredimensionar();
}

void CuckooHashTable::insertarRecursivo(std::pair<int, size_t> tupla, int depth)
{
    if (depth > log2(size) * numTables)
        // if (depth > size / log2(size))
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
        for (int j = 0; j < numberHashesPerTable; ++j)
        {
            int pos = hashFunction(tupla.first, i, j);
            if (tables[i][pos].first == -1)
            {
                tables[i][pos] = tupla;
                return;
            }
            else
            {
                std::pair<int, size_t> temp = tables[i][pos];
                tables[i][pos] = tupla;
                tupla = temp;
            }
        }
    }
    insertarRecursivo(tupla, depth + 1);
}

void CuckooHashTable::insertarRecursivoEnOtraTabla(std::pair<int, size_t> tupla, int depth, std::vector<std::vector<std::pair<int, size_t>>>& other_tables)
{
    if (depth > log2(size) * numTables)
        // if (depth > size / log2(size))
    {
        redimensionar();
        insertarEnOtraTabla(tupla.first, tupla.second, other_tables);
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, numTables - 1);

    for (int attempt = 0; attempt < numTables; ++attempt)
    {
        int i = dis(gen);
        for (int j = 0; j < numberHashesPerTable; ++j)
        {
            int pos = hashFunction(tupla.first, i, j);
            if (other_tables[i][pos].first == -1)
            {
                other_tables[i][pos] = tupla;
                return;
            }
            else
            {
                std::pair<int, size_t> temp = other_tables[i][pos];
                other_tables[i][pos] = tupla;
                tupla = temp;
            }
        }
    }
    insertarRecursivoEnOtraTabla(tupla, depth + 1, other_tables);
}

std::pair<int, size_t> CuckooHashTable::buscar(int clave)
{
    for (int i = 0; i < numTables; i++)
    {
        for (int j = 0; j < numberHashesPerTable; ++j)
        {
            int pos = hashFunction(clave, i, j);
            if (tables[i][pos].first != -1 && tables[i][pos].first == clave)
            {
                return tables[i][pos];
            }
        }
    }
    return { -1, 0 };
}

bool CuckooHashTable::existe(int clave)
{
    for (int i = 0; i < numTables; i++)
    {
        for (int j = 0; j < numberHashesPerTable; ++j)
        {
            int pos = hashFunction(clave, i, j);
            if (tables[i][pos].first != -1 && tables[i][pos].first == clave)
            {
                return true;
            }
        }
    }
    return false;
}

void CuckooHashTable::eliminar(int clave)
{
    for (int i = 0; i < numTables; i++)
    {
        for (int j = 0; j < numberHashesPerTable; ++j)
        {

            int pos = hashFunction(clave, i, j);
            if (tables[i][pos].first != -1 && tables[i][pos].first == clave)
            {
                tables[i][pos] = { -1, 0 };
                numElements--;
                return;
            }
        }
    }
}

void CuckooHashTable::mostrar()
{
    for (int i = 0; i < numTables; i++)
    {
        std::cout << "| Tabla " << i + 1 << " |" << std::endl;
        for (int j = 0; j < size; j++)
        {
            std::cout << "| Posicion " << j << ": ";
            if (tables[i][j].first != -1)
                std::cout << tables[i][j].first;
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
            if (tables[i][j].first != -1)
            {
                tables[i][j] = { -1, 0 };
            }
        }
    }
}
