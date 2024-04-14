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

void matrixToVector(C_Matrix& matrix, std::vector<int>& vector, const int& saltosFilas, const int& saltosColumnas) {
	int rows = matrix.LastRow();
	int columns = matrix.LastCol();

	//Como las matrices empieza en uno, entonces llegaremos hasta i y j <= rows y <= columns respectivamente
	for (int i = matrix.FirstRow(); i <= rows - saltosFilas; i++) {
		for (int j = matrix.FirstCol(); j <= columns - saltosColumnas; j++) {
			vector.push_back(matrix(i, j));
		}
	}
}

void copiarContenidoMatriz(C_Image& matrizImagen, C_Matrix& matrizCopiaContenido) {
	int rows = matrizCopiaContenido.LastRow();
	int columns = matrizCopiaContenido.LastCol();

	for (int i = matrizCopiaContenido.FirstRow() + 1; i <= rows - 1; i++) {
		for (int j = matrizCopiaContenido.FirstCol() + 1; j <= columns - 1; j++) {
			matrizCopiaContenido(i, j) = matrizImagen(i - 1, j - 1);
		}
	}
}

//void Mediana(C_Image& matrizImagen)
//{
//	//Definamos una matriz que será copia de la matriz de la imágen con la diferencia de que agregaremos sobre ella dos filas
//	//y dos columnas de ceros tanto al inicio como al final con el motivo de utilizarla para la eliminación de ruido
//	C_Matrix matrizCopiaContenido(matrizImagen.FirstRow(), matrizImagen.LastRow() + 2, matrizImagen.FirstCol(), matrizImagen.LastCol() + 2, 0);
//
//	//Llamemos a la función que copia el contenido de una matriz en otra, guardando las diferencias del problema
//	copiarContenidoMatriz(matrizImagen, matrizCopiaContenido);
//
//	//Definamos el vector donde volcaremos los elementos de la futura submatriz para su tratamiento
//	std::vector<int> vector;
//
//	//Definamos ciertas variables que estaremos utilizando
//	int rows = matrizCopiaContenido.LastRow();
//	int columns = matrizCopiaContenido.LastCol();
//
//	//Las respectivas matrices empiezan desde uno, entonces iteramos hasta rows/columns - 2 para no considerar las filas y columnas de ceros
//	//agregadas al momento de reemplazar el pixel con ruido con su correspondiente valor sin ruido
//	for (int i = matrizCopiaContenido.FirstRow(); i <= rows - 2; i++) {
//		for (int j = matrizCopiaContenido.FirstCol(); j <= columns - 2; j++) {
//			//Definamos una submatrix de dimension 3x3. Esta submatrix no será copia de la original, entonces no trabajaremos sobre ella,
//			//pero si la usaremos para facilitar el manejo sobre la matriz original. La elección de dimensión 3x3 ha sido para 
//			//para facilitar el calculo de la mediana debido que tendremos un vector de nueve elementos y, entonces, la mitad
//			//siempre será exactamente su tamaño partido entre dos 
//			C_Matrix submatrix(matrizCopiaContenido, 1, 3, 1, 3, i, j);
//
//			//Volquemos los elementos de la matriz al vector
//			matrixToVector(submatrix, vector);
//
//			//Ordenemos los elementos del vector
//			bubbleSort(vector);
//
//			//Reemplazamos el valor del pixel en la posición (i,j) por el valor de la mediana correspondiente dentro de la mascara centrada
//			//en ella 
//			matrizImagen(i, j) = vector[vector.size() / 2];
//
//			//Limpiemos el vector para futuras iteraciones
//			vector.clear();
//		}
//	}
//}

void mediana(C_Image& matrizImagen, const int& filasMascara, const int& columnasMascara) {
	// Declaramos la mascara como una submatriz de la matriz de la imagen que iremos desplazando sobre ella, descartando o no ciertas filas
	// o columnas finales de la mascara segun la ubicacion del pixel
	C_Matrix mascara(matrizImagen, 1, filasMascara, 1, columnasMascara, 1, 1);

	// Declaramos un vector de entero cuyo caracter sera dinamico
	std:vector<int> vector;
	
	// Declaramos una variable que sirva para determinar si un pixel se trata de un caso excepcional.
	// Un pixel excepcional sera aquel que haga que la mascara, donde es el centro, sobresalga respecto a la matriz de la imagen
	// Dependera de su ubicación en la matriz de la imagen y de las dimensiones de la mascara
	bool pixelExcepcional = false;

	// si i = 1 o i = filas o j = 1 o j = columnas, entonces pixel excepcional 100%
	// Partiendo de que filas como minimo es 3, entonces a partir de i = 1, si incremento a 5, entonces i = 2 tambien se cuenta y asi sigue... (lo mismo pasa al reves, i= filas, i =filas - 1...)
	// Lo mismo ocurre con las columnas donde como minimo es 3, entonces a partir de j = 1, si incremento a 5, entonces j = 2 tambien se cuenta y asi sigue... (lo mismo pasa al reves, i= columnas, i =columnas - 1...)

	//Cambiar nombre de variable a otro mas descriptivo
	//Minimamente filas y columnas son uno, entonces saltos en los bordes excepcionales como mucho es de 1: 3 - 1 = 2 --> 2/2 = 1. Asi con los demas
	int diferenciaFilas = filasMascara - 1;
	int diferenciaColumnas = columnasMascara - 1;

	// Saltos bordes excepcionales maximo
	int saltosFilasG = diferenciaFilas / 2;
	int saltosColumnasG = diferenciaColumnas / 2;
	
	// Saltos de un pixel cualquiera
	int saltosFilas, saltosColumnas = 0;
	

	//Iteramos sobre la matriz de la imagen
	for (int i = 1; i <= filasMascara; i++) {
		
		// Determinamos si el pixel es excepcional basandonos en la fila donde se encuentra
		if (1 <= i <= i + saltosFilasG || filasMascara - saltosFilasG <= i <= filasMascara) {
			pixelExcepcional = true;
			// Si fuera 3x3 descartariamos una fila. Si i = 1 tendriamos que descartar saltosFilasG = 1
			// Si fuera 5x5 descartariamos dos filas. Si i = 1 tendriamos que descartar saltosFilasG = 2, pero si fuera i = 2, solo tendriamos que
			// descartar una fila, es decir, saltosFilasG - 1. So on
			saltosFilas = saltosFilasG - (i-1);
		}

		for (int j = 1; j <= columnasMascara; j++) {
			// Determinar si el pixel se trata de un caso excepcional basandonos en la columna donde se encuentra
			if (pixelExcepcional != true && (1 <= j <= j + saltosColumnasG || columnasMascara - saltosColumnasG <= i <= columnasMascara)) {
				pixelExcepcional = true;
				saltosColumnas = saltosColumnasG - (j - 1);
			}

			// Desplazamos la mascara
			if (i != 1 || j != 1) {
				mascara.MoveSubMat(i, j);
			}

			// Procesamos al pixel segun su naturaleza
			if (pixelExcepcional) {
				// Cuando pasemos de matriz a vector, pasamos los saltos y copiaremos lenght - saltos (tanto de fila como columnas) respectivamente
				matrixToVector(mascara, vector, saltosFilas, saltosColumnas);
			}
			else {
				matrixToVector(mascara, vector, 0, 0);
			}

			// Ordenamos el vector

			// Reemplazamos el pixel de la matriz de la imagen por su mediana según la mascara

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
	}

	return 0;
}