#pragma once
#include <iostream>
#include <cstdlib>
#include <vector>
#include <cctype>
#include <string>
#include <ctime>
#include <random>
#include <utility>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/string.hpp>
#include "Ciudadano.h"

struct Departamento
{
    std::string nombre;
    std::vector<std::string> ciudades;
};

std::string toLowerCase(const std::string& str)
{
    std::string resultado = str;
    for (size_t i = 0; i < resultado.length(); ++i)
    {
        resultado[i] = tolower(static_cast<unsigned char>(resultado[i]));
    }
    return resultado;
}

int getRandom(std::vector<std::string> arr)
{
    int random = rand() % arr.size();
    return random;
}

int getRandomNacionality(std::vector<std::string> arr, const double probabilidad = 0.9609) // Tasa de innimgrantes (2023) = 3.91%
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0, 1.0);

    double valorAleatorio = dist(gen);

    if (valorAleatorio < probabilidad)
    {
        return 0;
    }
    else
    {
        int indiceAleatorio = rand() % (arr.size() - 1) + 1;
        return indiceAleatorio;
    }
}

int getRandomDepartament(std::vector<Departamento> arr)
{
    int random = rand() % arr.size();
    return random;
}

const std::vector<std::string> estadosCiviles_lst = { "CASADO", "SOLTERO", "DIVORCIADO", "VIUDO" };

const std::vector<std::string> nombres_lst = {
    "MARIA", "JOSE", "JUAN", "LUIS", "CARLOS", "ROSA", "JORGE", "VICTOR", "ANA", "LUZ",
    "CESAR", "CARMEN", "MIGUEL", "JULIO", "JESUS", "PEDRO", "MANUEL", "JUANA", "SANTOS", "SEGUNDO",
    "JHON", "DAVID", "ANGEL", "JULIA", "DIEGO", "FLOR", "ELIZABETH", "DANIEL", "OSCAR", "RUTH",
    "DIANA", "MARCO", "EDWIN", "JAVIER", "WALTER", "FRANCISCO", "FERNANDO", "MILAGROS", "ALEJANDRO", "RAUL",
    "RICARDO", "ALEX", "ROBERTO", "EDGAR", "VICTORIA", "EDUARDO", "MARIO", "JAIME", "GLORIA", "ANDREA",
    "NANCY", "GLADYS", "SONIA", "FELIX", "ISABEL", "HECTOR", "MARTHA", "PABLO", "CLAUDIA", "SILVIA",
    "KEVIN", "MARGARITA", "SANDRA", "ALEXANDER", "PATRICIA", "JEAN", "ROCIO", "CRISTIAN", "ANTONIO", "ALBERTO",
    "HUGO", "SEBASTIAN", "ANDRES", "YOLANDA", "NORMA", "JESSICA", "MERCEDES", "PERCY", "OLGA", "ANGELA",
    "CHRISTIAN", "EDITH", "LUCIA", "KAREN", "RICHARD", "ROXANA", "VILMA", "HILDA", "LIDIA", "RUBEN",
    "ALFREDO", "JOEL", "ERIKA", "RONALD", "ERICK", "TERESA", "ALICIA", "GUILLERMO", "CAMILA", "JOSUE",
    "ROSARIO", "LEONARDO", "NELLY", "KARINA", "ELMER", "MARIBEL", "KATHERINE", "ELSA", "LUISA", "RODRIGO",
    "DORIS", "FREDY", "MARCOS", "ARIANA", "SERGIO", "WILMER", "ANGELICA", "FRANK", "MARITZA", "MONICA",
    "HENRY", "MIRIAM", "WILLIAM", "SANTIAGO", "LILIANA", "GABRIEL", "LIZ", "RAFAEL", "MOISES", "ELENA",
    "SARA", "ROGER", "FRANCISCA", "GABRIELA", "MARTIN", "ENRIQUE", "BLANCA", "ESTHER", "LOURDES", "CRISTHIAN",
    "VERONICA", "ANTHONY", "MARY", "ELVIS", "BERTHA", "KIARA", "ADRIANA", "THIAGO", "JHONATAN", "BRAYAN",
    "DELIA", "VALERIA", "GUSTAVO", "MAXIMO", "CECILIA", "IVAN", "IRMA", "DANIELA", "ADRIAN", "SOFIA",
    "WILFREDO", "ANGIE", "SUSANA", "NICOLAS", "SAMUEL", "RENZO", "ROLANDO", "PIERO", "FELIPE", "BEATRIZ",
    "EVELYN", "JUNIOR", "NOEMI", "KARLA", "WILSON", "CRISTINA", "JUDITH", "WILDER", "ALEXANDRA", "ABEL",
    "MARINA", "FIORELLA", "ANDERSON", "OMAR", "LUCIANA", "ELIAS", "JENNY", "ALVARO", "ORLANDO", "LAURA",
    "DEYSI", "GRACIELA", "JULIAN", "ALEJANDRA", "DINA", "FRANKLIN", "TANIA", "GREGORIO", "MAYRA", "PAOLA" };

const std::vector<std::string> apellidosPeruanos = {
    "QUISPE", "FLORES", "SANCHEZ", "GARCIA", "RODRIGUEZ", "ROJAS", "HUAMAN",
    "MAMANI", "VASQUEZ", "RAMOS", "LOPEZ", "TORRES", "PEREZ", "DIAZ",
    "GONZALES", "RAMIREZ", "MENDOZA", "CHAVEZ", "ESPINOZA", "CASTILLO",
    "FERNANDEZ", "GUTIERREZ", "VARGAS", "CRUZ", "RUIZ", "DE LA CRUZ",
    "ROMERO", "GOMEZ", "CONDORI", "CASTRO", "SILVA", "CORDOVA", "MARTINEZ",
    "RIVERA", "REYES", "SALAZAR", "MEDINA", "AGUILAR", "PAREDES", "LEON",
    "MORALES", "PALOMINO", "HUAMANI", "HERRERA", "CAMPOS", "RIOS", "PEÑA",
    "VEGA", "CARDENAS", "DELGADO", "ALVAREZ", "SOTO", "CALDERON",
    "VELASQUEZ", "GUEVARA", "JIMENEZ", "MUÑOZ", "VILLANUEVA", "NUÑEZ",
    "ALVARADO", "APAZA", "ORTIZ", "SAAVEDRA", "MEZA", "NAVARRO", "SANDOVAL",
    "GUERRERO", "HERNANDEZ", "SALAS", "CABRERA", "PALACIOS", "VILCA",
    "DAVILA", "ZAPATA", "MORENO", "CONTRERAS", "CARRASCO", "SANTOS",
    "MEJIA", "MIRANDA", "VERA", "AGUIRRE", "JARA", "SUAREZ", "ALARCON",
    "AYALA", "ARIAS", "GUERRA", "VILCHEZ", "CACERES", "GUZMAN", "BRAVO",
    "CUEVA", "LOZANO", "PACHECO", "CHOQUE", "BAUTISTA", "JUAREZ", "TELLO",
    "QUIROZ", "ROSALES", "VALENCIA", "PAUCAR", "HUANCA", "TAPIA", "PINEDO",
    "CASTAÑEDA", "CARBAJAL", "ACOSTA", "LUNA", "VENTURA", "TRUJILLO",
    "VALDIVIA", "DOMINGUEZ", "PONCE", "BUSTAMANTE", "RIVAS", "GAMARRA",
    "VILLEGAS", "AQUINO", "HURTADO", "SALDAÑA", "BENITES", "ZEVALLOS",
    "AREVALO", "JULCA", "VALVERDE", "ACUÑA", "SALINAS", "TICONA", "POMA",
    "PERALTA", "INGA", "CARRANZA", "VELA", "AVILA", "SOLIS", "OCHOA",
    "FIGUEROA", "ROMAN", "ESCOBAR", "HIDALGO", "ESTRADA", "ALVA", "BECERRA",
    "CORREA", "RENGIFO", "ALFARO", "ZUÑIGA", "ROBLES", "FARFAN", "LAURA",
    "PUMA", "MARIN", "ORTEGA", "SOSA", "LLANOS", "ZEGARRA", "ROSAS",
    "MELENDEZ", "ROQUE", "CALLE", "CORNEJO", "MALDONADO", "GALVEZ", "CUBAS",
    "TAIPE", "BAZAN", "PAZ", "ZAVALETA", "LOAYZA", "ARROYO", "ALIAGA",
    "MONTES", "MENDEZ", "SALCEDO", "CORTEZ", "YUPANQUI", "LINARES",
    "GUILLEN", "COTRINA", "CARRILLO", "CHIPANA", "MANRIQUE", "MOLINA",
    "OSORIO", "PARIONA", "PADILLA", "ALTAMIRANO", "SOLANO", "ORE",
    "CABALLERO", "PRADO", "CHAMBI", "CAMACHO", "PANDURO", "MONTOYA",
    "VALDEZ", "ARAUJO", "LAZO", "LAZARO", "ANGULO", "FUENTES", "DEL AGUILA",
    "CORONADO", "MORI", "HINOSTROZA", "CHACON", "NEYRA", "AVALOS", "BLAS",
    "SALVADOR", "ZARATE", "GAMBOA", "MARQUEZ", "ALCANTARA", "GARAY",
    "MAYTA", "HUILLCA", "ABANTO", "SEGURA", "CORONEL", "CONDOR", "ARCE",
    "DURAND", "TERRONES", "POLO", "GONZALEZ", "TUANAMA", "CARRION",
    "ARTEAGA", "YUCRA", "HUARCAYA", "TITO", "SARMIENTO", "CERNA", "SANTIAGO",
    "ZAVALETA", "MACHACA", "OJEDA", "QUINTANA", "MEJIA", "LUQUE", "DURAN",
    "CRUZADO", "VIDAL", "MONTECINO", "PARI", "CESPEDES", "HUERTA", "MACEDO",
    "NINA", "BRICEño", "TARAZONA", "ORDOñEZ", "LUJAN", "PIZARRO", "TAPULLIMA",
    "ZAMORA", "TEJADA", "CABANILLAS", "MERINO", "REATEGUI", "GALLARDO",
    "BARRIENTOS", "PORRAS", "CANO", "PINTO", "ESCALANTE", "SANGAMA",
    "ENRIQUEZ", "HANCCO", "SULCA", "VILLALOBOS", "TINEO", "BARRETO",
    "GIL", "CISNEROS", "ARANDA", "VALDERRAMA", "OLIVERA", "FALCON",
    "MORE", "OBREGON", "BARRIOS", "CHINCHAY", "CARLOS", "SANTISTEBAN",
    "MOZOMBITE", "ABAD", "BACA", "VALENZUELA", "CARPIO", "CHURA", "LARA",
    "ZAMBRANO", "TAFUR", "VICENTE", "OLIVARES", "PIZANGO", "MELGAREJO",
    "HUAYTA", "CUBA", "HUALLPA", "CAHUANA", "CASAS", "MAZA", "HILARIO",
    "BENDEZU", "MONTALVO", "LIMA", "VERGARA", "SANTA CRUZ", "LEYVA",
    "ANDRADE", "MONTERO", "VALLEJOS", "SIFuentes", "CUTIPA", "NEIRA",
    "VALLE", "MALLQUI", "SANTILLAN", "GALLEGOS", "LEIVA", "TOLENTINO",
    "TUESTA", "CANALES", "YAURI", "VILLACORTA", "DAMIAN", "CHUNGA",
    "SAENZ", "COAQUIRA", "ARMAS", "RUBIO", "VELARDE", "SEMINARIO",
    "SANTAMARIA", "CUADROS", "QUIÑONES", "BARBOZA", "MATOS", "BENAVIDES",
    "JARAMILLO", "BALDEON", "CHERO", "HEREDIA", "ARANA" };

