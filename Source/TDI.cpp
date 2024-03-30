#include <iostream>
#include <vector>
#include <C_Matrix.hpp>
#include <C_Image.hpp>

//int Test(int argc, char **argv);

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

int calculateMedian(const C_Matrix& copyMatrix) {
	//Utilizar definicion de submatrix de uno de los constructores de C_Matrix
	return 0;
}

void removeNoise(C_Matrix& originalMatrix, const C_Matrix& copyMatrix) {
	int rows = originalMatrix.LastRow();
	int columns = originalMatrix.LastCol();

	for (int i = originalMatrix.FirstRow(); i <= rows; i++) {
		for (int j = originalMatrix.FirstCol(); j <= columns; j++) {
			originalMatrix(i, j) = calculateMedian(copyMatrix);
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

	originalMatrix.Read("matrix.txt");

	originalMatrix.Print(1, 1);

	C_Matrix copyMatrix(originalMatrix.FirstRow(), originalMatrix.LastRow() + 2, originalMatrix.FirstCol(), originalMatrix.LastCol() + 2, 0);

	std::cout << std::endl << std::endl;

	copyMatrixWithZeros(originalMatrix, copyMatrix);

	copyMatrix.Print(1, 1);

	return 0;
	//return Test(argc, argv);
}