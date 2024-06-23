#include "Serialization.h"

extern void SerializeBTree(const BTree& btree, const std::string& outputFilename) {
    std::ofstream outFile(outputFilename);
    if (!outFile.is_open()) {
        std::cerr << "\t\tError al abrir el archivo de salida: " << outputFilename << " - " << strerror(errno) << std::endl;
        return;
    }
    boost::archive::text_oarchive oa(outFile);
    oa << btree;
    outFile.close();
}

extern BTree DeserializeBTree(const std::string& inputFilename, int minimumDegree) {
    std::ifstream inFile(inputFilename);
    if (!inFile.is_open()) {
        std::cerr << "\t\tError al abrir el archivo de entrada: " << inputFilename << " - " << strerror(errno) << std::endl;
        throw std::runtime_error("No se pudo abrir el archivo de entrada.");
    }
    BTree btree(minimumDegree);
    boost::archive::text_iarchive ia(inFile);
    ia >> btree;
    inFile.close();
    return btree;
}
