//CuckooHashTable generarCiudadanosYTablaHash(int poblacionSize, const std::string& ciudadanosFileName, const std::string& tablaHashFileName) {
//    // Crear archivo para ciudadanos
//    std::ofstream ofsCiudadanos(ciudadanosFileName, std::ios::binary);
//
//    // Crear tabla hash de Cuckoo
//    CuckooHashTable tablaHash(NUM_TABLAS_CUCKOO, 10);
//
//    for (int i = 1; i <= poblacionSize; ++i) {
//        Ciudadano ciudadano(i);
//        // Serializar ciudadano en un stream
//        std::stringstream ss;
//        {
//            boost::archive::binary_oarchive oa(ss);
//            oa << ciudadano;
//        }
//        // Escribir el ciudadano en el archivo
//        std::string ciudadanoData = ss.str();
//        size_t memoryAddress = ofsCiudadanos.tellp();
//        size_t ciudadanoSize = ciudadanoData.size();
//        ofsCiudadanos.write(reinterpret_cast<const char*>(&ciudadanoSize), sizeof(size_t)); // Guardar el tamaño
//        ofsCiudadanos.write(ciudadanoData.c_str(), ciudadanoData.size());
//
//        // Insertar en la tabla hash
//        tablaHash.insertar(ciudadano.getId(), memoryAddress);
//    }
//
//    // Serializar la tabla hash
//    std::ofstream ofsTablaHash(tablaHashFileName, std::ios::binary);
//    boost::archive::binary_oarchive oaTablaHash(ofsTablaHash);
//    oaTablaHash << tablaHash;
//    return tablaHash;
//}
//
//CuckooHashTable cargarTablaHash(const std::string& tablaHashFileName) {
//    std::ifstream ifsTablaHash(tablaHashFileName, std::ios::binary);
//    boost::archive::binary_iarchive iaTablaHash(ifsTablaHash);
//    CuckooHashTable tablaHash;
//    iaTablaHash >> tablaHash;
//    return tablaHash;
//}
//
//CuckooHashTable cargarDatos(const std::string& ciudadanosFileName, const std::string& tablaHashFileName) {
//    std::ifstream fileStream(tablaHashFileName, std::ios::binary);
//
//    if (!fileStream.is_open()) {
//        return generarCiudadanosYTablaHash(POBLACION, ciudadanosFileName, tablaHashFileName);
//    }
//    else {
//        return cargarTablaHash(tablaHashFileName);
//    }
//}
//
//void insertarCiudadanoEnBinario(Ciudadano& nuevoCiudadano, const std::string& ciudadanosFileName, CuckooHashTable& tablaHash) {
//    // Abrir archivo en modo append
//    std::ofstream ofsCiudadanos(ciudadanosFileName, std::ios::binary | std::ios::app);
//    if (!ofsCiudadanos.is_open()) {
//        throw std::runtime_error("No se pudo abrir el archivo de ciudadanos para escribir");
//    }
//
//    // Serializar el nuevo ciudadano
//    std::stringstream ss;
//    {
//        boost::archive::binary_oarchive oa(ss);
//        oa << nuevoCiudadano;
//    }
//    std::string ciudadanoData = ss.str();
//    size_t ciudadanoSize = ciudadanoData.size();
//
//    // Escribir el tamaño y los datos del nuevo ciudadano en el archivo
//    ofsCiudadanos.write(reinterpret_cast<const char*>(&ciudadanoSize), sizeof(size_t));
//    ofsCiudadanos.write(ciudadanoData.c_str(), ciudadanoSize);
//
//    // Validar la posición después de escribir
//    size_t memoryAddressAfterWrite = ofsCiudadanos.tellp();
//
//    // Obtener la posición de memoria actual (al final del archivo)
//    size_t memoryAddress = memoryAddressAfterWrite - (ciudadanoSize + sizeof(size_t));
//
//    // Insertar en la tabla hash
//    tablaHash.insertar(nuevoCiudadano.getId(), memoryAddress);
//
//    ofsCiudadanos.close();
//}
//
//Ciudadano buscarCiudadanoPorDNI(const std::string& dni, const std::string& ciudadanosFileName, CuckooHashTable& tablaHash) {
//    int id = formatearDni(dni);
//
//    // Buscar la dirección de memoria en la tabla hash
//    std::pair<int, size_t> entry = tablaHash.buscar(id);
//    if (entry.first == -1) {
//        throw std::runtime_error("\t\tCiudadano no encontrado...\n");
//    }
//
//    size_t memoryAddress = entry.second;
//
//    // Cargar el ciudadano desde la dirección de memoria
//    boost::interprocess::file_mapping file(ciudadanosFileName.c_str(), boost::interprocess::read_only);
//    size_t ciudadanoSize;
//
//    // Leer el tamaño del ciudadano
//    {
//        boost::interprocess::mapped_region region(file, boost::interprocess::read_only, memoryAddress, sizeof(size_t));
//        std::memcpy(&ciudadanoSize, region.get_address(), sizeof(size_t));
//    }
//
//    // Leer los datos del ciudadano
//    boost::interprocess::mapped_region regionCiudadano(file, boost::interprocess::read_only, memoryAddress + sizeof(size_t), ciudadanoSize);
//    std::stringstream ss;
//    ss.write(static_cast<const char*>(regionCiudadano.get_address()), ciudadanoSize);
//
//    Ciudadano ciudadano;
//    {
//        boost::archive::binary_iarchive ia(ss);
//        ia >> ciudadano;
//    }
//
//    return ciudadano;
//}