const std::vector<std::string> apellidosVenezolanos = {
    "GONZALEZ", "RODRIGUEZ", "PEREZ", "HERNANDEZ", "GARCIA", "MARTINEZ",
    "SANCHEZ", "LOPEZ", "DIAZ", "ROJAS", "RAMIREZ", "CASTILLO", "GOMEZ",
    "ROMERO", "FERNANDEZ", "TORRES", "MENDOZA", "MEDINA", "MORENO",
    "GUTIERREZ", "JIMENEZ", "RIVAS", "ALVAREZ", "SALAZAR", "MARQUEZ",
    "SUAREZ", "FLORES", "SILVA", "PEÑA", "CONTRERAS", "MORALES", "HERRERA",
    "VARGAS", "MENDEZ", "PARRA", "RUIZ", "RAMOS", "BLANCO", "RIVERO",
    "QUINTERO", "BRICEÑO", "REYES", "ZAMBRANO", "VASQUEZ", "LEON",
    "DELGADO", "VELASQUEZ", "RONDON", "MARCANO", "CASTRO", "MOLINA",
    "ACOSTA", "TOVAR", "BARRIOS", "ALVARADO", "GIL", "MARIN", "MORA",
    "COLMENARES", "ESCALONA", "BRITO", "LUGO", "RANGEL", "MUÑOZ",
    "TORREALBA", "PACHECO", "GUERRERO", "GUZMAN", "DURAN", "URDANETA",
    "MONTILLA", "ORTIZ", "CHIRINOS", "NUÑEZ", "GUEVARA", "ORTEGA",
    "CHACON", "SOTO", "ESPINOZA", "GUERRA", "BOLIVAR", "SALAS", "COLINA",
    "CAMPOS", "CEDEÑO", "FIGUEROA", "LEAL", "ARIAS", "CAMACHO", "AGUILAR",
    "PINTO", "MORILLO", "CORDERO", "BRAVO", "ARAUJO", "LINARES", "RINCON",
    "PEREIRA", "NAVARRO", "PINEDA" };

const std::vector<std::string> apellidosColombianos = {
    "ACEVEDO", "ACOSTA", "AGUDELO", "ÁLVAREZ", "ARANGO", "ARIAS", "ÁVILA",
    "BARRERA", "BEDOYA", "BELTRÁN", "BERNAL", "BUITRAGO", "CAICEDO", "CALDERÓN",
    "CÁRDENAS", "CARDONA", "CARVAJAL", "CASTILLO", "CASTRO", "CONTRERAS", "CORREA",
    "CORTES", "CRUZ", "DELGADO", "DÍAZ", "DUQUE", "ESCOBAR", "FERNÁNDEZ",
    "FLÓREZ", "FORERO", "FRANCO", "GARCÍA", "GARZÓN", "GIL", "GIRALDO",
    "GÓMEZ", "GONZÁLEZ", "GUERRERO", "GUTIÉRREZ", "GUZMÁN", "HENAO", "HERNÁNDEZ",
    "HERRERA", "HURTADO", "JARAMILLO", "JIMÉNEZ", "LEÓN", "LONDOÑO", "LÓPEZ",
    "LOZANO", "MARÍN", "MARTÍNEZ", "MEDINA", "MEJÍA", "MÉNDEZ", "MENDOZA",
    "MOLINA", "MONTOYA", "MORA", "MORALES", "MORENO", "MOSQUERA", "MUÑOZ",
    "OROZCO", "ORTEGA", "ORTIZ", "OSORIO", "OSPINA", "PARRA", "PEÑA",
    "PÉREZ", "PINEDA", "PINZÓN", "QUINTERO", "RAMÍREZ", "RAMOS", "RESTREPO",
    "REYES", "RINCÓN", "RÍOS", "RIVERA", "RODRÍGUEZ", "ROJAS", "ROMERO",
    "RUIZ", "SALAZAR", "SÁNCHEZ", "SIERRA", "SILVA", "SUÁREZ", "TORRES",
    "TRUJILLO", "URIBE", "VALENCIA", "VARGAS" };

const std::vector<std::string> extensionEmail_lst = { "@gmail.com", "@hotmail.com", "@outlook.com", "@yahoo.com", "@icloud.com" };

const std::vector<std::string> nacionalidades_lst = { "PERUANA", "VENEZOLANA", "COLOMBIANA" };

const std::vector<std::string> paises_lst = { "Peru", "Venezuela", "Colombia" };

const std::vector<Departamento> departamentosPeru_lst = {
    {"Amazonas", {"Bagua Grande", "Chachapoyas", "Bagua"}},
    {"Áncash", {"Chimbote", "Huaraz", "Huarmey", "Casma", "Caraz"}},
    {"Apurímac", {"Abancay", "Andahuaylas"}},
    {"Arequipa", {"Arequipa", "Camaná", "Mollendo", "Mejía", "Majes"}},
    {"Ayacucho", {"Ayacucho", "Puquio", "Huanta", "Coracora", "Vilcashuamán", "Cangallo"}},
    {"Cajamarca", {"Cajamarca", "Jaén", "Cutervo", "Chota", "Celendín", "Cajabamba", "Bambamarca", "San Ignacio"}},
    {"Callao", {"Callao"}},
    {"Cusco", {"Cusco", "Sicuani", "Quillabamba"}},
    {"Huancavelica", {"Huancavelica", "Pampas"}},
    {"Huánuco", {"Huánuco", "Tingo María"}},
    {"Ica", {"Ica", "Chincha Alta", "Pisco", "Nazca", "Paracas"}},
    {"Junín", {"Huancayo", "Tarma", "Jauja", "La Oroya", "La Merced", "Satipo", "Pichanaqui"}},
    {"La Libertad", {"Trujillo", "Huanchaco", "Huamachuco", "Virú", "Chepén", "Pacasmayo", "Guadalupe"}},
    {"Lambayeque", {"Chiclayo", "Lambayeque", "Ferreñafe", "Monsefú", "Chongoyape"}},
    {"Lima", {"Lima", "Huacho", "Huaral", "Cañete", "Barranca", "Chancay", "Paramonga", "Yauyos"}},
    {"Loreto", {"Iquitos", "Yurimaguas", "Nauta"}},
    {"Madre de Dios", {"Puerto Maldonado"}},
    {"Moquegua", {"Moquegua", "Ilo"}},
    {"Pasco", {"Cerro de Pasco", "Oxapampa"}},
    {"Piura", {"Piura", "Sullana", "Paita", "Talara", "Catacaos", "Chulucanas", "Sechura"}},
    {"Puno", {"Juliaca", "Puno", "Ilave", "Ayaviri", "Azángaro"}},
    {"San Martín", {"Tarapoto", "Moyobamba", "Juanjuí", "Rioja"}},
    {"Tacna", {"Tacna"}},
    {"Tumbes", {"Tumbes", "Zarumilla"}},
    {"Ucayali", {"Pucallpa"}} };

