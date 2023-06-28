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
            std::cout << "2. Salir" << std::endl;
            std::cout << "Ingrese una opción: ";
            std::cin >> opcion;

            switch (opcion) {
                case 1: {
                    int idColaborador;
                    std::cout << "Ingrese el ID del colaborador: ";
                    std::cin >> idColaborador;

                    // Buscar al colaborador en la lista
                    auto it = std::find_if(colaboradores.begin(), colaboradores.end(), [idColaborador](const Colaborador& colaborador) {
                        return colaborador.id == idColaborador;
                    });

                    if (it != colaboradores.end()) {
                        Sucursal nuevaSucursal = registrarSucursal();
                        // Verificar validez de la distancia
                        if (nuevaSucursal.distancia > 0 && nuevaSucursal.distancia <= 50) {
                            asignarSucursal(*it, nuevaSucursal);
                        } else {
                            std::cout << "La distancia debe ser mayor que cero y no mayor que 50." << std::endl;
                        }
                    } else {
                        std::cout << "No se encontró al colaborador con el ID especificado." << std::endl;
                    }

                    break;
                }
                case 2:
                    std::cout << "¡Hasta luego!" << std::endl;
                    break;
                default:
                    std::cout << "Opción inválida. Intente nuevamente." << std::endl;
                    break;
            }
        } while (opcion != 2);

    } else {
        std::cout << "Credenciales inválidas. Inicio de sesión fallido." << std::endl;
        
    }

    return 0;
}
