#include "CuckooHash.h"

std::vector<std::pair<int, size_t>> TUPLES_NOT_INSERTED;

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

std::vector<int> shuffle(std::vector<int> vec)
{
    srand(time(NULL));
    std::vector<int> svec = vec;
    int len = svec.size();
    for (int i = 0; i < len; ++i)
    {
        int tmp = (rand() % (len - 1 - 0 + 1)) + 0;
        std::swap(svec[i], svec[tmp]);
    }
    return svec;
}

CuckooHashTable::CuckooHashTable(int numTables, int size) : numTables(numTables), size(size), numElements(0)
{
    srand(time(NULL));
    tables.resize(numTables);
    hashSeeds.resize(numTables);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1000000, 9999999);

    for (int i = 0; i < numTables; i++)
    {
        tables[i].resize(size, { -1, 0 });
        hashSeeds[i].resize(numberHashesPerTable);
        for (int j = 0; j < numberHashesPerTable; ++j)
        {
            hashSeeds[i][j] = dis(gen);
        }
    }
}

std::vector<int> CuckooHashTable::getTablesIndex()
{
    std::vector<int> tablesIndex;
    for (int i = 0; i < numTables; i++)
    {
        tablesIndex.push_back(i);
    }
    return tablesIndex;
}

int CuckooHashTable::hashFunction(int clave, int tableIndex, int hashSeedIndex)
{
    if (hashSeedIndex < 0 || hashSeedIndex >= numberHashesPerTable)
    {
        throw std::out_of_range("hashSeedIndex fuera de rango");
    }

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
    std::vector<std::vector<std::pair<int, size_t>>> newTables(numTables, std::vector<std::pair<int, size_t>>(newSize, { -1, 0 }));

    for (int i = 0; i < numTables; i++)
    {
        for (int j = 0; j < size; ++j)
        {
            if (tables[i][j].first != -1)
            {
                if (!insertarEnNuevaTabla(newTables, tables[i][j]))
                {
                    insertarIterativoEnOtraTabla(tables[i][j], newTables);
                }
            }
        }
    }

    tables = std::move(newTables);
    if (TUPLES_NOT_INSERTED.size() > 0)
    {
        for (int i = 0; i < TUPLES_NOT_INSERTED.size(); i++)
        {
            insertar(TUPLES_NOT_INSERTED[i].first, TUPLES_NOT_INSERTED[i].second, false);
        }
    }
    TUPLES_NOT_INSERTED.clear();
    size = newSize;
}

void CuckooHashTable::insertar(int clave, size_t memoryAdress, bool notInserted)
{
    std::pair<int, size_t> tupla = { clave, memoryAdress };
    insertarIterativo(tupla);
    if (notInserted)
    {
        numElements++;
    }
    validarredimensionar();
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
}

void CuckooHashTable::insertarRecursivo(std::pair<int, size_t> tupla, int depth)
{
    // if (depth > log2(size) * numTables)
    if (depth > size / log2(size))
        // if ((size / log2(size)) * numTables)
    {
        redimensionar();
        insertar(tupla.first, tupla.second);
        return;
    }

    std::vector<int> shuffleIndex = shuffle(getTablesIndex());

    for (int attempt = 0; attempt < numTables; ++attempt)
    {
        int i = shuffleIndex[attempt];
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
    // if (depth > log2(size) * numTables)
    if (depth > size / log2(size))
        // if ((size / log2(size)) * numTables)
    {
        TUPLES_NOT_INSERTED.push_back(tupla);
        return;
    }
    std::vector<int> shuffleIndex = shuffle(getTablesIndex());

    for (int attempt = 0; attempt < numTables; ++attempt)
    {
        int i = shuffleIndex[attempt];
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

void CuckooHashTable::insertarIterativo(std::pair<int, size_t> tupla)
{
    int depth = 0;

    while (true)
    {
        // if (depth > log2(size) * numTables)
        if (depth > size / log2(size))
            // if ((size / log2(size)) * numTables)
        {
            redimensionar();
            insertar(tupla.first, tupla.second);
            return;
        }

        std::vector<int> shuffleIndex = shuffle(getTablesIndex());
        bool inserted = false;

        for (int attempt = 0; attempt < numTables && !inserted; ++attempt)
        {
            int i = shuffleIndex[attempt];
            for (int j = 0; j < numberHashesPerTable && !inserted; ++j)
            {
                int pos = hashFunction(tupla.first, i, j);
                if (tables[i][pos].first == -1)
                {
                    tables[i][pos] = tupla;
                    inserted = true;
                }
                else
                {
                    std::swap(tupla, tables[i][pos]);
                }
            }
        }

        if (inserted)
        {
            break;
        }

        ++depth;
    }
}

void CuckooHashTable::insertarIterativoEnOtraTabla(std::pair<int, size_t> tupla, std::vector<std::vector<std::pair<int, size_t>>>& other_tables)
{
    int depth = 0;

    while (true)
    {
        // if (depth > log2(size) * numTables)
        if (depth > size / log2(size))
            // if ((size / log2(size)) * numTables)
        {
            TUPLES_NOT_INSERTED.push_back(tupla);
            return;
        }

        std::vector<int> shuffleIndex = shuffle(getTablesIndex());
        bool inserted = false;

        for (int attempt = 0; attempt < numTables && !inserted; ++attempt)
        {
            int i = shuffleIndex[attempt];
            for (int j = 0; j < numberHashesPerTable && !inserted; ++j)
            {
                int pos = hashFunction(tupla.first, i, j);
                if (other_tables[i][pos].first == -1)
                {
                    other_tables[i][pos] = tupla;
                    inserted = true;
                }
                else
                {
                    std::swap(tupla, other_tables[i][pos]);
                }
            }
        }

        if (inserted)
        {
            break;
        }

        ++depth;
    }
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
