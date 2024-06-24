#pragma once
#include <string>

const std::string POSITIVO = "si";
const int MAX_POBLACION = 99999999;
const int POBLACION = 1000000;
const int NUM_TABLAS_CUCKOO = 4;
const int GRADO_MINIMO = 195;

const std::string CIUDADANOS_FILE_NAME = "ciudadanosFile.dat";
const std::string CUCKOO_HASH_FILE_NAME = "cuckooFile.dat";
const std::string BTREE_FILE_NAME = "btreeFile.dat";

const std::vector<int> BENCH_MARKING_SIZES = { 100000,200000,300000,400000,500000,600000,700000,800000,900000,1000000,2000000,3000000,4000000,5000000, 10000000 };
const std::vector<int> INSERT_BENCH_MARKING_SIZES = { 100000,200000,500000,1000000,1500000 };