const std::vector<Departamento> departamentosVenezuela_lst = {
    {"Caracas", {"Distrito Capital", "Miranda"}},
    {"Maracaibo", {"Zulia"}},
    {"Valencia", {"Carabobo"}},
    {"Barquisimeto", {"Lara"}},
    {"Maracay", {"Aragua"}},
    {"Ciudad Guayana", {"Bolívar"}},
    {"Maturín", {"Monagas"}},
    {"Barinas", {"Barinas"}},
    {"San Cristóbal", {"Táchira"}},
    {"Ciudad Bolívar", {"Bolívar"}},
    {"Barcelona", {"Anzoátegui"}},
    {"Cumaná", {"Sucre"}},
    {"Cabimas", {"Zulia"}},
    {"Mérida", {"Mérida"}},
    {"Puerto La Cruz", {"Anzoátegui"}},
    {"Guatire", {"Miranda"}},
    {"Ciudad Ojeda", {"Zulia"}},
    {"Punto Fijo", {"Falcón"}},
    {"Coro", {"Falcón"}},
    {"Turmero", {"Aragua"}},
    {"Los Teques", {"Miranda"}},
    {"Guanare", {"Portuguesa"}},
    {"San Felipe", {"Yaracuy"}},
    {"Acarigua", {"Portuguesa"}},
    {"Carora", {"Lara"}},
    {"El Tigre", {"Anzoátegui"}},
    {"Guarenas", {"Miranda"}},
    {"Cabudare", {"Lara"}},
    {"Carúpano", {"Sucre"}},
    {"San Fernando de Apure", {"Apure"}},
    {"Guacara", {"Carabobo"}},
    {"Puerto Cabello", {"Carabobo"}},
    {"El Tocuyo", {"Lara"}},
    {"Valera", {"Trujillo"}},
    {"La Victoria", {"Aragua"}},
    {"Santa Rita", {"Aragua"}},
    {"Güigüe", {"Carabobo"}},
    {"Anaco", {"Anzoátegui"}},
    {"Calabozo", {"Guárico"}},
    {"Quibor", {"Lara"}},
    {"El Vigía", {"Mérida"}},
    {"Palo Negro", {"Aragua"}},
    {"San Carlos", {"Cojedes"}},
    {"Mariara", {"Carabobo"}},
    {"Villa de Cura", {"Aragua"}},
    {"Ocumare del Tuy", {"Miranda"}},
    {"Yaritagua", {"Yaracuy"}},
    {"Cúa", {"Miranda"}},
    {"Araure", {"Portuguesa"}},
    {"San Juan de los Morros", {"Guárico"}},
    {"Táriba", {"Táchira"}},
    {"Guasdualito", {"Apure"}},
    {"Puerto Ayacucho", {"Amazonas"}},
    {"Machiques", {"Zulia"}},
    {"Cagua", {"Aragua"}},
    {"Porlamar", {"Nueva Esparta"}},
    {"Charallave", {"Miranda"}},
    {"La Asunción", {"Nueva Esparta"}},
    {"Valle de la Pascua", {"Guárico"}},
    {"Santa Lucía", {"Miranda"}},
    {"Trujillo", {"Trujillo"}},
    {"Quíbor", {"Nueva Esparta"}},
    {"Tinaquillo", {"Cojedes"}},
    {"Puerto Píritu", {"Anzoátegui"}},
    {"El Limón", {"Aragua"}},
    {"Socopó", {"Barinas"}},
    {"Boconó", {"Trujillo"}},
    {"Punta de Mata", {"Monagas"}} };

const std::vector<Departamento> departamentosColombia_lst = {
    {"Antioquia", {"Apartadó", "Medellín"}},
    {"Arauca", {"Arauca", "Arauquita"}},
    {"Quindío", {"Armenia"}},
    {"Atlántico", {"Barranquilla"}},
    {"Bogotá", {"Bogotá"}},
    {"Santander", {"Bucaramanga"}},
    {"Valle del Cauca", {"Cali"}},
    {"Bolívar", {"Cartagena"}},
    {"Cundinamarca", {"Chía", "Girardot", "Soacha"}},
    {"Norte de Santander", {"Cúcuta"}},
    {"Caquetá", {"Florencia"}},
    {"Tolima", {"Ibagué"}},
    {"Guainía", {"Inírida"}},
    {"Amazonas", {"Leticia"}},
    {"Caldas", {"Manizales"}},
    {"Antioquia", {"Medellín"}},
    {"Córdoba", {"Montería"}},
    {"Huila", {"Neiva", "Pitalito"}},
    {"Nariño", {"Pasto"}},
    {"Risaralda", {"Pereira", "Santa Rosa de Cabal"}},
    {"Cauca", {"Popayán"}},
    {"Chocó", {"Quibdó"}},
    {"La Guajira", {"Riohacha"}},
    {"Magdalena", {"Santa Marta"}},
    {"Risaralda", {"Santa Rosa de Cabal"}},
    {"Sucre", {"Sincelejo"}},
    {"Boyacá", {"Tunja"}},
    {"Cesar", {"Valledupar"}},
    {"Meta", {"Villavicencio"}},
    {"Casanare", {"Yopal"}} };

