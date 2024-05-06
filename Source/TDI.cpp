#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <C_Matrix.hpp>
#include <C_Image.hpp>

//int Test(int argc, char **argv);

void bubbleSort(std::vector<int>& vector) {
	int n = vector.size();

	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - 1 - i; j++) {
			if (vector[j] > vector[j + 1]) {
				std::swap(vector[j], vector[j + 1]);
			}
		}
	}
}

void mediana(C_Image& matrizImagen, const int& filasMascara, const int& columnasMascara, C_Image& matrizResultado) {
	// Almacenamos en variables los valores devueltos por las siguientes funciones para mejorar la eficiencia del programa
	int primeraFilaMatrizImagen = matrizImagen.FirstRow();
	int primeraColumnaMatrizImagen = matrizImagen.FirstCol();
	int ultimaFilaMatrizImagen = matrizImagen.LastRow();
	int ultimaColumnaMatrizImagen = matrizImagen.LastCol();

	// Dividimos la mascara por la mitad
	int mitadFilasMascara = filasMascara / 2;
	int mitadColumnasMascara = columnasMascara / 2;

	// Declaramos un vector de entero cuyo caracter sera dinamico
    std::vector<int> vector;

	// Iteramos sobre la matriz de la imagen
	for (int fila = matrizImagen.FirstRow(); fila <= ultimaFilaMatrizImagen; fila++) {
		for (int columna = matrizImagen.FirstCol(); columna <= ultimaColumnaMatrizImagen; columna++) {
			// Iteramos sobre la mascara
			for (int filaMascara = fila - mitadFilasMascara; filaMascara <= fila + mitadFilasMascara; filaMascara++) {
				// Determinamos si la fila del pixel de la mascara seria valida con respecto a la matriz de la imagen
				if (filaMascara < primeraFilaMatrizImagen || filaMascara > ultimaFilaMatrizImagen) continue;
				for (int columnaMascara = columna - mitadColumnasMascara; columnaMascara <= columna + mitadColumnasMascara; columnaMascara++) {
					// Determinamos si la columna del pixel de la mascara seria valida con respecto a la matriz de la imagen
					if (columnaMascara < primeraColumnaMatrizImagen || columnaMascara > ultimaColumnaMatrizImagen) continue;
					// Anadimos al vector
					vector.push_back(matrizImagen(filaMascara, columnaMascara));
				}
			}
			// Ordenamos el vector
			bubbleSort(vector);

			// Rellenamos la matrizResultado con el valor correspondiente al pixel
			matrizResultado(fila, columna) = vector[(vector.size()) / 2];

			// Limpiamos el vector
			vector.clear();
		}
	}
}

void convolucion(C_Image& matrizImagen, const int& filasMascara, const int& columnasMascara, C_Matrix& matrizMascara, C_Image& matrizResultado) {
	// Almacenamos en variables los valores devueltos por las siguientes funciones para mejorar la eficiencia del programa
	int primeraFilaMatrizImagen = matrizImagen.FirstRow();
	int primeraColumnaMatrizImagen = matrizImagen.FirstCol();
	int ultimaFilaMatrizImagen = matrizImagen.LastRow();
	int ultimaColumnaMatrizImagen = matrizImagen.LastCol();
	int primeraFilaMatrizMascara = matrizMascara.FirstRow();
	int primeraColumnaMatrizMascara = matrizMascara.FirstCol();

	// Dividimos la mascara por la mitad
	int mitadFilasMascara = filasMascara / 2;
	int mitadColumnasMascara = columnasMascara / 2;

	// Definimos ciertos iteradores
	int i, j;

	// Declaramos un vector de entero cuyo caracter sera dinamico
	std::vector<int> vectorImagen;
	std::vector<double> vectorMascara, vectorResultado;

	// Definimos cierta variable utilizada mas adelante
	int suma = 0;

	// Iteramos sobre la matriz de la imagen
	for (int fila = matrizImagen.FirstRow(); fila <= ultimaFilaMatrizImagen; fila++) {
		for (int columna = matrizImagen.FirstCol(); columna <= ultimaColumnaMatrizImagen; columna++) {
			// Definimos iteradores
			i = primeraFilaMatrizMascara;
			// Iteramos sobre la mascara
			for (int filaMascara = fila - mitadFilasMascara; filaMascara <= fila + mitadFilasMascara; filaMascara++) {
				// Definimos iteradores
				j = primeraColumnaMatrizMascara;
				// Determinamos si la fila del pixel de la mascara seria valida con respecto a la matriz de la imagen
				if (filaMascara < primeraFilaMatrizImagen || filaMascara > ultimaFilaMatrizImagen) {
					i++;
					continue;
				}
				for (int columnaMascara = columna - mitadColumnasMascara; columnaMascara <= columna + mitadColumnasMascara; columnaMascara++) {
					// Determinamos si la columna del pixel de la mascara seria valida con respecto a la matriz de la imagen
					if (columnaMascara < primeraColumnaMatrizImagen || columnaMascara > ultimaColumnaMatrizImagen) {
						j++;
						continue;
					}

					// Anadimos al vector
					vectorImagen.push_back(matrizImagen(filaMascara, columnaMascara));
					//std::cout << "i, j: " << i << ", " << j;
					vectorMascara.push_back(matrizMascara(i, j));
					j++;
				}
				i++;
			}

			// Los vectores obtenidos tienen la misma dimension
			// Multiplicamos sus elementos uno a uno
			for (int k = 0; k < vectorImagen.size(); k++) {
				vectorResultado.push_back(((double)vectorImagen[k]) * vectorMascara[k]);
			}

			// Sumamos los elementos de vectorResultado
			for (int k = 0; k < vectorResultado.size(); k++) {
				suma += (int)vectorResultado[k];
			}

			// Rellenamos la matrizResultado con el valor correspondiente al pixel
			matrizResultado(fila, columna) = suma;

			// Limpiamos los vectores
			vectorImagen.clear();
			vectorMascara.clear();
			vectorResultado.clear();

			// Reiniciamos el contador de la suma
			suma = 0;
		}
	}
}

