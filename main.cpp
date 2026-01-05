#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <string>
#include <cmath>
#include <fstream> // Para leer archivos .txt

using namespace std;

// === CONFIGURACIÓN ===
const int NUM_SALAS = 7;
const int FILAS = 20;     
const int COLUMNAS = 30;  
const float CENTRO_FILA = FILAS / 2.0;     
const float CENTRO_COL = COLUMNAS / 2.0;

// === RANGOS DE LA ZONA VIP ===
// Fila 8 es la 'I', Fila 13 es la 'N'
const int FILA_VIP_INICIO = 8; 
const int FILA_VIP_FIN = 13;
// Columnas de la 10 a la 20 (Indices 9 a 19)
const int COL_VIP_INICIO = 9;
const int COL_VIP_FIN = 19;


// === COLORES ===
const string RESET = "\033[0m";
const string ROJO = "\033[41m\033[37m";    
const string VERDE = "\033[48;5;28m\033[37m";
const string VERDE_VIP = "\033[102m\033[30m"; 
const string ORO = "\033[43m\033[30m";     
const string AZUL_TXT = "\033[1;36m";      
const string GRIS = "\033[1;30m";

struct Asiento {
    bool ocupado;
    bool sugerido; 
};

struct Sala {
    int id;
    Asiento matriz[FILAS][COLUMNAS]; // <--- AQUÍ ESTÁ LA MATRIZ
};

vector<Sala> cine(NUM_SALAS);

// Inicialización Aleatoria (Para pruebas rápidas)
void inicializarCineAleatorio() {
    srand(time(0));
    for (int s = 0; s < NUM_SALAS; s++) {
        cine[s].id = s + 1;
        for (int i = 0; i < FILAS; i++) {
            for (int j = 0; j < COLUMNAS; j++) {
                cine[s].matriz[i][j].ocupado = (rand() % 100) < 40; // 40% ocupado
                cine[s].matriz[i][j].sugerido = false;
            }
        }
    }
}

void visualizarSala(int idSala) {
    if (idSala < 0 || idSala >= NUM_SALAS) return;

    cout << "\n\t\t\t      === SALA " << idSala + 1 << " ===\n";
    cout << "\t\t     (Zona VIP resaltada en verde claro)\n\n";
    
    // 1. IMPRIMIR ENCABEZADO DE COLUMNAS
    cout << "   ";
    for (int j = 0; j < COLUMNAS; j++) {
        // Pintamos de azul el número si pertenece a la columna VIP
        if(j >= COL_VIP_INICIO && j <= COL_VIP_FIN) cout << AZUL_TXT;
        cout << setw(2) << j + 1 << RESET << " ";
    }
    cout << endl;

    // 2. IMPRIMIR FILAS
    for (int i = 0; i < FILAS; i++) {
        // Pintamos de azul la letra si pertenece a la fila VIP
        if(i >= FILA_VIP_INICIO && i <= FILA_VIP_FIN) 
            cout << AZUL_TXT << " " << (char)('A' + i) << RESET << " ";
        else 
            cout << " " << (char)('A' + i) << " "; 
        
        // IMPRIMIR ASIENTOS DE LA FILA ACTUAL
        for (int j = 0; j < COLUMNAS; j++) {
            Asiento &a = cine[idSala].matriz[i][j];
            
            if (a.sugerido) {
                // Caso: Asiento seleccionado por el buscador (Dorado)
                cout << ORO << "[]" << RESET << " ";
            } else if (a.ocupado) {
                // Caso: Asiento ocupado (Rojo)
                cout << ROJO << "XX" << RESET << " ";
            } else {
                // Caso: Asiento Libre (Diferenciar VIP vs Normal)
                bool esFilaVip = (i >= FILA_VIP_INICIO && i <= FILA_VIP_FIN);
                bool esColVip = (j >= COL_VIP_INICIO && j <= COL_VIP_FIN);

                if (esFilaVip && esColVip) {
                    // Es VIP y Libre (Verde Brillante)
                    cout << VERDE_VIP << "  " << RESET << " ";
                } else {
                    // Es Normal y Libre (Verde Estándar)
                    cout << VERDE << "  " << RESET << " ";
                }
            }
        }
        
        // Repetir letra al final de la fila para referencia visual
        if(i >= FILA_VIP_INICIO && i <= FILA_VIP_FIN) 
            cout << AZUL_TXT << (char)('A' + i) << RESET << endl;
        else 
            cout << (char)('A' + i) << endl;
    }
    
    // 3. LEYENDA 
    cout << "\nLeyenda: " << VERDE << "  " << RESET << " General  " 
         << VERDE_VIP << "  " << RESET << " Zona VIP  " 
         << ROJO << "XX" << RESET << " Ocupado  " 
         << ORO << "[]" << RESET << " Tu Selección\n";
}


