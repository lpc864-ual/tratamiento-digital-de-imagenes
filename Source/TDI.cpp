#include <iostream>
#include <string>
#include <vector>
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

void mediana(C_Image& matrizImagen, const int& filasMascara, const int& columnasMascara) {
	// Almacenamos en variables los valores devueltos por las siguientes funciones para mejorar la eficiencia del programa
	int primeraFilaMatrizImagen = matrizImagen.FirstRow();
	int primeraColumnaMatrizImagen = matrizImagen.FirstCol();
	int ultimaFilaMatrizImagen = matrizImagen.LastRow();
	int ultimaColumnaMatrizImagen = matrizImagen.LastCol();

	// Dividimos la mascara por la mitad
	int mitadFilasMascara = filasMascara / 2;
	int mitadColumnasMascara = columnasMascara / 2;

	// Declaramos un vector de entero cuyo caracter sera dinamico
std:vector<int> vector;

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
					// Añadimos al vector
					vector.push_back(matrizImagen(filaMascara, columnaMascara));
				}
			}
			// Ordenamos el vector
			bubbleSort(vector);

			// Reemplazamos el pixel de la matriz de la imagen por su mediana según la mascara
			matrizImagen(fila, columna) = vector[(vector.size()) / 2];

			// Limpiamos el vector
			vector.clear();
		}
	}
}

void convolucion(C_Image& matrizImagen, const int& filas, const int& columnas, const std::string& nombreFiltro) {

}


int main(int argc, char** argv)
{
	// Declaramos variables.
	// La primera servirá para guardar la ruta hacia la imagen que queramos procesar.
	// La segunda servirá para guardar la ruta donde será guardada la imagen sin ruido.
	// La tercera representa una instancia de la clase C_Image. Dicha clase define un cuerpo para trabajar con la matriz de una imagen
	// Las dos últimas servirán para guardar tanto el numero de filas como el numero de columna de la matriz que estaremos utilizando como máscara 
	// para el procesamiento de la imagen.
	std::string ruta_imagen_ruido, ruta_imagen_sin_ruido;
	C_Image matrizImagen;
	int filasMascara, columnasMascara;

	// Realizamos la presentación de nuestro programa indicando tanto su objetivo como sus restricciones.
	// De igual modo, solicitamos al usuario la ruta de la imagen con ruido sal-pimienta y que será procesada para su tratamiento.
	for (int i = 0; i < 91; i++) std::cout << "-";
	std::cout << " Bienvenido usuario ";
	for (int i = 0; i < 100; i++) std::cout << "-";
	std::cout << std::endl << "El objetivo de este programa es eliminar ruido sal-pimienta de una imagen con formato BMP. Antes de seguir deben mencionarse ciertas restricciones: " << std::endl << std::endl;
	std::cout << "1. Si introduce el nombre de la imagen como ruta hacia ella, entonces, por defecto, el programa entiende que dicha imagen esta dentro de la carpeta Run del directorio del programa. " << std::endl;
	std::cout << "2. Al momento de introducir el nombre de la imagen debe especificar su formato. En nuestro caso, estamos trabajando con imagenes en formato BMP." << std::endl;
	std::cout << "3. Las dimensiones de la matriz que estaremos utilizando como mascara deben ser impar" << std::endl << std::endl;
	std::cout << "Como resultado de lo anterior, si tiene una imagen con formato BMP dentro de la carpeta Run, ubicado en el directorio del programa, entonces su ruta sera: nombreImagen.bmp" << std::endl << std::endl;
	std::cout << "Una vez realizadas estas aclaraciones, empecemos. " << std::endl << std::endl;

	std::cout << "Introduzca la ruta de la imagen con formato BMP con ruido sal-pimienta: ";
	std::getline(std::cin, ruta_imagen_ruido);

	// Obtenemos la matriz de la imagen 
	matrizImagen.ReadBMP(ruta_imagen_ruido.c_str());

	// Por defecto, si la ruta especificada es incorrecta, entonces la matriz asociada a la imagen estará vacía.
	// Como resultado, solo trabajaremos cuando la matriz asociada a la imagen no este vacía.
	if (matrizImagen.Empty() != true) {

		// Solicitamos al usuario las dimensiones de la mascara
		std::cout << "Introduzca el numero de filas y columnas de la mascara (separados con un espacio): ";
		std::cin >> filasMascara >> columnasMascara;
		// Si las dimensiones de la mascara no son impar, entonces mensaje de error y terminamos el programa
		if (filasMascara / 2 == 0 || columnasMascara / 2 == 0) {
			std::cout << "ERROR: El numero de filas o columnas es par cuando deben ser impar. " << std::endl;
			return 0;
		}

		// Limpiamos el buffer de entrada para evitar futuros problemas. 
		// En este código, std::numeric_limits<std::streamsize>::max() devuelve el número máximo de caracteres que se pueden
		// extraer del stream. El carácter '\n' es el delimitador que indica hasta dónde se debe ignorar el stream.
		// Por lo tanto, esta línea de código ignora todos los caracteres en el buffer de entrada hasta 
		// que encuentra un salto de línea ('\n'), lo que efectivamente limpia el buffer.
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		// Aplicamos el operador de la mediana sobre la imagen para eliminar gran parte del ruido.
		//Mediana(matrizImagen);
		mediana(matrizImagen, filasMascara, columnasMascara);

		// Solicitamos al usuario la ruta donde será guardada la imagen sin ruido sal-pimienta
		std::cout << "Introduzca la ruta donde sera la guardada la imagen con formato BMP sin ruido sal-pimienta: ";
		std::getline(std::cin, ruta_imagen_sin_ruido);

		// Escribamos el contenido de la matriz sobre un archivo
		matrizImagen.Write(ruta_imagen_sin_ruido.c_str());

		return 0;
	}
}