#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <C_Matrix.hpp>
#include <C_Image.hpp>

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
			if (vector.size() % 2 != 0) {
				matrizResultado(fila, columna) = vector[vector.size() / 2];
			}
			else {
				matrizResultado(fila, columna) = (vector[vector.size() / 2 - 1] + vector[vector.size() / 2]) / 2;
			}

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
					matrizResultado(fila, columna) += matrizImagen(filaMascara, columnaMascara) * matrizMascara(i, j);
					j++;
				}
				i++;
			}
		}
	}
}

int main(int argc, char** argv)
{
	// Variables para guardar las rutas hacia la imagen que se desea procesar, hacia una máscara, y hacia donde se guardará la imagen procesada. 
	std::string ruta_imagen_ruido, ruta_mascara, ruta_imagen_sin_ruido;

	// Instancias de la clase C_Image para trabajar con la matriz de una imagen
	C_Image matrizImagen, matrizResultadoA, matrizResultadoB, matrizResultado;

	// Instancia de la clase C_Matrix para trabajar con matrices.
	C_Matrix matrizMascara;

	// Variables para guardar la opción de procesamiento escogida por el usuario, así como las filas y columnas de la máscara con cual se esta 
	// trabajando
	int opcion, filasMascara, columnasMascara;

	// Variable auxiliar de tipo bool que será true cuando el programa tenga algún error o cuando el usuario desee volver a ejecutar el programa.
	bool aux = false;

	do
	{
		// Realizamos la cabecera de nuestro programa
		for (int i = 0; i < 91; i++) std::cout << "-";
		std::cout << " Bienvenido usuario ";
		for (int i = 0; i < 100; i++) std::cout << "-";

		// Indicamos tanto el objetivo como las restricciones de nuestro programa
		std::cout << endl;
		std::cout << "El objetivo de este programa es el procesamiento de una imagen. Antes de seguir deben mencionarse ciertas consideraciones y restricciones: " << std::endl << std::endl;
		std::cout << "1. Al momento de introducir la ruta hacia la imagen, el programa entiende que si introduce el nombre de la imagen como ruta, entonces dicha imagen esta dentro de la carpeta Run del directorio del programa." << std::endl << std::endl;
		std::cout << "2. Al momento de introducir el nombre de la imagen debe especificar su formato. En nuestro caso, estamos trabajando con imagenes en formato BMP." << std::endl << std::endl;
		std::cout << "4. Al momento de procesar la imagen estaremos utilizando matrices de dimensiones impar como mascaras." << std::endl << std::endl;
		std::cout << "5. La cuarta opcion de procesamiento utiliza una mascara contenida en un archivo en formato TXT para realizar la convolucion sobre la imagen seleccionada para el procesamiento." << std::endl << std::endl;
		std::cout << "6. Al momento de introducir el nombre de la mascara debe especificar su formato. En nuestro caso, estamos trabajando con mascaras contenidas en archivos en formato TXT." << std::endl << std::endl;
		std::cout << "7. El archivo que contenga la mascara debe tener el contenido de la mascara expresado como numeros enteros o fraccionarios" << std::endl << std::endl;
		std::cout << "Una vez realizadas estas aclaraciones, empecemos. " << std::endl << std::endl;

		// Solicitamos al usuario la ruta de la imagen que quiere procesar
		std::cout << "Introduzca la ruta de la imagen con intencion de procesar: ";
		std::getline(std::cin, ruta_imagen_ruido);

		// Obtenemos la matriz de la imagen 
		matrizImagen.ReadBMP(ruta_imagen_ruido.c_str());

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
		std::cout << "   4. Filtro de Laplaciano" << std::endl; 
		std::cout << "   5. Filtro de Marr-Hildreth" << std::endl << std::endl;
		std::cout << "4. Personalizado" << std::endl << std::endl;

		std::cout << "Introduzca una opcion (P.E. 11): ";
		std::cin >> opcion;

		std::cout << endl;

		// Ignoramos el carácter de nueva línea pendiente para evitar futuros problemas
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		// El usuario ha escogido el filtro de la mediana, el filtro de la media o filtro gaussiano respectivamente
		if (opcion == 11 || opcion == 21 || opcion == 22) {
			std::cout << "Introduzca el numero de filas y columnas de la mascara (separados con un espacio): ";
			std::cin >> filasMascara >> columnasMascara;

			std::cout << endl;

			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			
			// Control de errores
			if (filasMascara / 2 == 0 || columnasMascara / 2 == 0) {
				std::cout << "ERROR: El numero de filas o columnas es par cuando deben ser impar. " << std::endl;
				aux == true;
			}
			// Filtro de la mediana
			else if (opcion == 11) {
				mediana(matrizImagen, filasMascara, columnasMascara, matrizResultado);
			}
			// Filtro de la media
			else if (opcion == 21) {
				double constante = 1.0 / ((double)(filasMascara * columnasMascara));
				matrizMascara.Resize(1, filasMascara, 1, columnasMascara, constante);
				convolucion(matrizImagen, filasMascara, columnasMascara, matrizMascara, matrizResultado);
			}
			// Filtro gaussiano
			else {
				matrizMascara.Resize(1, filasMascara, 1, columnasMascara, 0);
				double pi = 3.14159265358979323846, e = 2.718281828459045, total = 0.0, sigma;

				std::cout << "Introduzca el valor de sigma (valor fraccionario, p.e., 1.0): ";
				std::cin >> sigma;

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
		}

		// El usuario ha escogido el filtro de roberts, el filtro de prewitt, el filtro de sobel o el filtro laplaciano respectivamente
		else if (opcion >= 31 && opcion <= 34) {
			// Las mascaras en estos filtros siempre son de dimension 3x3
			matrizMascara.Resize(1, 3, 1, 3, 0);

			// Filtro de roberts, filtro de prewitt o filtro de sobel respectivamente
			if (opcion >= 31 && opcion <= 33) {
				int x;
				std::cout << "La deteccion de bordes se realizara sobre bordes horizontales (1), verticales (2) o ambos (3)?: ";
				cin >> x;
				
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				if (x == 1) {
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
				else if (x == 2) {
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

			// Filtro laplaciano
			else {
				int x;
				std::cout << "La deteccion de bordes incluira las diagonales (si = 1, no = 2)?: ";
				cin >> x;

				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

				if (x == 1) {
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

		// El usuario ha escogido el filtro de Marr-Hildreth
		else if (opcion == 35) {
			// La mascara utilizada en este filtro tiene dimension 5x5
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

		// El usuario escoge utilizar una mascara personalizada para realizar el proceso de convolucion 
		else if (opcion == 4) {
			std::cout << "Introduzca la ruta de la mascara: ";
			std::getline(std::cin, ruta_mascara);

			matrizMascara.Read(ruta_mascara.c_str());

			convolucion(matrizImagen, matrizMascara.LastRow(), matrizMascara.LastCol(), matrizMascara, matrizResultado);

		}

		// Opcion invalida
		else {
			std::cout << endl << "ERROR: La opcion " << opcion << " no existe." << endl;
			aux = true;
		}

		if (!aux) {
			// Solicitamos al usuario la ruta donde sera guardada la imagen procesada
			std::cout << "Introduzca la ruta donde sera la guardada la imagen procesada: ";
			std::getline(std::cin, ruta_imagen_sin_ruido);;

			// Limitamos los valores de los elementos de una imagen a un rango específico
			matrizResultado.Trunc(0, 255);

			matrizResultado.palette = matrizImagen.palette;

			// Escribamos el contenido de la matriz sobre un archivo
			matrizResultado.WriteBMP(ruta_imagen_sin_ruido.c_str());
		}

		//
		std::cout << endl << "Quiere utilizar el programa otra vez? (si = 1, no = 2): ";
		std::cin >> opcion;

		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		if (opcion == 1) {
			std::system("cls");
			matrizImagen.Free();
			matrizMascara.Free();
			matrizResultadoA.Free();
			matrizResultadoB.Free();
			matrizResultado.Free();
			aux = false;
		}
		else {
			aux = true;
		}
	} while (!aux);

	return 0;
}