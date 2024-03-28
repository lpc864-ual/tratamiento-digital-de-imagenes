# TDI - Proyecto 

Partimos de una imagen con ruido:

1. Reducimos ruido suavizando la imagen usando el algoritmo de la mediana --> En un par de horas se consigue

2. Tras ello, nuestra intencion sera detectar los bordes de la imagen, pero los bordes seran detectados para cierto umbral.
   Entonces realizar el calculo del valor del umbral utilizando el metodo de Otsu --> Hasta aqui estaria bien el proyecto

3. Una vez calculado el valor del umbral nuestra intencion sera realizar la deteccion de borde según este umbral  --> Adicional

# Parte 01: Eliminación de ruido utilizando la mediana

1.  Partimos de una imagen, A, en formato BMP. Este formato facilitará trabajar sobre la imagen porque no aplica sobre la imagen ningun algoritmo de compresión.
    Además, como veremos dentro de la librería del profesor, más allá de tener un método generico de lectura de imagenes de distinto formato, tenemos un método exclusivo para leer imagenes dentro del formato BMP.

2.  Obtenemos la matriz asociada a la imagen, M, utilizando el método ReadBMP() del archivo de cabecera C_Image.hpp. Para evitar futuros problemas en relación a la
    convolución, agregaremos, sobre la matriz generada, una nueva fila y columna de ceros. Dicha fila y columna representarán la primera fila y la primera columna respectivamente.

3.  Definimos una máscara, M', utilizando el constructor creador de submatrices del archivo de cabecera C_Matrix.hpp. La matriz generada por el constructor será una     submatriz de la matriz de la imagen. Esta submatriz será de dimensión 3x3. Inicialmente la submatriz será generada en base a la primera fila y primera columna de la matriz de la imagen, luego iremos incremento tanto la fila y columna a partir de la cual se basa la creación de la submatriz sobre la matriz de la imagen. El centro de la submatriz será el pixel, p, que será transformado. La transformación ocurre al ordenar los elementos de la submatriz siguiendo cierto orden (p.e. orden ascendente) y calculando la mediana. Esta mediana representará el pixel, p', que sustituirá al pixel original, p. 

4.  Una vez realizado el proceso anterior sobre cada pixel de la imagen original, tendremos casi la matriz asociada a la imagen sin ruido, M''. Lo único restante es deshacernos tanto de la primera fila y la segunda columna de la matriz obtenida, cubiertas de ceros e introducidas en el paso número dos. Una vez hecho esto, entonces tendremos la matriz asociada a la imagen sin ruido, M''

10. A partir de la matriz obtenida, M'', obtenemos la imagen sin ruido, A', utilizando el método WriteBMP() del archivo de cabecera C_Image.hpp.

Vease el siguiente video explicativo ante cualquier duda: https://www.youtube.com/watch?v=3kwH3Rb5LJ4