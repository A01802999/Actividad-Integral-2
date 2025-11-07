/*
Descripción: Programa que lee una bitácora de eventos de red y los almacena en
una lista ligada ordenada (Sorted Linked List) por IP destino. Posteriormente,
permite buscar todos los eventos asociados a una IP específica.

Equipo 4: Diego Sánchez Mancilla A01802999, Alexa Ávila Luna A01803526, Valeria Gissele Portilla Robles A01752371

*/

#include <iostream>
#include <fstream>          
#include <sstream>
#include <ctime>
#include <string>
#include <iomanip>          
#include <filesystem>       //Biblioteca para manejo de rutas de archivos
#include "SortedLinkedList.h"

using std::cout;
using std::cin;
using std::string;
using std::ifstream;
using std::ofstream;
using std::getline;
using std::stringstream;


// Estructura para representar dirección IP
struct Ip {
    int o1;
    int o2;
    int o3;
    int o4;
};


// Estructura para representar un evento de red
struct Event {
    std::tm ts;      // Fecha y hora
    Ip ipOrigin;     // IP origen
    string portOrigin;
    string domainOrigin;
    Ip ipDest;       // IP destino
    string portDest;
    string domainDest;
};


// Sobrecarga de operadores
std::ostream& operator<<(std::ostream &os, const Ip &i) {       // Imprime IP
    if (i.o1 == 0) {
        os << "-";
    } else {
        os << i.o1 << "." << i.o2 << "." << i.o3 << "." << i.o4;
    }
    return os;
}

// Imprime un evento completo en formato CSV
std::ostream& operator<<(std::ostream &os, const Event &e) {
    char date_output[20];
    strftime(date_output, 20, "%d-%m-%Y,%H:%M:%S", &e.ts);
    os << date_output << "," << e.ipOrigin << "," << e.portOrigin << ","
       << e.domainOrigin << "," << e.ipDest << "," << e.portDest << ","
       << e.domainDest;
    return os;
}

// Comparación entre dos IPs
bool operator<(Ip &ip1, Ip &ip2) {
    if (ip1.o1 != ip2.o1) return ip1.o1 < ip2.o1;
    if (ip1.o2 != ip2.o2) return ip1.o2 < ip2.o2;
    if (ip1.o3 != ip2.o3) return ip1.o3 < ip2.o3;
    return ip1.o4 < ip2.o4;
}

// Comparación entre eventos (según IP destino)
bool operator<(Event &e1, Event &e2) {
    return e1.ipDest < e2.ipDest;
}


// Función para convertir texto de fecha a struct tm
std::tm parseDateTime(const string &date, const string &time) {
    std::tm tm = {};
    string full = date + " " + time;
    std::istringstream ss(full);
    ss >> std::get_time(&tm, "%d-%m-%Y %H:%M:%S");
    return tm;
}


// Función auxiliar para convertir texto de IP a struct Ip
Ip parseIp(const string &ipText) {
    Ip ip = {0, 0, 0, 0};
    if (ipText == "-") return ip;
    char dot;
    std::istringstream ss(ipText);
    ss >> ip.o1 >> dot >> ip.o2 >> dot >> ip.o3 >> dot >> ip.o4;
    return ip;
}


// Función principal
int main() {
    cout << "Directorio actual: " << std::filesystem::current_path() << "\n";

    SortedLinkedList<Event> lista;

    // Abrir archivo CSV
    ifstream file("equipo4.csv");
    if (!file.is_open()) {
        cout << "Error: No se pudo abrir el archivo de bitácora.\n";
        return 1;
    }
    cout << "Archivo abierto correctamente.\n";

    // Leer línea por línea
    string line;

    //Hace que lea todo el archivo y lo inserte en la lista ordenada
    while (getline(file, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string date, time, ip_o, port_o, domain_o, ip_d, port_d, domain_d;

        getline(ss, date, ',');
        getline(ss, time, ',');
        getline(ss, ip_o, ',');
        getline(ss, port_o, ',');
        getline(ss, domain_o, ',');
        getline(ss, ip_d, ',');
        getline(ss, port_d, ',');
        getline(ss, domain_d, ',');

        Event e;        // Crea un evento y llena datos
        e.ts = parseDateTime(date, time);
        e.ipOrigin = parseIp(ip_o);
        e.portOrigin = port_o;
        e.domainOrigin = domain_o;
        e.ipDest = parseIp(ip_d);
        e.portDest = port_d;
        e.domainDest = domain_d;

        lista.add(e);
    }
    file.close();
    cout << "Datos cargados y lista ordenada por IP destino.\n";

    // Guardar la lista ordenada en un nuevo archivo
    ofstream out("resultado.csv");
    for (auto it = lista.begin(); it != lista.end(); ++it) {
        out << *it << "\n";
    }
    out.close();
    cout << "Archivo resultado.csv generado correctamente.\n\n";

    // Solicitar IP destino al usuario
    cout << "Introduce la IP destino a buscar (formato x.x.x.x): ";
    string userIp;
    cin >> userIp;
    Ip buscada = parseIp(userIp);

// Crea función lambda para comparar IPs
    auto match = [buscada](Event &e) {      
        return (e.ipDest.o1 == buscada.o1 &&
                e.ipDest.o2 == buscada.o2 &&
                e.ipDest.o3 == buscada.o3 &&
                e.ipDest.o4 == buscada.o4);
    };

    // Busca primer evento con esa IP
    auto it = lista.find(match);
    if (it == lista.end()) {
        cout << "No se encontraron eventos con esa IP destino.\n";
    } else {
        cout << "Eventos con IP destino " << userIp << ":\n";
        for (; it != lista.end(); ++it) {
            if (!match(*it)) break;
            cout << *it << "\n";
        }
    }

    return 0;
}