// === FUNCIÓN IMPORTAR ACTUALIZADA ===
void importarSalaDesdeArchivo(int idSala) {
    // 1. Mostrar MENSAJE DE AYUDA / PLANTILLA
    cout << "\n===========================================================\n";
    cout << "            GUÍA DE IMPORTACIÓN DE ARCHIVOS\n";
    cout << "===========================================================\n";
    cout << "Para importar, cree un archivo .txt en la misma carpeta.\n";
    cout << "El formato debe ser: 20 filas x 30 columnas.\n";
    cout << "Use " << VERDE << "0 para LIBRE" << RESET << " y " << ROJO << "1 para OCUPADO" << RESET << ".\n\n";
    
    cout << "Ejemplo de contenido (SALA VACÍA):\n" << AZUL_TXT;
    cout << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    cout << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    cout << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    cout << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    cout << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    cout << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    cout << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    cout << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    cout << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    cout << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    cout << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    cout << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    cout << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    cout << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    cout << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    cout << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    cout << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    cout << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    cout << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    cout << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0\n";
    cout << "===========================================================\n";

    // 2. Lógica de lectura (Igual que antes)
    string nombreArchivo;
    cout << "\nIngrese el nombre del archivo a leer (ej. sala_vacia.txt): ";
    cin >> nombreArchivo;

    ifstream archivo(nombreArchivo); // Abrir modo lectura

    if (!archivo.is_open()) {
        cout << ROJO << "Error: No se pudo abrir el archivo " << nombreArchivo << RESET << endl;
        cout << "Verifique que el nombre sea correcto y incluya la extensión .txt" << endl;
        return;
    }

    // Limpiamos sugerencias previas y leemos
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            int estadoLeido;
            if (archivo >> estadoLeido) {
                // 1 = Ocupado, 0 = Libre
                cine[idSala].matriz[i][j].ocupado = (estadoLeido == 1);
                cine[idSala].matriz[i][j].sugerido = false;
            } else {
                cine[idSala].matriz[i][j].ocupado = false; 
            }
        }
    }

    archivo.close();
    cout << VERDE << "¡Sala " << idSala + 1 << " importada correctamente!" << RESET << endl;
    
	visualizarSala(idSala); 
}

void limpiarSugerencias(int idSala) {
    for (int i = 0; i < FILAS; i++) {
        for (int j = 0; j < COLUMNAS; j++) {
            cine[idSala].matriz[i][j].sugerido = false;
        }
    }
}

double calcularDistanciaAlCentro(int fila, int colInicio, int nAsientos) {
    double y = fila; 
    double x = colInicio + (nAsientos - 1) / 2.0;
    return sqrt(pow(y - CENTRO_FILA, 2) + pow(x - CENTRO_COL, 2));
}

bool buscarMejorUbicacionCentral(int idSala, int n) {
    limpiarSugerencias(idSala);
    cout << "\nBuscando la ubicación más céntrica disponible...\n";

    double mejorDistancia = 99999.0;
    int mejorFila = -1;
    int mejorColInicio = -1;
    bool encontrado = false;

    for (int i = 0; i < FILAS; i++) {
        int contador = 0;
        for (int j = 0; j < COLUMNAS; j++) {
            if (!cine[idSala].matriz[i][j].ocupado) {
                contador++;
                if (contador >= n) {
                    int colInicioActual = j - n + 1;
                    double dist = calcularDistanciaAlCentro(i, colInicioActual, n);

                    if (dist < mejorDistancia) {
                        mejorDistancia = dist;
                        mejorFila = i;
                        mejorColInicio = colInicioActual;
                        encontrado = true;
                    }
                }
            } else {
                contador = 0;
            }
        }
    }

    if (encontrado) {
        for (int k = 0; k < n; k++) {
            cine[idSala].matriz[mejorFila][mejorColInicio + k].sugerido = true;
        }
        cout << ORO << "-> ¡Encontrado! Fila " << (char)('A' + mejorFila) 
             << " | Asientos " << (mejorColInicio + 1) << " al " << (mejorColInicio + n) << RESET << endl;
        return true;
    }

    cout << ROJO << "No hay espacio suficiente para " << n << " personas juntas." << RESET << endl;
    return false;
}



// === MENÚ CON SWITCH CASE ===
void menu() {
    int opcion;
    do {
        cout << "\n=======================================\n";
        cout << "   SISTEMA BUSQUEDA ASIENTOS CINE\n";
        cout << "=======================================\n";
        cout << "1. Ver estado de una Sala\n";
        cout << "2. Buscar mejor ubicación (Algoritmo Centralidad)\n";
        cout << "3. Generar simulación aleatoria\n";
        cout << "4. Importar Sala Personalizada (.txt)\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opción: ";
        cin >> opcion;

        // Estructura Switch solicitada
        switch (opcion) {
            case 1: {
                int sala;
                cout << "Ingrese número de Sala (1-7): ";
                cin >> sala;
                if(sala >=1 && sala <=7) visualizarSala(sala - 1);
                else cout << "Sala inválida.\n";
                break;
            }
            case 2: {
                int sala, n;
                cout << "Ingrese número de Sala (1-7): "; cin >> sala;
                cout << "¿Cuántos asientos necesita?: "; cin >> n;
                if(sala >=1 && sala <=7) {
                    if (buscarMejorUbicacionCentral(sala - 1, n)) {
                        visualizarSala(sala - 1);
                    }
                } else cout << "Sala inválida.\n";
                break;
            }
            case 3: {
                inicializarCineAleatorio();
                cout << AZUL_TXT << "Todas las salas han sido regeneradas aleatoriamente." << RESET << endl;
                break;
            }
            case 4: {
                int sala;
                cout << "Ingrese número de Sala a sobrescribir (1-7): "; cin >> sala;
                if(sala >=1 && sala <=7) {
                    importarSalaDesdeArchivo(sala - 1);
                } else cout << "Sala inválida.\n";
                break;
            }
            case 5:
                cout << "Saliendo del sistema...\n";
                break;
            default:
                cout << "Opción no reconocida. Intente de nuevo.\n";
                break;
        }

    } while (opcion != 5);
}

int main() {
    system("chcp 65001 > nul"); 
    inicializarCineAleatorio(); // Carga inicial por defecto
    menu();
    return 0;
}