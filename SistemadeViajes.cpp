#include <iostream>
#include <sqlite3.h>
#include <vector>
#include <algorithm>

struct Sucursal {
    int id;
    std::string nombre;
    double distancia;
};

struct Colaborador {
    int id;
    std::string nombre;
    std::vector<Sucursal> sucursales;
    std::vector<Viaje> viajes;
};


struct Viaje {
    Colaborador colaborador;
    Sucursal sucursal;
    std::string transportista;
    double distanciaAcumulada;
};

bool login(const std::string& username, const std::string& password) {
    sqlite3* db;
    int rc = sqlite3_open("database.db", &db); // Abre la conexión con la base de datos

    if (rc != SQLITE_OK) {
        std::cerr << "Error al abrir la base de datos: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    // Prepara la consulta SQL
    std::string sql = "SELECT * FROM users WHERE username = ? AND password = ?";
    sqlite3_stmt* stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

    if (rc != SQLITE_OK) {
        std::cerr << "Error al preparar la consulta: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return false;
    }

    // Asigna los valores a los parámetros de la consulta
    sqlite3_bind_text(stmt, 1, username.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password.c_str(), -1, SQLITE_STATIC);

    // Ejecuta la consulta
    rc = sqlite3_step(stmt);

    bool success = false;
    if (rc == SQLITE_ROW) {
        // El usuario y la contraseña coinciden
        success = true;
    }

    // Finaliza la consulta y cierra la conexión con la base de datos
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return success;
}


bool verificarPerfil(const std::string& username) {
    // Aquí debo verificar el perfil del usuario en la base de datos
    // Verificar el perfil del usuario
    return true; // Devolver true si es un "Gerente de tienda", de lo contrario, devolver false
}

// Función para verificar si una sucursal ya está asignada a un colaborador
bool verificarSucursalAsignada(const Colaborador& colaborador, int idSucursal) {
    auto it = std::find_if(colaborador.sucursales.begin(), colaborador.sucursales.end(), [idSucursal](const Sucursal& sucursal) {
        return sucursal.id == idSucursal;
    });

    return (it != colaborador.sucursales.end());
}

// Función para asignar una sucursal a un colaborador
void asignarSucursal(Colaborador& colaborador, const Sucursal& sucursal) {
    if (!verificarSucursalAsignada(colaborador, sucursal.id)) {
        colaborador.sucursales.push_back(sucursal);
        std::cout << "Sucursal asignada correctamente." << std::endl;
    } else {
        std::cout << "La sucursal ya está asignada al colaborador." << std::endl;
    }
}

// Función para registrar una nueva sucursal
Sucursal registrarSucursal() {
    Sucursal sucursal;
    std::cout << "Ingrese el ID de la sucursal: ";
    std::cin >> sucursal.id;
    std::cout << "Ingrese el nombre de la sucursal: ";
    std::cin >> sucursal.nombre;
    std::cout << "Ingrese la distancia (en kilómetros) desde la sucursal hasta la casa del colaborador: ";
    std::cin >> sucursal.distancia;

    return sucursal;
}


bool verificarViajeDuplicado(const Colaborador& colaborador) {
    // Verificar si el colaborador ya ha realizado un viaje en el día actual
    return false; // Devolver true si ya ha realizado un viaje, de lo contrario, devolver false
}

int main() {
    std::string username, password;

    // Solicitar las credenciales al usuario
    std::cout << "Ingrese el nombre de usuario: ";
    std::cin >> username;
    std::cout << "Ingrese la contraseña: ";
    std::cin >> password;

    // Autenticar al usuario
    if (login(username, password)) {
        std::cout << "Inicio de sesión exitoso. ¡Bienvenido, " << username << "!" << std::endl;

        std::vector<Colaborador> colaboradores;

        // Menú principal
int opcion;
do {
    std::cout << "\n--- Menú ---" << std::endl;
    std::cout << "1. Asignar sucursal a colaborador" << std::endl;
    std::cout << "2. Registrar viaje" << std::endl; // Opción agregada para registrar un viaje
    std::cout << "3. Salir" << std::endl;
    std::cout << "Ingrese una opción: ";
    std::cin >> opcion;

    switch (opcion) {
        case 1:
            // Lógica para asignar sucursal a colaborador
            break;
        case 2:
            registrarViaje(colaboradores); // Llamar a la función para registrar un viaje
            break;
        case 3:
            std::cout << "¡Hasta luego!" << std::endl;
            break;
        default:
            std::cout << "Opción inválida. Intente nuevamente." << std::endl;
            break;
    }
} while (opcion != 3);


    } else {
        std::cout << "Credenciales inválidas. Inicio de sesión fallido." << std::endl;
        
    }

    return 0;
}