const std::vector<std::string> direcciones_lst = {
    "Avenida Comodoro Rivadavia N° 2797 Formosa 3600, Formosa",
    "Blv. Ciudad Autónoma de Buenos Aires N° 142 Chilecito 5360, La Rioja",
    "Calle Mar del Plata N° 24 San Ferando del Valle de Catamarca 4700, Catamarca",
    "Diagonal Omar Nuñez N° 5983 San Luis 5700, San Luis",
    "Av. 2 N° 9796 Paraná 3100, Entre Ríos",
    "Diagonal San Luis N° 461 Formosa 3600, Formosa",
    "Diagonal 4 N° 8387 Local 13 Santa Rosa 6300, La Pampa",
    "Av. Rivadavia N° 646 Oficina 8 Río Gallegos 9400, Santa Cruz",
    "Av. Entre Ríos N° 1564 Córdoba 5000, Córdoba",
    "Calle Saavedra N° 72 La Rioja 5300, La Rioja",
    "Blv. Omar Nuñez N° 58 Torre 4 Dto. 7 Paraná 3100, Entre Ríos",
    "Diagonal 3 N° 424 Piso 3 Dto. 7 Rawson 9103, Chubut",
    "Calle Saavedra N° 5638 Local 7 Comodoro Rivadavia 9000, Chubut",
    "Avenida Córdoba N° 593 La Rioja 5300, La Rioja",
    "Calle Malvinas Argentinas N° 12 Río Gallegos 9400, Santa Cruz",
    "Avenida 8 N° 172 Torre 3 Dto. 2 Resistencia 3500, Chaco",
    "Diag. Santa Cruz N° 632 Comodoro Rivadavia 9000, Chubut",
    "Boulevard 199 A N° 747 Torre 9 Dto. 7 Santa Rosa 6300, La Pampa",
    "Diagonal La Rioja N° 4072 Torre 2 Dto. 2 Salta 4400, Salta",
    "Av. Malvinas Argentinas N° 525 Torre 4 Dto. 9 Constitución 1004, Ciudad Autónoma de Buenos Aires",
    "Avenida Formosa N° 384 Paraná 3100, Entre Ríos",
    "Diagonal Merlo N° 587 La Rioja 5300, La Rioja",
    "Blv. Córdoba N° 876 Córdoba 5000, Córdoba",
    "Calle 2 N° 91 Mar del Plata 7600, Buenos Aires",
    "Boulevard San Martin N° 25 Piso 6 Dto. 5 San Miguel de Tucumán 4000, Tucumán",
    "Av. La Rioja N° 4632 Córdoba 5000, Córdoba",
    "Boulevard Malvinas Argentinas N° 96 Dto. 5 Resistencia 3500, Chaco",
    "Camino J.J. Castelli N° 84 Piso 1 Dto. 7 Santa Rosa 6300, La Pampa",
    "Boulevard Rawson N° 4373 San Salvador de Jujuy 4600, Jujuy",
    "Avenida Santa Fe N° 648 Piso 1 Dto. 4 San Ferando del Valle de Catamarca 4700, Catamarca",
    "Boulevard 8 N° 591 Dto. 6 Santa Fe 3000, Santa Fe",
    "Avenida Santiago del Estero N° 958 Piso 5 Dto. 6 Posadas 3300, Misiones",
    "Diagonal 7 N° 9013 Mendoza 5500, Mendoza",
    "Diagonal Corrientes N° 44 Piso 8 Dto. 9 San Ferando del Valle de Catamarca 4700, Catamarca",
    "Av. 3 N° 11 Piso 7 Dto. 7 Comodoro Rivadavia 9000, Chubut",
    "Boulevard Entre Ríos N° 8832 Piso 5 Dto. 3 Santa Fe 3000, Santa Fe",
    "Diagonal Córdoba N° 98 Paraná 3100, Entre Ríos",
    "Av. Santiago del Estero N° 48 La Plata 1900, Buenos Aires",
    "Boulevard San Martin N° 48 Mendoza 5500, Mendoza",
    "Diagonal 4 N° 3211 Piso 1 Dto. 7 San Miguel de Tucumán 4000, Tucumán",
    "Avenida 2 N° 558 Corrientes 3400, Corrientes",
    "Diagonal Santa Fe N° 9718 Piso 8 Dto. 7 Córdoba 5000, Córdoba",
    "Diagonal Malvinas Argentinas N° 347 Local 8 Santa Fe 3000, Santa Fe",
    "Camino Bahía Blanca N° 3577 Corrientes 3400, Corrientes",
    "Avenida Santa Cruz N° 599 Río Gallegos 9400, Santa Cruz",
    "Diag. Chaco N° 297 La Plata 1900, Buenos Aires",
    "Calle San Martin N° 33 Piso 2 Dto. 6 Santa Fe 3000, Santa Fe",
    "Avenida Viedma N° 185 Oficina 2 San Miguel de Tucumán 4000, Tucumán",
    "Av. Pte. Perón N° 866 Piso 3 Dto. 9 Santa Fe 3000, Santa Fe",
    "Blv. 8 N° 767 Torre 1 Dto. 8 Constitución 1004, Ciudad Autónoma de Buenos Aires",
    "Avenida Neuquén N° 219 Dto. 3 Ushuaia 9410, Tierra del Fuego",
    "Diag. San Martin N° 46 Local 1 Constitución 1004, Ciudad Autónoma de Buenos Aires",
    "Avenida Saavedra N° 8344 Resistencia 3500, Chaco",
    "Avenida Malvinas Argentinas N° 161 Dto. 9 Bahía Blanca 8000, Buenos Aires",
    "Av. Corrientes N° 713 Piso 6 Dto. 8 Neuquén 8300, Neuquén",
    "Diagonal 1 N° 61 Oficina 39 San Miguel de Tucumán 4000, Tucumán",
    "Av. J.J. Castelli N° 481 Local 6 Ushuaia 9410, Tierra del Fuego",
    "Av. San Luis N° 86 Posadas 3300, Misiones",
    "Av. Entre Ríos N° 3185 Piso 6 Dto. 7 Ushuaia 9410, Tierra del Fuego",
    "Diagonal Catamarca N° 905 San Miguel de Tucumán 4000, Tucumán",
    "Av. 139 Bis N° 17 La Rioja 5300, La Rioja",
    "Diagonal San Luis N° 508 Piso 2 Dto. 3 Mendoza 5500, Mendoza",
    "Calle J.B. Alberdi N° 288 San Juan 5400, San Juan",
    "Av. Saavedra N° 7361 Salta 4400, Salta",
    "Diagonal Salta N° 599 Santiago del Estero 4200, Santiago del Estero",
    "Calle J.B. Alberdi N° 863 Dto. 5 Posadas 3300, Misiones",
    "Diag. Chaco N° 7360 Local 51 San Salvador de Jujuy 4600, Jujuy",
    "Calle Saavedra N° 9872 Bahía Blanca 8000, Buenos Aires",
    "Boulevard Jujuy N° 229 Dto. 5 Santa Fe 3000, Santa Fe",
    "Diagonal Santiago del Estero N° 793 Local 10 Viedma 8500, Río Negro",
    "Calle Chaco N° 5559 Oficina 4 Río Gallegos 9400, Santa Cruz",
    "Avenida San Juan N° 754 Córdoba 5000, Córdoba",
    "Av. Belgrano N° 633 Piso 2 Dto. 2 Neuquén 8300, Neuquén",
    "Calle San Miguel de Tucumán N° 289 Posadas 3300, Misiones",
    "Calle 6 N° 88 Oficina 54 Formosa 3600, Formosa",
    "Diagonal Neuquén N° 37 San Miguel de Tucumán 4000, Tucumán",
    "Av. Malvinas Argentinas N° 565 San Salvador de Jujuy 4600, Jujuy",
    "Boulevard Chaco N° 546 San Luis 5700, San Luis",
    "Av. Alvear N° 212 La Rioja 5300, La Rioja",
    "Camino Entre Ríos N° 5588 Ushuaia 9410, Tierra del Fuego",
    "Avenida Rawson N° 624 Santa Rosa 6300, La Pampa",
    "Camino Rosario N° 59 Dto. 3 Río Gallegos 9400, Santa Cruz",
    "Diag. Río Negro N° 76 Corrientes 3400, Corrientes",
    "Avenida 4 N° 21 Torre 9 Dto. 3 Rawson 9103, Chubut",
    "Av. Río Negro N° 49 Piso 5 Dto. 5 Viedma 8500, Río Negro",
    "Calle 4 N° 392 Piso 9 Dto. 5 Corrientes 3400, Corrientes",
    "Av. 7 N° 49 Bahía Blanca 8000, Buenos Aires",
    "Av. Salta N° 647 Río Gallegos 9400, Santa Cruz",
    "Calle Alvear N° 698 Corrientes 3400, Corrientes",
    "Av. San Salvador de Jujuy N° 98 Santa Fe 3000, Santa Fe",
    "Boulevard Córdoba N° 9010 Comodoro Rivadavia 9000, Chubut",
    "Av. 7 N° 83 Mar del Plata 7600, Buenos Aires",
    "Camino Omar Nuñez N° 2121 Comodoro Rivadavia 9000, Chubut",
    "Calle 124 A N° 16 Local 2 Río Gallegos 9400, Santa Cruz",
    "Av. San Luis N° 8267 Santa Fe 3000, Santa Fe",
    "Avenida Buenos Aires N° 310 Local 17 Santiago del Estero 4200, Santiago del Estero",
    "Av. 8 N° 3042 Formosa 3600, Formosa",
    "Calle Corrientes N° 684 Piso 1 Dto. 3 Resistencia 3500, Chaco",
    "Diagonal Neuquén N° 1027 San Juan 5400, San Juan",
    "Camino Rawson N° 2899 Piso 6 Dto. 5 San Luis 5700, San Luis",
    "Calle 4 N° 302 Paraná 3100, Entre Ríos",
    "Diag. Chaco N° 125 Dto. 4 Santa Fe 3000, Santa Fe",
    "Diagonal Santa Fe N° 618 Salta 4400, Salta",
    "Avenida Tierra del Fuego N° 4949 Piso 4 Dto. 5 Bahía Blanca 8000, Buenos Aires",
    "Diag. 2 N° 129 Río Gallegos 9400, Santa Cruz",
    "Diagonal Rawson N° 921 Local 58 Resistencia 3500, Chaco",
    "Av. Catamarca N° 5327 Dto. 7 Río Gallegos 9400, Santa Cruz",
    "Boulevard Pte. Perón N° 483 Local 95 Santiago del Estero 4200, Santiago del Estero",
    "Avenida Salta N° 12 Santa Rosa 6300, La Pampa",
    "Boulevard 5 N° 95 Merlo 5881, San Luis",
    "Diagonal San Juan N° 3655 San Juan 5400, San Juan",
    "Avenida Posadas N° 528 La Rioja 5300, La Rioja",
    "Avenida Santa Cruz N° 15 Formosa 3600, Formosa",
    "Avenida 2 N° 185 San Salvador de Jujuy 4600, Jujuy",
    "Diag. 4 N° 221 Posadas 3300, Misiones",
    "Avenida La Pampa N° 824 Dto. 3 Santa Rosa 6300, La Pampa",
    "Avenida La Pampa N° 372 Santiago del Estero 4200, Santiago del Estero",
    "Blv. Alem N° 397 Mar del Plata 7600, Buenos Aires",
    "Av. 7 N° 945 Oficina 9 Bahía Blanca 8000, Buenos Aires",
    "Blv. Alvear N° 264 Oficina 9 Posadas 3300, Misiones",
    "Avenida Chubut N° 91 Torre 5 Dto. 3 Rosario 2000, Santa Fe",
    "Diag. Neuquén N° 26 Torre 6 Dto. 4 Viedma 8500, Río Negro",
    "Av. La Plata N° 4772 Paraná 3100, Entre Ríos",
    "Diagonal 143 Bis N° 1067 San Ferando del Valle de Catamarca 4700, Catamarca",
    "Diagonal 7 N° 81 San Ferando del Valle de Catamarca 4700, Catamarca",
    "Boulevard 1 N° 901 Piso 1 Dto. 3 Comodoro Rivadavia 9000, Chubut",
    "Boulevard Alvear N° 25 Mendoza 5500, Mendoza",
    "Calle Saavedra N° 655 Resistencia 3500, Chaco",
    "Av. 4 N° 64 Local 5 Posadas 3300, Misiones",
    "Calle La Pampa N° 87 Torre 2 Dto. 3 San Miguel de Tucumán 4000, Tucumán",
    "Calle Misiones N° 943 La Plata 1900, Buenos Aires",
    "Diagonal Jujuy N° 666 Dto. 4 Mendoza 5500, Mendoza",
    "Boulevard Tierra del Fuego N° 27 Paraná 3100, Entre Ríos",
    "Avenida Río Gallegos N° 8856 Local 13 San Salvador de Jujuy 4600, Jujuy",
    "Camino Misiones N° 4814 Ushuaia 9410, Tierra del Fuego",
    "Calle Alvear N° 204 Torre 6 Dto. 5 Rosario 2000, Santa Fe",
    "Av. 1 N° 77 Merlo 5881, San Luis",
    "Av. San Salvador de Jujuy N° 227 Santiago del Estero 4200, Santiago del Estero",
    "Calle Constitución N° 6680 San Salvador de Jujuy 4600, Jujuy",
    "Calle San Luis N° 15 Torre 1 Dto. 4 Neuquén 8300, Neuquén",
    "Diag. 115 B N° 833 Oficina 95 Comodoro Rivadavia 9000, Chubut",
    "Av. Pte. Perón N° 870 San Juan 5400, San Juan",
    "Diagonal 5 N° 63 Local 83 San Salvador de Jujuy 4600, Jujuy",
    "Diagonal Santiago del Estero N° 646 La Plata 1900, Buenos Aires",
    "Calle San Martin N° 49 Piso 3 Dto. 2 La Rioja 5300, La Rioja",
    "Calle 1 N° 889 Piso 3 Dto. 6 San Miguel de Tucumán 4000, Tucumán",
    "Av. Catamarca N° 681 Posadas 3300, Misiones",
    "Calle Entre Ríos N° 53 Oficina 5 Santa Fe 3000, Santa Fe",
    "Calle Güemes N° 975 Piso 6 Dto. 1 Santa Rosa 6300, La Pampa",
    "Avenida Posadas N° 688 Santiago del Estero 4200, Santiago del Estero",
    "Calle 1 N° 466 Río Gallegos 9400, Santa Cruz",
    "Calle 1 N° 815 Oficina 82 San Luis 5700, San Luis",
    "Avenida J.B. Alberdi N° 37 Corrientes 3400, Corrientes",
    "Av. San Martin N° 426 La Plata 1900, Buenos Aires",
    "Avenida G. Brown N° 765 Piso 2 Dto. 8 Merlo 5881, San Luis",
    "Diagonal Córdoba N° 386 Piso 6 Dto. 1 Santa Fe 3000, Santa Fe",
    "Calle 5 N° 4498 Piso 3 Dto. 9 La Plata 1900, Buenos Aires",
    "Calle 3 N° 22 La Plata 1900, Buenos Aires",
    "Avenida San Juan N° 54 Local 4 La Rioja 5300, La Rioja",
    "Av. Saavedra N° 324 San Luis 5700, San Luis",
    "Boulevard J.J. Castelli N° 672 Salta 4400, Salta",
    "Av. 8 N° 389 Oficina 2 San Ferando del Valle de Catamarca 4700, Catamarca",
    "Av. 3 N° 52 Posadas 3300, Misiones",
    "Blv. G. Brown N° 3712 Chilecito 5360, La Rioja",
    "Diagonal Bahía Blanca N° 214 Posadas 3300, Misiones",
    "Avenida 6 N° 5410 Torre 2 Dto. 9 Corrientes 3400, Corrientes",
    "Av. Mar del Plata N° 383 Dto. 5 Neuquén 8300, Neuquén",
    "Diagonal Omar Nuñez N° 5113 Oficina 3 Santa Fe 3000, Santa Fe",
    "Av. 6 N° 366 Local 1 La Plata 1900, Buenos Aires",
    "Boulevard San Luis N° 688 Salta 4400, Salta",
    "Av. Mar del Plata N° 2090 La Rioja 5300, La Rioja",
    "Av. Saavedra N° 5748 San Ferando del Valle de Catamarca 4700, Catamarca",
    "Avenida 7 N° 2053 Dto. 3 Bahía Blanca 8000, Buenos Aires",
    "Diagonal San Ferando del Valle de Catamarca N° 464 Oficina 5 Córdoba 5000, Córdoba",
    "Calle Saavedra N° 69 Resistencia 3500, Chaco",
    "Av. Omar Nuñez N° 238 San Miguel de Tucumán 4000, Tucumán",
    "Calle Santa Cruz N° 7482 Merlo 5881, San Luis",
    "Avenida G. Brown N° 179 Córdoba 5000, Córdoba",
    "Avenida Rosario N° 25 Piso 3 Dto. 2 Santiago del Estero 4200, Santiago del Estero",
    "Avenida 177 Bis N° 930 Torre 3 Dto. 8 San Salvador de Jujuy 4600, Jujuy",
    "Av. Santa Fe N° 93 Comodoro Rivadavia 9000, Chubut",
    "Av. Malvinas Argentinas N° 408 Piso 7 Dto. 9 Paraná 3100, Entre Ríos",
    "Av. 6 N° 341 Piso 9 Dto. 9 San Miguel de Tucumán 4000, Tucumán",
    "Avenida 5 N° 34 Mar del Plata 7600, Buenos Aires",
    "Av. Omar Nuñez N° 267 Dto. 2 La Plata 1900, Buenos Aires",
    "Diagonal Malvinas Argentinas N° 238 La Plata 1900, Buenos Aires",
    "Diagonal J.B. Alberdi N° 168 Torre 3 Dto. 7 Constitución 1004, Ciudad Autónoma de Buenos Aires",
    "Calle San Martin N° 571 Torre 2 Dto. 1 Resistencia 3500, Chaco",
    "Diagonal San Juan N° 861 Dto. 8 Paraná 3100, Entre Ríos",
    "Av. 4 N° 1393 Oficina 9 La Plata 1900, Buenos Aires",
    "Camino Viedma N° 637 La Plata 1900, Buenos Aires",
    "Av. Salta N° 410 Torre 6 Dto. 4 Rawson 9103, Chubut",
    "Diag. Güemes N° 132 Paraná 3100, Entre Ríos",
    "Avenida 3 N° 311 Córdoba 5000, Córdoba",
    "Av. Santa Rosa N° 2621 Local 43 Río Gallegos 9400, Santa Cruz",
    "Diagonal 7 N° 368 Formosa 3600, Formosa",
    "Av. Mendoza N° 703 Paraná 3100, Entre Ríos",
    "Avenida Posadas N° 488 San Salvador de Jujuy 4600, Jujuy",
    "Av. Jujuy N° 4627 Santa Rosa 6300, La Pampa",
    "Diagonal 9 N° 1352 Chilecito 5360, La Rioja",
    "Blv. Tierra del Fuego N° 72 Oficina 1 Resistencia 3500, Chaco",
    "Diagonal 167 Bis N° 988 San Ferando del Valle de Catamarca 4700, Catamarca",
    "Avenida J.B. Alberdi N° 23 Piso 7 Dto. 5 Santiago del Estero 4200, Santiago del Estero",
    "Diagonal 7 N° 114 Dto. 2 Chilecito 5360, La Rioja",
    "Diagonal Pte. Perón N° 71 Torre 8 Dto. 5 Mar del Plata 7600, Buenos Aires",
    "Avenida Chaco N° 8114 Córdoba 5000, Córdoba",
    "Avenida Córdoba N° 422 Local 7 Córdoba 5000, Córdoba",
    "Diagonal San Salvador de Jujuy N° 86 Córdoba 5000, Córdoba",
    "Blv. 6 N° 589 Constitución 1004, Ciudad Autónoma de Buenos Aires",
    "Calle Catamarca N° 382 Neuquén 8300, Neuquén",
    "Av. Chilecito N° 541 Ushuaia 9410, Tierra del Fuego",
    "Camino Catamarca N° 691 San Luis 5700, San Luis",
    "Calle 5 N° 7887 Mendoza 5500, Mendoza",
    "Diagonal Río Negro N° 3142 Local 68 Santa Rosa 6300, La Pampa",
    "Calle Salta N° 473 Oficina 2 Rawson 9103, Chubut",
    "Avenida J.B. Alberdi N° 598 Córdoba 5000, Córdoba",
    "Av. 2 N° 167 Ushuaia 9410, Tierra del Fuego",
    "Blv. Mendoza N° 12 Córdoba 5000, Córdoba",
    "Diagonal Chaco N° 227 Constitución 1004, Ciudad Autónoma de Buenos Aires",
    "Diagonal Córdoba N° 47 Piso 3 Dto. 5 Constitución 1004, Ciudad Autónoma de Buenos Aires",
    "Calle Tierra del Fuego N° 568 Piso 2 Dto. 7 Salta 4400, Salta",
    "Diag. Córdoba N° 89 Resistencia 3500, Chaco",
    "Boulevard 8 N° 5947 Rosario 2000, Santa Fe",
    "Diagonal J.J. Castelli N° 1784 Dto. 4 Rawson 9103, Chubut",
    "Camino 8 N° 729 Torre 5 Dto. 9 Rosario 2000, Santa Fe",
    "Diagonal J.M. de Rosas N° 762 Merlo 5881, San Luis",
    "Avenida J.B. Alberdi N° 8974 San Ferando del Valle de Catamarca 4700, Catamarca",
    "Avenida J.B. Alberdi N° 401 Oficina 28 Santiago del Estero 4200, Santiago del Estero",
    "Av. 9 N° 6198 La Plata 1900, Buenos Aires",
    "Avenida Malvinas Argentinas N° 874 Formosa 3600, Formosa",
    "Camino 9 N° 11 Constitución 1004, Ciudad Autónoma de Buenos Aires",
    "Camino 9 N° 4438 Corrientes 3400, Corrientes",
    "Boulevard Mitre N° 37 Piso 9 Dto. 4 Constitución 1004, Ciudad Autónoma de Buenos Aires",
    "Calle 6 N° 8669 Rosario 2000, Santa Fe",
    "Calle Omar Nuñez N° 294 Torre 9 Dto. 5 Posadas 3300, Misiones",
    "Avenida Formosa N° 66 Neuquén 8300, Neuquén",
    "Avenida Córdoba N° 249 San Ferando del Valle de Catamarca 4700, Catamarca",
    "Calle San Miguel de Tucumán N° 115 Mendoza 5500, Mendoza",
    "Diagonal Misiones N° 66 Constitución 1004, Ciudad Autónoma de Buenos Aires",
    "Diag. La Plata N° 34 San Ferando del Valle de Catamarca 4700, Catamarca",
    "Av. Córdoba N° 698 Oficina 9 Rosario 2000, Santa Fe",
    "Av. Viedma N° 14 Merlo 5881, San Luis",
    "Av. 6 N° 7287 Torre 7 Dto. 6 Merlo 5881, San Luis",
    "Avenida La Pampa N° 55 Chilecito 5360, La Rioja",
    "Avenida 5 N° 465 Oficina 4 San Ferando del Valle de Catamarca 4700, Catamarca",
    "Diagonal La Plata N° 3622 Formosa 3600, Formosa",
    "Boulevard Alvear N° 691 Local 29 Comodoro Rivadavia 9000, Chubut",
    "Avenida 7 N° 7472 Dto. 8 Rawson 9103, Chubut",
    "Calle Paraná N° 276 Piso 7 Dto. 9 Paraná 3100, Entre Ríos",
    "Diagonal Salta N° 79 Piso 3 Dto. 6 Rosario 2000, Santa Fe",
    "Avenida 3 N° 471 Mendoza 5500, Mendoza",
    "Blv. Saavedra N° 3033 Dto. 8 Rosario 2000, Santa Fe",
    "Calle Chubut N° 909 San Salvador de Jujuy 4600, Jujuy",
    "Av. 3 N° 573 Dto. 9 Merlo 5881, San Luis",
    "Diagonal Mitre N° 975 Piso 4 Dto. 8 San Luis 5700, San Luis",
    "Avenida Salta N° 7896 Torre 1 Dto. 4 La Plata 1900, Buenos Aires",
    "Calle 6 N° 8890 Río Gallegos 9400, Santa Cruz",
    "Calle Corrientes N° 592 Oficina 15 Merlo 5881, San Luis",
    "Boulevard 4 N° 86 Bahía Blanca 8000, Buenos Aires",
    "Av. Formosa N° 127 Torre 4 Dto. 3 Bahía Blanca 8000, Buenos Aires",
    "Diag. Corrientes N° 925 Local 9 San Miguel de Tucumán 4000, Tucumán",
    "Av. 186 B N° 8994 Oficina 6 Merlo 5881, San Luis",
    "Calle Misiones N° 162 Oficina 30 Córdoba 5000, Córdoba",
    "Av. Buenos Aires N° 835 Neuquén 8300, Neuquén",
    "Diag. Santa Fe N° 828 Local 68 Bahía Blanca 8000, Buenos Aires",
    "Diagonal 6 N° 55 Santa Fe 3000, Santa Fe",
    "Av. Córdoba N° 5956 Oficina 5 Santa Fe 3000, Santa Fe",
    "Diag. Tierra del Fuego N° 606 Rawson 9103, Chubut",
    "Av. Belgrano N° 8115 Rawson 9103, Chubut",
    "Avenida Corrientes N° 882 Local 5 Merlo 5881, San Luis",
    "Diagonal Chubut N° 235 Dto. 4 Río Gallegos 9400, Santa Cruz",
    "Boulevard Alem N° 97 Viedma 8500, Río Negro",
    "Calle Mendoza N° 381 Local 20 Formosa 3600, Formosa",
    "Diagonal Saavedra N° 3111 Piso 4 Dto. 7 Chilecito 5360, La Rioja",
    "Diagonal Buenos Aires N° 409 Local 1 San Ferando del Valle de Catamarca 4700, Catamarca",
    "Avenida Mitre N° 566 Mendoza 5500, Mendoza",
    "Av. La Pampa N° 32 Oficina 6 Rosario 2000, Santa Fe",
    "Camino 3 N° 516 Oficina 3 Mendoza 5500, Mendoza",
    "Avenida Mitre N° 139 Torre 6 Dto. 4 Paraná 3100, Entre Ríos",
    "Avenida 7 N° 76 Neuquén 8300, Neuquén",
    "Calle Neuquén N° 34 Rosario 2000, Santa Fe",
    "Diagonal Alem N° 895 Oficina 8 San Juan 5400, San Juan",
    "Boulevard Buenos Aires N° 369 Neuquén 8300, Neuquén",
    "Av. Catamarca N° 533 Dto. 1 Corrientes 3400, Corrientes",
    "Diagonal San Salvador de Jujuy N° 633 Neuquén 8300, Neuquén",
    "Diag. Resistencia N° 55 Mendoza 5500, Mendoza",
    "Calle Alvear N° 954 Dto. 6 Comodoro Rivadavia 9000, Chubut",
    "Avenida 7 N° 859 Torre 4 Dto. 5 San Luis 5700, San Luis",
    "Calle 7 N° 612 Bahía Blanca 8000, Buenos Aires",
    "Diagonal Santa Cruz N° 537 Local 15 San Miguel de Tucumán 4000, Tucumán",
    "Boulevard Neuquén N° 738 Oficina 5 San Salvador de Jujuy 4600, Jujuy",
    "Calle La Rioja N° 8664 Dto. 5 Viedma 8500, Río Negro",
    "Blv. Formosa N° 3319 Salta 4400, Salta",
    "Av. San Luis N° 768 Torre 7 Dto. 5 San Ferando del Valle de Catamarca 4700, Catamarca",
    "Boulevard Mendoza N° 530 Santiago del Estero 4200, Santiago del Estero",
    "Avenida San Luis N° 629 Torre 4 Dto. 3 Constitución 1004, Ciudad Autónoma de Buenos Aires",
    "Av. Misiones N° 162 San Luis 5700, San Luis",
    "Diag. Omar Nuñez N° 978 Local 8 Santa Fe 3000, Santa Fe",
    "Calle 4 N° 8618 Oficina 7 Santiago del Estero 4200, Santiago del Estero",
    "Calle Mar del Plata N° 774 Río Gallegos 9400, Santa Cruz",
    "Av. Comodoro Rivadavia N° 799 San Salvador de Jujuy 4600, Jujuy",
    "Calle San Martin N° 469 Oficina 6 Constitución 1004, Ciudad Autónoma de Buenos Aires",
    "Calle Paraná N° 2245 San Luis 5700, San Luis",
    "Boulevard Santa Fe N° 604 Corrientes 3400, Corrientes",
    "Diagonal 4 N° 15 San Luis 5700, San Luis",
    "Av. 7 N° 784 Rosario 2000, Santa Fe",
    "Avenida Omar Nuñez N° 49 Oficina 4 Santa Rosa 6300, La Pampa",
    "Diagonal 7 N° 360 San Juan 5400, San Juan",
    "Calle La Rioja N° 976 Neuquén 8300, Neuquén",
    "Blv. San Martin N° 34 Piso 7 Dto. 3 San Miguel de Tucumán 4000, Tucumán",
    "Diagonal Malvinas Argentinas N° 74 Comodoro Rivadavia 9000, Chubut",
    "Boulevard Santa Cruz N° 969 Dto. 8 Rosario 2000, Santa Fe",
    "Calle La Plata N° 98 Oficina 93 Neuquén 8300, Neuquén",
    "Avenida J.J. Castelli N° 65 Oficina 16 Córdoba 5000, Córdoba",
    "Calle Constitución N° 5862 San Luis 5700, San Luis",
    "Avenida 7 N° 3828 Bahía Blanca 8000, Buenos Aires",
    "Diagonal 9 N° 2996 Oficina 26 Córdoba 5000, Córdoba",
    "Calle Neuquén N° 6442 Oficina 3 Resistencia 3500, Chaco",
    "Avenida Mar del Plata N° 356 Piso 8 Dto. 8 Córdoba 5000, Córdoba",
    "Avenida Chubut N° 763 San Ferando del Valle de Catamarca 4700, Catamarca",
    "Calle Rawson N° 4994 La Plata 1900, Buenos Aires",
    "Av. Córdoba N° 950 Río Gallegos 9400, Santa Cruz",
    "Calle Río Negro N° 74 Piso 5 Dto. 6 Santa Rosa 6300, La Pampa",
    "Calle Paraná N° 3079 Ushuaia 9410, Tierra del Fuego",
    "Av. 1 N° 154 Rosario 2000, Santa Fe",
    "Blv. 7 N° 28 Local 2 Corrientes 3400, Corrientes",
    "Blv. Salta N° 612 Local 8 Mendoza 5500, Mendoza",
    "Calle 4 N° 6014 Paraná 3100, Entre Ríos",
    "Diagonal Belgrano N° 95 Santa Rosa 6300, La Pampa",
    "Calle Mendoza N° 888 Corrientes 3400, Corrientes",
    "Av. 9 N° 778 Rosario 2000, Santa Fe",
    "Diagonal Belgrano N° 551 Chilecito 5360, La Rioja",
    "Calle Santiago del Estero N° 147 Local 2 Formosa 3600, Formosa",
    "Diagonal Salta N° 6781 Rosario 2000, Santa Fe",
    "Av. 9 N° 447 San Salvador de Jujuy 4600, Jujuy",
    "Av. Tucumán N° 73 Paraná 3100, Entre Ríos",
    "Calle Mar del Plata N° 954 Santa Rosa 6300, La Pampa",
    "Diagonal 7 N° 4799 San Luis 5700, San Luis",
    "Av. San Juan N° 427 Santa Rosa 6300, La Pampa",
    "Diagonal Corrientes N° 4350 Dto. 3 San Luis 5700, San Luis",
    "Diagonal Jujuy N° 506 Rosario 2000, Santa Fe",
    "Boulevard 9 N° 483 Dto. 6 Chilecito 5360, La Rioja",
    "Camino Ushuaia N° 458 Local 27 La Rioja 5300, La Rioja",
    "Av. Corrientes N° 2920 San Salvador de Jujuy 4600, Jujuy",
    "Av. 4 N° 33 Santa Fe 3000, Santa Fe",
    "Avenida Ciudad Autónoma de Buenos Aires N° 64 Santa Rosa 6300, La Pampa",
    "Av. Jujuy N° 116 Corrientes 3400, Corrientes",
    "Diagonal Tierra del Fuego N° 3491 Oficina 96 Río Gallegos 9400, Santa Cruz",
    "Calle Saavedra N° 238 Bahía Blanca 8000, Buenos Aires",
    "Diagonal 6 N° 33 Local 30 Santa Fe 3000, Santa Fe",
    "Camino 1 N° 16 Mar del Plata 7600, Buenos Aires",
    "Diagonal Salta N° 3941 Chilecito 5360, La Rioja",
    "Diagonal Corrientes N° 33 Resistencia 3500, Chaco",
    "Avenida J.B. Alberdi N° 4353 Resistencia 3500, Chaco",
    "Calle 7 N° 388 Local 65 Rawson 9103, Chubut",
    "Diagonal Chubut N° 16 Rosario 2000, Santa Fe",
    "Av. La Pampa N° 471 San Juan 5400, San Juan",
    "Av. Corrientes N° 47 Rawson 9103, Chubut",
    "Blv. 6 N° 814 Local 7 Córdoba 5000, Córdoba",
    "Av. Güemes N° 421 Corrientes 3400, Corrientes",
    "Diagonal 7 N° 968 Mendoza 5500, Mendoza",
    "Diag. Santa Fe N° 62 Mar del Plata 7600, Buenos Aires",
    "Calle Chilecito N° 9171 Oficina 11 Rawson 9103, Chubut",
    "Av. Saavedra N° 598 Neuquén 8300, Neuquén",
    "Calle J.J. Castelli N° 5220 Torre 8 Dto. 3 Chilecito 5360, La Rioja",
    "Avenida Entre Ríos N° 2187 La Plata 1900, Buenos Aires",
    "Diagonal Güemes N° 190 Dto. 5 Merlo 5881, San Luis",
    "Diag. La Plata N° 3947 Torre 2 Dto. 9 Río Gallegos 9400, Santa Cruz",
    "Camino Viedma N° 628 Posadas 3300, Misiones",
    "Calle 3 N° 574 Santa Rosa 6300, La Pampa",
    "Avenida San Luis N° 741 Santa Fe 3000, Santa Fe",
    "Av. 9 N° 77 Mendoza 5500, Mendoza",
    "Diagonal Ciudad Autónoma de Buenos Aires N° 454 Bahía Blanca 8000, Buenos Aires",
    "Av. Santiago del Estero N° 32 Local 4 Salta 4400, Salta",
    "Diagonal Jujuy N° 9137 Mar del Plata 7600, Buenos Aires",
    "Diagonal San Luis N° 9838 San Ferando del Valle de Catamarca 4700, Catamarca",
    "Avenida 5 N° 883 Río Gallegos 9400, Santa Cruz",
    "Diag. Mitre N° 4473 Córdoba 5000, Córdoba",
    "Camino Saavedra N° 53 Local 8 Neuquén 8300, Neuquén",
    "Calle J.B. Alberdi N° 17 Local 25 Constitución 1004, Ciudad Autónoma de Buenos Aires",
    "Av. Alvear N° 65 Constitución 1004, Ciudad Autónoma de Buenos Aires",
    "Av. Güemes N° 5670 Dto. 1 Ushuaia 9410, Tierra del Fuego",
    "Blv. 2 N° 9584 Posadas 3300, Misiones",
    "Calle Rosario N° 65 Córdoba 5000, Córdoba",
    "Diagonal La Pampa N° 18 Santa Fe 3000, Santa Fe",
    "Diag. Mitre N° 2428 Oficina 4 Corrientes 3400, Corrientes",
    "Av. 2 N° 975 Local 7 Chilecito 5360, La Rioja",
    "Avenida 7 N° 1960 Bahía Blanca 8000, Buenos Aires",
    "Calle San Juan N° 4229 Oficina 11 Corrientes 3400, Corrientes",
    "Avenida Belgrano N° 429 Dto. 4 Constitución 1004, Ciudad Autónoma de Buenos Aires",
    "Diagonal Mitre N° 965 Oficina 64 Resistencia 3500, Chaco",
    "Diagonal J.M. de Rosas N° 8438 Piso 1 Dto. 6 San Juan 5400, San Juan",
    "Calle 5 N° 6335 Piso 2 Dto. 2 Corrientes 3400, Corrientes",
    "Avenida 7 N° 196 Piso 3 Dto. 8 Chilecito 5360, La Rioja",
    "Diagonal Corrientes N° 6060 Local 50 San Miguel de Tucumán 4000, Tucumán",
    "Calle 4 N° 380 Córdoba 5000, Córdoba",
    "Calle Alvear N° 675 San Luis 5700, San Luis",
    "Calle Merlo N° 4588 Mar del Plata 7600, Buenos Aires",
    "Av. Misiones N° 9589 Corrientes 3400, Corrientes",
    "Boulevard Corrientes N° 63 Local 46 Bahía Blanca 8000, Buenos Aires",
    "Diag. Misiones N° 676 Santa Fe 3000, Santa Fe",
    "Diagonal Catamarca N° 61 Posadas 3300, Misiones",
    "Avenida Rosario N° 64 Local 7 San Ferando del Valle de Catamarca 4700, Catamarca",
    "Boulevard Córdoba N° 562 Paraná 3100, Entre Ríos",
    "Diagonal Saavedra N° 131 Rosario 2000, Santa Fe",
    "Diagonal Ciudad Autónoma de Buenos Aires N° 73 Torre 9 Dto. 8 Paraná 3100, Entre Ríos",
    "Avenida Mendoza N° 1188 Santiago del Estero 4200, Santiago del Estero",
    "Blv. Santa Rosa N° 5254 Dto. 9 Santa Rosa 6300, La Pampa",
    "Av. Neuquén N° 979 Local 2 Rawson 9103, Chubut",
    "Calle La Pampa N° 44 Local 20 Neuquén 8300, Neuquén",
    "Blv. J.B. Alberdi N° 33 Santa Fe 3000, Santa Fe",
    "Av. San Luis N° 6692 Merlo 5881, San Luis",
    "Avenida 5 N° 9777 Torre 1 Dto. 9 Córdoba 5000, Córdoba",
    "Av. J.J. Castelli N° 512 Local 2 Comodoro Rivadavia 9000, Chubut",
    "Calle Alvear N° 2684 Oficina 71 Salta 4400, Salta",
    "Av. Rawson N° 5366 Local 44 San Luis 5700, San Luis",
    "Avenida La Rioja N° 479 Torre 7 Dto. 3 San Juan 5400, San Juan",
    "Diag. 2 N° 54 Local 94 Formosa 3600, Formosa",
    "Camino Río Gallegos N° 3677 Local 9 Santa Rosa 6300, La Pampa",
    "Avenida Corrientes N° 985 La Rioja 5300, La Rioja",
    "Av. 3 N° 9157 Oficina 69 San Juan 5400, San Juan",
    "Diagonal 7 N° 837 Resistencia 3500, Chaco",
    "Avenida San Luis N° 367 Chilecito 5360, La Rioja",
    "Av. Pte. Perón N° 6888 Torre 3 Dto. 1 Río Gallegos 9400, Santa Cruz",
    "Avenida 5 N° 312 Salta 4400, Salta",
    "Calle 3 N° 64 Rawson 9103, Chubut",
    "Av. San Juan N° 41 Neuquén 8300, Neuquén",
    "Avenida Río Gallegos N° 395 Santa Fe 3000, Santa Fe",
    "Calle 9 N° 45 Formosa 3600, Formosa",
    "Camino 172 Bis N° 593 Bahía Blanca 8000, Buenos Aires",
    "Diag. Alem N° 537 Mar del Plata 7600, Buenos Aires",
    "Calle Buenos Aires N° 531 Torre 6 Dto. 9 Posadas 3300, Misiones",
    "Calle Ushuaia N° 5450 Dto. 1 San Ferando del Valle de Catamarca 4700, Catamarca",
    "Diagonal 7 N° 486 Dto. 9 San Ferando del Valle de Catamarca 4700, Catamarca",
    "Boulevard Santiago del Estero N° 344 Constitución 1004, Ciudad Autónoma de Buenos Aires",
    "Diagonal San Juan N° 17 Torre 3 Dto. 2 Bahía Blanca 8000, Buenos Aires",
    "Calle Belgrano N° 15 Piso 9 Dto. 7 Río Gallegos 9400, Santa Cruz",
    "Calle 9 N° 22 Oficina 2 Paraná 3100, Entre Ríos",
    "Diagonal 3 N° 72 Torre 5 Dto. 7 Corrientes 3400, Corrientes",
    "Avenida 1 N° 962 Córdoba 5000, Córdoba",
    "Av. 2 N° 7078 San Juan 5400, San Juan",
    "Camino 8 N° 3486 San Luis 5700, San Luis",
    "Avenida Misiones N° 740 Dto. 6 Mar del Plata 7600, Buenos Aires",
    "Blv. 9 N° 6963 Santa Fe 3000, Santa Fe",
    "Av. 138 A N° 44 San Salvador de Jujuy 4600, Jujuy",
    "Blv. Santa Cruz N° 41 San Ferando del Valle de Catamarca 4700, Catamarca",
    "Av. Resistencia N° 82 Torre 4 Dto. 1 Córdoba 5000, Córdoba",
    "Calle Alem N° 538 Merlo 5881, San Luis",
    "Avenida Tucumán N° 357 Ushuaia 9410, Tierra del Fuego",
    "Calle 9 N° 381 Mendoza 5500, Mendoza",
    "Avenida 8 N° 789 La Rioja 5300, La Rioja",
    "Calle La Rioja N° 53 Santiago del Estero 4200, Santiago del Estero",
    "Diagonal Río Gallegos N° 5689 Torre 8 Dto. 5 Viedma 8500, Río Negro",
    "Diagonal 9 N° 543 Mendoza 5500, Mendoza",
    "Camino Alvear N° 897 Piso 1 Dto. 2 Ushuaia 9410, Tierra del Fuego",
    "Calle Posadas N° 520 Local 44 Río Gallegos 9400, Santa Cruz",
    "Diag. 8 N° 923 Santa Rosa 6300, La Pampa",
    "Boulevard San Luis N° 253 Torre 9 Dto. 7 Paraná 3100, Entre Ríos",
    "Calle Alvear N° 29 Córdoba 5000, Córdoba",
    "Av. 7 N° 61 Local 3 Santa Rosa 6300, La Pampa",
    "Av. Rosario N° 2267 San Luis 5700, San Luis",
    "Calle Mendoza N° 5139 Santa Fe 3000, Santa Fe",
    "Calle 117 B N° 67 Neuquén 8300, Neuquén",
    "Boulevard La Plata N° 5520 Oficina 8 Comodoro Rivadavia 9000, Chubut",
    "Av. 129 Bis N° 99 Merlo 5881, San Luis",
    "Av. Corrientes N° 6377 Oficina 2 San Miguel de Tucumán 4000, Tucumán",
    "Av. 2 N° 53 Resistencia 3500, Chaco",
    "Calle J.B. Alberdi N° 4334 Mendoza 5500, Mendoza",
    "Diagonal 179 B N° 199 Dto. 5 Santa Fe 3000, Santa Fe",
    "Camino Comodoro Rivadavia N° 203 Mar del Plata 7600, Buenos Aires",
    "Calle Rawson N° 3447 Ushuaia 9410, Tierra del Fuego",
    "Diagonal J.B. Alberdi N° 9791 Torre 8 Dto. 5 Formosa 3600, Formosa",
    "Calle 7 N° 87 Dto. 4 Rosario 2000, Santa Fe",
    "Av. 169 A N° 565 Local 69 Santa Rosa 6300, La Pampa",
    "Boulevard 5 N° 972 Local 61 Ushuaia 9410, Tierra del Fuego",
    "Calle 4 N° 1161 Santa Rosa 6300, La Pampa",
    "Av. San Luis N° 814 Ushuaia 9410, Tierra del Fuego",
    "Blv. Omar Nuñez N° 37 Piso 3 Dto. 6 Río Gallegos 9400, Santa Cruz",
    "Av. 1 N° 682 San Miguel de Tucumán 4000, Tucumán",
    "Diagonal Misiones N° 23 Dto. 5 Bahía Blanca 8000, Buenos Aires",
    "Avenida Pte. Perón N° 49 Corrientes 3400, Corrientes",
    "Avenida J.M. de Rosas N° 2532 La Rioja 5300, La Rioja",
    "Avenida San Luis N° 250 San Ferando del Valle de Catamarca 4700, Catamarca",
    "Av. Santiago del Estero N° 266 La Rioja 5300, La Rioja",
    "Av. Entre Ríos N° 760 Local 1 Viedma 8500, Río Negro",
    "Calle Tierra del Fuego N° 225 Rosario 2000, Santa Fe",
    "Calle Rawson N° 73 San Ferando del Valle de Catamarca 4700, Catamarca",
    "Blv. 7 N° 75 Comodoro Rivadavia 9000, Chubut",
    "Avenida Güemes N° 96 Local 9 Corrientes 3400, Corrientes",
    "Av. San Luis N° 150 Local 4 San Luis 5700, San Luis",
    "Diagonal Chilecito N° 527 San Salvador de Jujuy 4600, Jujuy",
    "Av. 120 Bis N° 884 Corrientes 3400, Corrientes",
    "Calle Pte. Perón N° 923 Santiago del Estero 4200, Santiago del Estero",
    "Diagonal 9 N° 309 Dto. 4 San Ferando del Valle de Catamarca 4700, Catamarca",
    "Av. Rawson N° 7549 Piso 7 Dto. 1 Corrientes 3400, Corrientes",
    "Calle 9 N° 681 La Rioja 5300, La Rioja",
    "Calle Chaco N° 88 Torre 7 Dto. 1 Formosa 3600, Formosa",
    "Diagonal Güemes N° 1598 Dto. 1 Mar del Plata 7600, Buenos Aires",
    "Calle J.J. Castelli N° 561 Torre 1 Dto. 8 Río Gallegos 9400, Santa Cruz",
    "Av. La Pampa N° 763 San Salvador de Jujuy 4600, Jujuy" };

