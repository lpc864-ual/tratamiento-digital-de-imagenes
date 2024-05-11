#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <C_Matrix.hpp>
#include <C_Image.hpp>

//int Test(int argc, char **argv);

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
	for (int fila = primeraFilaMatrizImagen; fila <= ultimaFilaMatrizImagen; fila++) {
		for (int columna = primeraColumnaMatrizImagen; columna <= ultimaColumnaMatrizImagen; columna++) {
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
			std::sort(vector.begin(), vector.end());

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

	// Dividimos la mascara por la mitad
	int mitadFilasMascara = filasMascara / 2;
	int mitadColumnasMascara = columnasMascara / 2;

	// Definimos ciertos iteradores
	int i, j;

	// Iteramos sobre la matriz de la imagen
	for (int fila = primeraFilaMatrizImagen; fila <= ultimaFilaMatrizImagen; fila++) {
		for (int columna = primeraColumnaMatrizImagen; columna <= ultimaColumnaMatrizImagen; columna++) {
			i = matrizMascara.FirstRow();
			// Iteramos sobre la mascara
			for (int filaMascara = fila - mitadFilasMascara; filaMascara <= fila + mitadFilasMascara; filaMascara++) {
				// Determinamos si la fila del pixel de la mascara seria valida con respecto a la matriz de la imagen
				if (filaMascara < primeraFilaMatrizImagen || filaMascara > ultimaFilaMatrizImagen) {
					i++;
					continue;
				}
				j = matrizMascara.FirstCol();
				for (int columnaMascara = columna - mitadColumnasMascara; columnaMascara <= columna + mitadColumnasMascara; columnaMascara++) {
					// Determinamos si la columna del pixel de la mascara seria valida con respecto a la matriz de la imagen
					if (columnaMascara < primeraColumnaMatrizImagen || columnaMascara > ultimaColumnaMatrizImagen) {
						j++;
						continue;
					}
					matrizResultado(fila, columna) += std::abs(matrizImagen(filaMascara, columnaMascara) * matrizMascara(i, j));
					j++;
				}
				i++;
			}
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
	C_Image matrizImagen, matrizResultadoA, matrizResultadoB, matrizResultado;
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

		matrizResultado.Trunc(0, 255);

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

		std::cout << endl << endl;

		// Ignorar el carácter de nueva línea pendiente para evitar futuros problemas
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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
				matrizMascara.Resize(1, filasMascara, 1, columnasMascara, constante);
				convolucion(matrizImagen, filasMascara, columnasMascara, matrizMascara, matrizResultado);
			}
		}
		else if (opcion == 22 || opcion >= 32 && opcion <= 35) {
			matrizMascara.Resize(1, 3, 1, 3, 0);

			if (opcion == 22) {
				double pi = 3.14159265358979323846;
				double e = 2.718281828459045;
				double total = 0.0;
				double sigma;
				// Determinar valor de sigma
				std::cout << "Introduzca el valor de sigma (valor fraccionario, p.e., 1.0): ";
				std::cin >> sigma;

				// Ignorar el carácter de nueva línea pendiente
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				
				for (int i = matrizMascara.FirstRow(); i <= matrizMascara.LastRow(); i++) {
					for (int j = matrizMascara.FirstCol(); j <= matrizMascara.LastCol(); j++) {
						double exponente = -((std::pow(i, 2) + std::pow(j, 2)) / (2 * std::pow(sigma, 2)));
						matrizMascara(i, j) = (1 / (2 * pi * std::pow(sigma, 2))) * std::pow(e, exponente);
						total += matrizMascara(i, j);
					}
				}

				for (int i = matrizMascara.FirstRow(); i <= matrizMascara.LastRow(); i++) {
					for (int j = matrizMascara.FirstCol(); j <= matrizMascara.LastCol(); j++) {
						matrizMascara(i, j) /= total;
					}
				}

				convolucion(matrizImagen, 3, 3, matrizMascara, matrizResultado);
			}
			else if (opcion >= 31 && opcion <= 33) {
				// Preguntar si quiere detectar horizantal, verticales o ambas
				int a;
				std::cout << "horizontal (1), vertical(2) o ambas(3)?: ";
				cin >> a;

				// Ignorar el carácter de nueva línea pendiente
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				if (a == 1) {
					if (opcion == 31) {
						matrizMascara(1, 1) = 0;
						matrizMascara(1, 2) = 0;
						matrizMascara(1, 3) = 0;

						matrizMascara(2, 1) = 0;
						matrizMascara(2, 2) = -1;
						matrizMascara(2, 3) = 0;

						matrizMascara(3, 1) = 0;
						matrizMascara(3, 2) = 0;
						matrizMascara(3, 3) = 1;

						convolucion(matrizImagen, 3, 3, matrizMascara, matrizResultado);
					}
					else if (opcion == 32) {
						matrizMascara(1, 1) = -1;
						matrizMascara(1, 2) = -1;
						matrizMascara(1, 3) = -1;

						matrizMascara(2, 1) = 0;
						matrizMascara(2, 2) = 0;
						matrizMascara(2, 3) = 0;

						matrizMascara(3, 1) = 1;
						matrizMascara(3, 2) = 1;
						matrizMascara(3, 3) = 1;

						convolucion(matrizImagen, 3, 3, matrizMascara, matrizResultado);
					}
					else {
						matrizMascara(1, 1) = -1;
						matrizMascara(1, 2) = -2;
						matrizMascara(1, 3) = -1;

						matrizMascara(2, 1) = 0;
						matrizMascara(2, 2) = 0;
						matrizMascara(2, 3) = 0;

						matrizMascara(3, 1) = 1;
						matrizMascara(3, 2) = 2;
						matrizMascara(3, 3) = 1;

						convolucion(matrizImagen, 3, 3, matrizMascara, matrizResultado);
					}
				}
				else if (a == 2) {
					if (opcion == 31) {
						matrizMascara(1, 1) = 0;
						matrizMascara(1, 2) = 0;
						matrizMascara(1, 3) = 0;

						matrizMascara(2, 1) = 0;
						matrizMascara(2, 2) = 0;
						matrizMascara(2, 3) = -1;

						matrizMascara(3, 1) = 0;
						matrizMascara(3, 2) = 1;
						matrizMascara(3, 3) = 0;

						convolucion(matrizImagen, 3, 3, matrizMascara, matrizResultado);
					}
					else if (opcion == 32) {
						matrizMascara(1, 1) = -1;
						matrizMascara(1, 2) = 0;
						matrizMascara(1, 3) = 1;

						matrizMascara(2, 1) = -1;
						matrizMascara(2, 2) = 0;
						matrizMascara(2, 3) = 1;

						matrizMascara(3, 1) = -1;
						matrizMascara(3, 2) = 0;
						matrizMascara(3, 3) = 1;

						convolucion(matrizImagen, 3, 3, matrizMascara, matrizResultado);
					}
					else {
						matrizMascara(1, 1) = -1;
						matrizMascara(1, 2) = 0;
						matrizMascara(1, 3) = 1;

						matrizMascara(2, 1) = -2;
						matrizMascara(2, 2) = 0;
						matrizMascara(2, 3) = 2;

						matrizMascara(3, 1) = -1;
						matrizMascara(3, 2) = 0;
						matrizMascara(3, 3) = 1;

						convolucion(matrizImagen, 3, 3, matrizMascara, matrizResultado);
					}
				}
				else {
					matrizResultadoA.Resize(matrizImagen.FirstRow(), matrizImagen.LastRow(), matrizImagen.FirstCol(), matrizImagen.LastCol(), 0);
					matrizResultadoB.Resize(matrizImagen.FirstRow(), matrizImagen.LastRow(), matrizImagen.FirstCol(), matrizImagen.LastCol(), 0);
					
					if (opcion == 31) {
						matrizMascara(1, 1) = 0;
						matrizMascara(1, 2) = 0;
						matrizMascara(1, 3) = 0;

						matrizMascara(2, 1) = 0;
						matrizMascara(2, 2) = -1;
						matrizMascara(2, 3) = 0;

						matrizMascara(3, 1) = 0;
						matrizMascara(3, 2) = 0;
						matrizMascara(3, 3) = 1;

						convolucion(matrizImagen, 3, 3, matrizMascara, matrizResultadoA);

						matrizMascara.Clear();

						matrizMascara(1, 1) = 0;
						matrizMascara(1, 2) = 0;
						matrizMascara(1, 3) = 0;

						matrizMascara(2, 1) = 0;
						matrizMascara(2, 2) = 0;
						matrizMascara(2, 3) = -1;

						matrizMascara(3, 1) = 0;
						matrizMascara(3, 2) = 1;
						matrizMascara(3, 3) = 0;

						convolucion(matrizImagen, 3, 3, matrizMascara, matrizResultadoB);

						for (int i = matrizResultado.FirstRow(); i <= matrizResultado.LastRow(); i++) {
							for (int j = matrizResultado.FirstCol(); j <= matrizResultado.LastCol(); j++) {
								matrizResultado(i, j) = matrizResultadoA(i, j) + matrizResultadoB(i, j);
							}
						}
					}
					else if (opcion == 32) {
						matrizMascara(1, 1) = -1;
						matrizMascara(1, 2) = -1;
						matrizMascara(1, 3) = -1;

						matrizMascara(2, 1) = 0;
						matrizMascara(2, 2) = 0;
						matrizMascara(2, 3) = 0;

						matrizMascara(3, 1) = 1;
						matrizMascara(3, 2) = 1;
						matrizMascara(3, 3) = 1;

						convolucion(matrizImagen, 3, 3, matrizMascara, matrizResultadoA);

						matrizMascara(1, 1) = -1;
						matrizMascara(1, 2) = 0;
						matrizMascara(1, 3) = 1;

						matrizMascara(2, 1) = -1;
						matrizMascara(2, 2) = 0;
						matrizMascara(2, 3) = 1;

						matrizMascara(3, 1) = -1;
						matrizMascara(3, 2) = 0;
						matrizMascara(3, 3) = 1;

						convolucion(matrizImagen, 3, 3, matrizMascara, matrizResultadoB);

						for (int i = matrizResultado.FirstRow(); i <= matrizResultado.LastRow(); i++) {
							for (int j = matrizResultado.FirstCol(); j <= matrizResultado.LastCol(); j++) {
								matrizResultado(i, j) = matrizResultadoA(i, j) + matrizResultadoB(i, j);
							}
						}
					}
					else {
						matrizMascara(1, 1) = -1;
						matrizMascara(1, 2) = -2;
						matrizMascara(1, 3) = -1;

						matrizMascara(2, 1) = 0;
						matrizMascara(2, 2) = 0;
						matrizMascara(2, 3) = 0;

						matrizMascara(3, 1) = 1;
						matrizMascara(3, 2) = 2;
						matrizMascara(3, 3) = 1;

						convolucion(matrizImagen, 3, 3, matrizMascara, matrizResultadoA);

						matrizMascara(1, 1) = -1;
						matrizMascara(1, 2) = 0;
						matrizMascara(1, 3) = 1;

						matrizMascara(2, 1) = -2;
						matrizMascara(2, 2) = 0;
						matrizMascara(2, 3) = 2;

						matrizMascara(3, 1) = -1;
						matrizMascara(3, 2) = 0;
						matrizMascara(3, 3) = 1;

						convolucion(matrizImagen, 3, 3, matrizMascara, matrizResultadoB);

						for (int i = matrizResultado.FirstRow(); i <= matrizResultado.LastRow(); i++) {
							for (int j = matrizResultado.FirstCol(); j <= matrizResultado.LastCol(); j++) {
								matrizResultado(i, j) = matrizResultadoA(i, j) + matrizResultadoB(i, j);
							}
						}
					}
				}
			}
			else {
				// Preguntar si quiere detectar diagonales tambien
				int a;
				std::cout << "diagonales? (si = 1, no = 2): ";
				cin >> a;

				// Ignorar el carácter de nueva línea pendiente
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				if (a == 1) {
					matrizMascara(1, 1) = 0;
					matrizMascara(1, 2) = 1;
					matrizMascara(1, 3) = 0;

					matrizMascara(2, 1) = 1;
					matrizMascara(2, 2) = -4;
					matrizMascara(2, 3) = 1;

					matrizMascara(3, 1) = 0;
					matrizMascara(3, 2) = 1;
					matrizMascara(3, 3) = 0;

					convolucion(matrizImagen, 3, 3, matrizMascara, matrizResultado);
				}
				else {
					matrizMascara(1, 1) = 1;
					matrizMascara(1, 2) = 1;
					matrizMascara(1, 3) = 1;

					matrizMascara(2, 1) = 1;
					matrizMascara(2, 2) = -8;
					matrizMascara(2, 3) = 1;

					matrizMascara(3, 1) = 1;
					matrizMascara(3, 2) = 1;
					matrizMascara(3, 3) = 1;

					convolucion(matrizImagen, 3, 3, matrizMascara, matrizResultado);
				}
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

			convolucion(matrizImagen, 5, 5, matrizMascara, matrizResultado);
		}
		else if (opcion == 4) {
			// Si se trabajan con decimales debe estar en su forma decimal, no fraccionaria. Indicar .txt al final de la ruta

			//  Preguntar ruta de archivo .txt donde esta la matriz del filtro
			std::cout << "Introduzca la ruta de la mascara: ";
			std::getline(std::cin, ruta_mascara);

			matrizMascara.Read(ruta_mascara.c_str());

			convolucion(matrizImagen, matrizMascara.LastRow(), matrizMascara.LastCol(), matrizMascara, matrizResultado);

		}
		else {
			std::cout << endl << "ERROR: La opcion " << opcion << " no existe." << endl;
			aux = true;
		}

		if (!aux) {
			// Solicitamos al usuario la ruta donde sera guardada la imagen procesada
			std::cout << endl << endl << "Introduzca la ruta donde sera la guardada la imagen procesada: ";
			std::getline(std::cin, ruta_imagen_sin_ruido);;

			matrizResultado.Trunc(0, 255);

			matrizResultado.palette = matrizImagen.palette;

			// Escribamos el contenido de la matriz sobre un archivo
			matrizResultado.WriteBMP(ruta_imagen_sin_ruido.c_str());
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

			//LIMPIAR MATRIZ RESULTADO PARA EVITAR FUTUROS PROBLEMAS
		}
	} while (!aux);
}
