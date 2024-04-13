#include <iostream>
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

void matrixToVector(C_Matrix& matrix, std::vector<int>& vector) {
	int rows = matrix.LastRow();
	int columns = matrix.LastCol();

	//Como las matrices empieza en uno, entonces llegaremos hasta i y j <= rows y <= columns respectivamente
	for (int i = matrix.FirstRow(); i <= rows; i++) {
		for (int j = matrix.FirstCol(); j <= columns; j++) {
			vector.push_back(matrix(i, j));
		}
	}
}

void copiarContenidoMatriz(C_Image& matrizImagen, C_Matrix& matrizCopiaContenido) {
	int rows = matrizCopiaContenido.LastRow();
	int columns = matrizCopiaContenido.LastCol();

	for (int i = matrizCopiaContenido.FirstRow() + 1; i <= rows - 1; i++) {
		for (int j = matrizCopiaContenido.FirstCol() + 1; j <= columns - 1; j++) {
			matrizCopiaContenido(i, j) = matrizImagen(i-1, j-1);
		}
	}
}

void mediana(C_Image& matrizImagen)
{
	//Definamos una matriz que será copia de la matriz de la imágen con la diferencia de que agregaremos sobre ella dos filas
	//y dos columnas de ceros tanto al inicio como al final con el motivo de utilizarla para la eliminación de ruido
	C_Matrix matrizCopiaContenido(matrizImagen.FirstRow(), matrizImagen.LastRow() + 2, matrizImagen.FirstCol(), matrizImagen.LastCol() + 2, 0);

	//Llamemos a la función que copia el contenido de una matriz en otra, guardando las diferencias del problema
	copiarContenidoMatriz(matrizImagen, matrizCopiaContenido);

	//Definamos el vector donde volcaremos los elementos de la futura submatriz para su tratamiento
	std::vector<int> vector;

	//Definamos ciertas variables que estaremos utilizando
	int rows = matrizCopiaContenido.LastRow();
	int columns = matrizCopiaContenido.LastCol();

	//Las respectivas matrices empiezan desde uno, entonces iteramos hasta rows/columns - 2 para no considerar las filas y columnas de ceros
	//agregadas al momento de reemplazar el pixel con ruido con su correspondiente valor sin ruido
	for (int i = matrizCopiaContenido.FirstRow(); i <= rows - 2; i++) {
		for (int j = matrizCopiaContenido.FirstCol(); j <= columns - 2; j++) {
			//Definamos una submatrix de dimension 3x3. Esta submatrix no será copia de la original, entonces no trabajaremos sobre ella,
			//pero si la usaremos para facilitar el manejo sobre la matriz original. La elección de dimensión 3x3 ha sido para 
			//para facilitar el calculo de la mediana debido que tendremos un vector de nueve elementos y, entonces, la mitad
			//siempre será exactamente su tamaño partido entre dos 
			C_Matrix submatrix(matrizCopiaContenido, 1, 3, 1, 3, i, j);

			//Volquemos los elementos de la matriz al vector
			matrixToVector(submatrix, vector);

			//Ordenemos los elementos del vector
			bubbleSort(vector);

			//Reemplazamos el valor del pixel en la posición (i,j) por el valor de la mediana correspondiente dentro de la mascara centrada
			//en ella 
			matrizImagen(i, j) = vector[vector.size() / 2];

			//Limpiemos el vector para futuras iteraciones
			vector.clear();
		}
	}
}

void convolucion(C_Image& matrizImagen) {
	
}


int main(int argc, char **argv)
{
	//La clase C_Image define un cuerpo para trabajar con la matriz de una imagen, entonces definamos un objeto de dicha clase llamada "matrizImagen"
	C_Image matrizImagen;

	//Obtengamos la matriz de la imagen de prueba
	matrizImagen.ReadBMP("bear_ruido.bmp");

	//Llamemos a la función que elimine el ruido estilo "sal-pimienta" de la imagen
	mediana(matrizImagen);

	//Escribamos el contenido de la matriz sobre un archivo
	matrizImagen.Write("bear_sin_ruido.bmp");

	return 0;
}