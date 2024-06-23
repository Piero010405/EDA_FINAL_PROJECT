#pragma once
#include "BTree.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#include <fstream>

void SerializeBTree(const BTree& btree, const std::string& outputFilename);

BTree DeserializeBTree(const std::string& inputFilename, int minimumDegree);
