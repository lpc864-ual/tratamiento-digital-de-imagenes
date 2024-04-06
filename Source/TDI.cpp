#include <iostream>
#include <vector>
#include <C_Matrix.hpp>
#include <C_Image.hpp>

int Test(int argc, char **argv);

//void printMatrix(const std::vector<std::vector<int>>& matrix) {
//	for (auto& rows : matrix) {
//		for (auto element : rows) {
//			std::cout << element << " ";
//		}
//		std::cout << std::endl;
//	}
//}

//void copyMatrixWithZeros(const std::vector<std::vector<int>>& originalMatrix, std::vector<std::vector<int>>& copyMatrix) {
//	int rows = copyMatrix.size();
//	int columns = copyMatrix[0].size();
//	
//	for (int i = 1; i <= rows - 2; i++) {
//		for (int j = 1; j <= columns - 2; j++) {
//			copyMatrix[i][j] = originalMatrix[i-1][j-1];
//		}
//	}
//}

//Modificar para que reciva los indices a partir de la cual la submatriz copiara del copyMatrix para luego tenerla, 
//vaciar sus elementos en un vector, ordenarlos y calcular la mediana dentro del vector
int calculateMedian(const C_Matrix& copyMatrix) {
	//Utilizar definicion de submatrix de uno de los constructores de C_Matrix
	return 0;
}

void printVector(std::vector<int>& vector) {
	int n = vector.size();

	for (int i = 0; i < n; i++) {
		std::cout << vector[i] << " ";
	}
	std::cout << std::endl;
}

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
	////La clase C_Image define un cuerpo para trabajar con imagenes, entonces definimos un objeto de dicha clase
	//C_Image image;

	////Cargamos la imagen de prueba en el objeto definido con anterioridad
	////image.ReadBMP("tomates_ruido.bmp");
	//image.ReadBMP("tomates_ruido.bmp");

	////Mostramos la matriz asociada a la imagen con ruido
	////image.Print(1, 3);

	/*std::vector<std::vector<int>> originalMatrix =
	{
		{1, 2, 3, 4},
		{5, 6, 7, 8}
	};*/

	/*std::vector<std::vector<int>> copyMatrix(originalMatrix.size()+2, std::vector<int>(originalMatrix[0].size()+2,0));
	
	copyMatrixWithZeros(originalMatrix, copyMatrix);
	
	printMatrix(copyMatrix);*/

	C_Matrix originalMatrix;
	//std::vector<int> vector;

	originalMatrix.Read("matrix.txt");

	std::cout << std::endl << std::endl;

	originalMatrix.Print(1, 1);

	std::cout << std::endl << std::endl;

	/*matrixToVector(originalMatrix, vector);

	printVector(vector);

	bubbleSort(vector);

	printVector(vector);

	std::cout << std::endl << std::endl;*/
	
	C_Matrix copyMatrix(originalMatrix.FirstRow(), originalMatrix.LastRow() + 2, originalMatrix.FirstCol(), originalMatrix.LastCol() + 2, 0);

	copyMatrixWithZeros(originalMatrix, copyMatrix);

	std::cout << std::endl << std::endl;

	copyMatrix.Print(1, 1);

	removeNoise(originalMatrix, copyMatrix);
	
	return 0;
}