/*
	TERMINAR: DEFINIR LAS DISTINTAS MASCARAS
*/

int main(int argc, char** argv)
{
	// Declaramos variables.
	// La primera servira para guardar la ruta hacia la imagen que queramos procesar.
	// La segunda servira para guardar la ruta donde sera guardada la imagen procesada.
	// La tercera y la cuarta representan instancias de la clase C_Image. Dicha clase define un cuerpo para trabajar con la matriz de una imagen
	// Las dos ultimas serviran para guardar tanto el numero de filas como el numero de columna de la matriz que estaremos utilizando como mascara 
	// para el procesamiento de la imagen.
	std::string ruta_imagen_ruido, ruta_imagen_sin_ruido, ruta_mascara;
	C_Image matrizImagen, matrizResultado;
	C_Matrix matrizMascara;
	int opcion, filasMascara, columnasMascara;
	bool aux = false;

	do
	{
		// Realizamos la cabecera de nuestro programa
		for (int i = 0; i < 91; i++) std::cout << "-";
		std::cout << " Bienvenido usuario ";
		for (int i = 0; i < 100; i++) std::cout << "-";

		// Indicamos tanto el objetivo como las restricciones de nuestro programa
		std::cout << endl;
		std::cout << "El objetivo de este programa es el procesamiento de una imagen. Antes de seguir deben mencionarse ciertas restricciones: " << std::endl << std::endl;
		std::cout << "1. Al momento de introducir la ruta hacia la imagen, el programa entiende que si introduce el nombre de la imagen como ruta, entonces dicha imagen esta dentro de la carpeta Run del directorio del programa." << std::endl << std::endl;
		std::cout << "2. Al momento de introducir el nombre de la imagen debe especificar su formato. En nuestro caso, estamos trabajando con imagenes en formato BMP." << std::endl << std::endl;
		std::cout << "3. Al momento de procesar la imagen estaremos utilizando matrices de dimensiones impar como mascaras." << std::endl << std::endl;
		std::cout << "Una vez realizadas estas aclaraciones, empecemos. " << std::endl << std::endl;

		// Solicitamos al usuario la ruta de la imagen que quiere procesar
		std::cout << "Introduzca la ruta de la imagen con intencion de procesar: ";
		std::getline(std::cin, ruta_imagen_ruido);

		// Obtenemos la matriz de la imagen 
		matrizImagen.ReadBMP(ruta_imagen_ruido.c_str());

		// Controlar error para que si salta no pidamos ninguna opcion de procesado

		// Matriz donde guardaremos el resultado de procesar sobre la matriz de la imagen
		matrizResultado.Resize(matrizImagen.FirstRow(), matrizImagen.LastRow(), matrizImagen.FirstCol(), matrizImagen.LastCol(), 0);

		// Mostramos al usuario las opciones de procesado
		std::cout << endl << "Opciones de procesado: " << std::endl << std::endl;
		std::cout << "1. Eliminacion de ruido" << std::endl;
		std::cout << "   1. Filtro de la mediana" << std::endl << std::endl;
		std::cout << "2. Suavizado" << std::endl;
		std::cout << "   1. Filtro de la media" << std::endl;
		std::cout << "   2. Filtro de gaussiano" << std::endl << std::endl;
		std::cout << "3. Deteccion de bordes" << std::endl;
		std::cout << "   1. Filtro de Roberts" << std::endl;
		std::cout << "   2. Filtro de Prewitt" << std::endl;
		std::cout << "   3. Filtro de Sobel" << std::endl;
		std::cout << "   4. Filtro de Marr-Hildreth" << std::endl;
		std::cout << "   5. Filtro Laplaciano" << std::endl << std::endl;
		std::cout << "4. Personalizado" << std::endl << std::endl;

		std::cout << "Introduzca una opcion (P.E. 11): ";
		std::cin >> opcion;

		std::cout << endl << endl;;

		// Ignorar el carácter de nueva línea pendiente para evitar futuros problemas
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		// 
		if (opcion == 11 || opcion == 21) {
			std::cout << "Introduzca el numero de filas y columnas de la mascara (separados con un espacio): ";
			std::cin >> filasMascara >> columnasMascara;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			if (filasMascara / 2 == 0 || columnasMascara / 2 == 0) {
				std::cout << "ERROR: El numero de filas o columnas es par cuando deben ser impar. " << std::endl;
				aux == true;
			}
			else if (opcion == 11) {
				mediana(matrizImagen, filasMascara, columnasMascara, matrizResultado);
			}
			else {
				double constante = 1.0 / ((double)(filasMascara * columnasMascara));
				matrizMascara.Resize(1, filasMascara, 1, columnasMascara, 0);

				for (int i = 1; i <= matrizMascara.LastRow(); i++) {
					for (int j = 1; j <= matrizMascara.LastCol(); j++) {
						matrizMascara(i, j) = constante;
					}
				}

				convolucion(matrizImagen, filasMascara, columnasMascara, matrizMascara, matrizResultado);
			}
		}
		else if (opcion == 22 || opcion == 31 || opcion == 32 || opcion == 33 || opcion == 35) {
			matrizMascara.Resize(1, 3, 1, 3, 0);

			if (opcion == 22) {
				// Determinar valor de sigma
			}
			else if (31 <= opcion <= 33) {
				// Preguntar si quiere detectar horizantal, verticales o ambas
			}
			else {
				// Preguntar si quiere detectar diagonales tambien
			}
		}
		else if (opcion == 34) {
			matrizMascara.Resize(1, 5, 1, 5, 0);

			for (int i = 1; i < matrizMascara.LastRow(); i++) {
				if (i == 1 || i == 5) {
					matrizMascara(i, 1) = 0;
					matrizMascara(i, 2) = 0;
					matrizMascara(i, 3) = -1;
					matrizMascara(i, 4) = 0;
					matrizMascara(i, 5) = 0;
				}
				else if (i == 2 || i == 4) {
					matrizMascara(1, 1) = 0;
					matrizMascara(1, 2) = -1;
					matrizMascara(1, 3) = -2;
					matrizMascara(1, 4) = -1;
					matrizMascara(1, 5) = 0;
				}
				else {
					matrizMascara(2, 1) = -1;
					matrizMascara(2, 2) = -2;
					matrizMascara(2, 3) = 16;
					matrizMascara(2, 4) = -2;
					matrizMascara(2, 5) = -1;
				}
			}
		}
		else if (opcion == 4) {
			//  Preguntar ruta de archivo .txt donde esta la matriz del filtro
		}
		else {
			std::cout << endl << "ERROR: La opcion " << opcion << " no existe." << endl;
			aux = true;
		}

		if (!aux) {
			// Solicitamos al usuario la ruta donde sera guardada la imagen procesada
			std::cout << endl << endl << "Introduzca la ruta donde sera la guardada la imagen procesada: ";
			std::getline(std::cin, ruta_imagen_sin_ruido);

			// Escribamos el contenido de la matriz sobre un archivo
			matrizResultado.Write(ruta_imagen_sin_ruido.c_str());
		}

		//
		std::cout << endl << "Quiere utilizar el programa otra vez? (1 = si, 2 = no): ";
		std::cin >> opcion;

		// Ignorar el carácter de nueva línea pendiente
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (opcion == 1) {
			std::system("cls");
			aux = false;
		}
		else {
			aux = true;
		}

	} while (!aux);
}
