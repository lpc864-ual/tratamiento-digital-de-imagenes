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

void removeNoise(C_Matrix& originalMatrix, C_Matrix& copyMatrix) {
	//Definimos el vector donde volcaremos los elementos de la submatriz para su tratamiento
	std::vector<int> vector;
	
	int rows = copyMatrix.LastRow();
	int columns = copyMatrix.LastCol();

	//Iteramos hasta i <= rows - 2 y j <= colums - 2 para no considerar las filas y columnas de ceros para no desbordarnos al generar las submatrices.
	//que usaremos para el calculo de la mediana de los pixeles que queremos. 
	//Si rows y columns empezaran desde 0, entonces tendriamos que restar tres
	for (int i = copyMatrix.FirstRow(); i <= rows - 2; i++) {
		for (int j = copyMatrix.FirstCol(); j <= columns - 2; j++) {
			//Utilizaremos una submatrix. Este submatrix no será copia de la original, entonces no trabajaremos sobre ella, pero si la usaremos 
			//para facilitar el manejo sobre la matriz original
			C_Matrix submatrix(copyMatrix, 1, 3, 1, 3, i, j);

			matrixToVector(submatrix, vector);

			bubbleSort(vector);

			//Tomamos la mediana. Estamos trabajando con submatrices 3x3 para facilitar el calculo de la mediana debido que tendremos
			//un vector de nueve elementos y entonces la mitad siempre será exactamente su tamaño entre dos 
			originalMatrix(i, j) = vector[vector.size() / 2];

			//Limpiamos el vector
			vector.clear();
		}
	}
}

void copyMatrixWithZeros(C_Matrix& originalMatrix, C_Matrix& copyMatrix) {
	int rows = copyMatrix.LastRow();
	int columns = copyMatrix.LastCol();

	for (int i = copyMatrix.FirstRow() + 1; i <= rows - 1; i++) {
		for (int j = copyMatrix.FirstCol() + 1; j <= columns - 1; j++) {
			copyMatrix(i, j) = originalMatrix(i-1, j-1);
		}
	}
}

int main(int argc, char **argv)
{
	
	//La clase C_Image define un cuerpo para trabajar con imagenes, entonces definimos un objeto de dicha clase
	C_Image image;

	//Cargamos la imagen de prueba en el objeto definido con anterioridad
	image.ReadBMP("bear_ruido.bmp");

	//Definimos una matriz copia de la original, pero con ceros
	C_Matrix copyMatrix(image.FirstRow(), image.LastRow() + 2, image.FirstCol(), image.LastCol() + 2, 0);

	//Copiamos su contenido
	copyMatrixWithZeros(image, copyMatrix);

	//Eliminamos el ruido
	removeNoise(image, copyMatrix);

	//Escribimos sobre un nuevo archivo
	image.Write("bear_sin_ruido.bmp");

	return 0;
}