#include <iostream>
#include <sqlite3.h> // Asegúrate de tener la biblioteca SQLite instalada

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
        // Aquí puedes continuar con el flujo principal de tu aplicación después del inicio de sesión exitoso
    } else {
        std::cout << "Credenciales inválidas. Inicio de sesión fallido." << std::endl;
        // Aquí puedes manejar el caso de inicio de sesión fallido, como mostrar un mensaje de error o finalizar la aplicación
    }

    return 0;
}