std::string generarDNI(int id) {
    std::stringstream ss;
    ss << std::setw(8) << std::setfill('0') << id;
    return ss.str();
}


std::string generarTelefono()
{
    std::string tel = "9";
    for (int i = 0; i < 8; i++)
    {
        int num = rand() % 10;
        tel += static_cast<char>(num + '0');
    }
    return tel;
}

std::string generarEmail(const std::string& nombre, const std::string& apellido)
{
    std::string email = std::string(1, tolower(static_cast<unsigned char>(nombre[0]))) + "." + toLowerCase(apellido) + extensionEmail_lst[getRandom(extensionEmail_lst)];
    return email;
}

std::pair<std::vector<std::string>, std::string> generarInformacionBasica(const std::string& nacionalidad)
{
    std::string lugarNacimiento;
    std::vector<std::string> apellidos;
    if (nacionalidad == nacionalidades_lst[0])
    {
        Departamento dptPeruano = departamentosPeru_lst[getRandomDepartament(departamentosPeru_lst)];

        apellidos = { apellidosPeruanos[getRandom(apellidosPeruanos)], apellidosPeruanos[getRandom(apellidosPeruanos)] };

        lugarNacimiento = dptPeruano.ciudades[getRandom(dptPeruano.ciudades)] + ", " + dptPeruano.nombre + ", " + paises_lst[0];
    }
    else if (nacionalidad == nacionalidades_lst[1])
    {
        Departamento dptVenezolano = departamentosVenezuela_lst[getRandomDepartament(departamentosVenezuela_lst)];
        apellidos = { apellidosVenezolanos[getRandom(apellidosVenezolanos)], apellidosVenezolanos[getRandom(apellidosVenezolanos)] };
        lugarNacimiento = dptVenezolano.ciudades[getRandom(dptVenezolano.ciudades)] + ", " + dptVenezolano.nombre + ", " + paises_lst[1];
    }
    else
    {
        Departamento dptColombiano = departamentosColombia_lst[getRandomDepartament(departamentosColombia_lst)];
        apellidos = { apellidosColombianos[getRandom(apellidosColombianos)], apellidosColombianos[getRandom(apellidosColombianos)] };
        lugarNacimiento = dptColombiano.ciudades[getRandom(dptColombiano.ciudades)] + ", " + dptColombiano.nombre + ", " + paises_lst[2];
    }

    return std::pair<std::vector<std::string>, std::string>(apellidos, lugarNacimiento);
}

