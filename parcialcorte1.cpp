#include <iostream>
#include <malloc.h> 

using namespace std;

// Definición de la estructura para un cliente
struct Cliente {
    int numeroCuenta;
    int tipoCuenta;
    int turno;  // 1 para preferencial, 2 para tradicional
    Cliente* siguiente; // Puntero al siguiente cliente en la fila
};

// Función para crear un nuevo cliente
Cliente* crearCliente(int numeroCuenta, int tipoCuenta) {
    Cliente* nuevoCliente = (Cliente*)malloc(sizeof(Cliente));
    nuevoCliente->numeroCuenta = numeroCuenta;
    nuevoCliente->tipoCuenta = tipoCuenta;
    nuevoCliente->siguiente = NULL;
    return nuevoCliente;
}

// Definición de la clase para la fila de espera
class FilaEspera {
private:
    Cliente* frente; 
    Cliente* final; 
    int cantidadClientes; 
    
    int contadorTurnos;
public:
    
    FilaEspera() : frente(NULL), final(NULL), cantidadClientes(0) {}

    // Función para registrar un nuevo cliente en la fila
    void registrarCliente(int numeroCuenta, int tipoCuenta) {
        Cliente* nuevoCliente = crearCliente(numeroCuenta, tipoCuenta);

        if (frente ==NULL) { 
            frente = nuevoCliente;
            final = nuevoCliente;
        } else { 
            final->siguiente = nuevoCliente;
            final = nuevoCliente;
        }

        cantidadClientes++;

        
        transferirClientes();
    }

    // Función para transferir clientes entre filas si es necesario
    void transferirClientes() {
        if (cantidadClientes >= 5 && tipoClientes() == 2) { // Condición de transferencia
            Cliente* actual = frente;
            for (int i = 1; i < 3; i++) {
                actual = actual->siguiente; // Obtener el tercer cliente en la fila tradicional
            }

            // Quitar cliente de la fila tradicional
            Cliente* temp = actual->siguiente;
            actual->siguiente = temp->siguiente;
            temp->siguiente = NULL;

            // Agregar cliente a la fila preferencial
            temp->tipoCuenta = 1; 
            if (frente == NULL) { 
                frente = temp;
                final = temp;
            } else { 
                final->siguiente = temp;
                final = temp;
            }

            cantidadClientes--;
        }
    }
// Función para atender al cliente en el frente de la fila
    void atenderCliente() {
        if (frente != NULL) {
            Cliente* temp = frente;
            frente = frente->siguiente;
            free(temp); // Liberar la memoria del cliente atendido
            cantidadClientes--;

            // Reasignar los turnos a los clientes restantes
            Cliente* actual = frente;
            while (actual != NULL) {
                actual->turno = contadorTurnos++;
                actual = actual->siguiente;
            }
        } else {
            cout << "No hay clientes para atender." << endl;
        }
    }
    // Función para mostrar los clientes presentes en la fila
    void mostrarClientes() {
        Cliente* actual = frente;
        int turno = 1;
        while (actual != nullptr) {
            cout << "Turno " << turno << ": Cuenta #" << actual->numeroCuenta << " - ";
            cout << ((actual->tipoCuenta == 1) ? "Preferencial" : "Tradicional") << endl;
            actual = actual->siguiente;
            turno++;
        }
    }

    // Función para obtener el tipo de cuenta predominante en la fila
    int tipoClientes() {
        int preferencial = 0, tradicional = 0;
        Cliente* actual = frente;
        while (actual != NULL) {
            if (actual->tipoCuenta == 1)
                preferencial++;
            else
                tradicional++;
            actual = actual->siguiente;
        }
        return (preferencial > tradicional) ? 1 : 2;
    }
};
int main() {
    FilaEspera filaPreferencial, filaTradicional;
    int opcion;

    do {
        cout << "----- Menu -----" << endl;
        cout << "1. Registrar cliente" << endl;
        cout << "2. Mostrar filas de espera" << endl;
        cout << "3. Atender cliente" << endl;
        cout << "4. Salir" << endl;
        cout << "Ingrese su opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                int numeroCuenta, tipoCuenta;
                cout << "Ingrese el número de cuenta del cliente: ";
                cin >> numeroCuenta;
                cout << "Ingrese el tipo de cuenta (1 para preferencial, 2 para tradicional): ";
                cin >> tipoCuenta;
                if (tipoCuenta != 1 && tipoCuenta != 2) {
                    cout << "Tipo de cuenta inválido. Por favor ingrese 1 o 2." << endl;
                    break;
                }
                if (tipoCuenta == 1)
                    filaPreferencial.registrarCliente(numeroCuenta, tipoCuenta);
                else
                    filaTradicional.registrarCliente(numeroCuenta, tipoCuenta);
                cout << "Cliente registrado exitosamente." << endl;
                break;
            }
            case 2: {
                cout << "----- Filas de Espera -----" << endl;
                cout << "Fila Preferencial:" << endl;
                filaPreferencial.mostrarClientes();
                cout << endl << "Fila Tradicional:" << endl;
                filaTradicional.mostrarClientes();
                break;
            }
            case 3: {
                cout << "Atendiendo cliente..." << endl;
                filaPreferencial.atenderCliente();
                filaTradicional.atenderCliente();
                break;
            }
            case 4: {
                cout << "Saliendo del programa..." << endl;
                break;
            }
            default:
                cout << "Opción inválida. Por favor ingrese una opción válida." << endl;
        }

    } while (opcion != 4);

    return 0;
}