Ciudadano::Ciudadano(int id)
{
    this->id = id;
    std::string nacionalidad = nacionalidades_lst[getRandomNacionality(nacionalidades_lst)];
    std::string nombre = nombres_lst[getRandom(nombres_lst)];
    std::pair<std::vector<std::string>, std::string> info = generarInformacionBasica(nacionalidad);
    this->apellidos = info.first;
    this->lugarNacimiento = info.second;

    this->nacionalidad = nacionalidad;

    this->dni = generarDNI(id);

    this->nombre = nombre;

    this->telefono = generarTelefono();
    this->email = generarEmail(nombre, this->apellidos[0]);
    this->estadoCivil = estadosCiviles_lst[getRandom(estadosCiviles_lst)];
    this->direccion = direcciones_lst[getRandom(direcciones_lst)];
}

void Ciudadano::imprimir()
{
    std::cout << "\t\tDNI: " << this->dni << std::endl;
    std::cout << "\t\tNombres: " << this->nombre << std::endl;
    std::cout << "\t\tApellidos: " << this->apellidos[0] << " " << this->apellidos[1] << std::endl;
    std::cout << "\t\tNacionalidad: " << this->nacionalidad << std::endl;
    std::cout << "\t\tLugar de Nacimiento: " << this->lugarNacimiento << std::endl;
    std::cout << "\t\tDirección: " << this->direccion << std::endl;
    std::cout << "\t\tTeléfono: " << this->telefono << std::endl;
    std::cout << "\t\tEmail: " << this->email << std::endl;
    std::cout << "\t\tEstado Civil: " << this->estadoCivil << std::endl;
    std::cout << std::endl;
}

void Ciudadano::save(std::ostringstream& oss)
{
    boost::archive::binary_oarchive oa(oss);
    oa&* (this);
}

void Ciudadano::load(std::ostringstream& oss)
{
    std::string str_data = oss.str();
    std::istringstream iss(str_data);
    boost::archive::binary_iarchive ia(iss);
    ia&* (this);
}

size_t Ciudadano::getCiudadanoSize() {
    size_t size = 0;
    size = sizeof(this->id);
    size += this->dni.size() + 1;
    size += this->nombre.size() + 1;
    size += this->nacionalidad.size() + 1;
    size += this->lugarNacimiento.size() + 1;
    size += this->direccion.size() + 1;
    size += this->telefono.size() + 1;
    size += this->email.size() + 1;
    size += this->estadoCivil.size() + 1;
    size += sizeof(this->apellidos);
    for (const std::string& apellido : this->apellidos) {
        size += apellido.size() + 1;
    }

    return size;
}

int Ciudadano::getId() {
    return this->id;
}