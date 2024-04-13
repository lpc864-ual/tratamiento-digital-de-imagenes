# Nuestro objetivo

El objetivo primario es eliminar el ruido de tipo sal-pimienta de una imagen. Opcionalmente, se tiene la motivación de realizar detección de bordes siguiendo cierto umbral.

# Nuestro esquema

El programa recibe como parametro la ruta hacia la imagen con ruido de tipo sal-pimienta. La imagen debe encontrarse dentro del proyecto en la carpeta **Run**.
De igual modo, recibirá las dimensiones de la matriz que estaremos utilizando como máscara en los diversos algoritmos que estemos empleando para conseguir nuestro objetivo. Opcionalmente, recibiría un umbral, necesario si queremos realizar la detección de bordes. 

A partir de la ruta hacia la imagen, el programa obtiene su matriz. Una vez obtenida su matriz, el programa llama al método de la mediana para la eliminación de ruido. Este método recibira la matriz de la imagen y las dimensiones de la máscara. Estando dentro del método se seguiría el siguiente esquema:

1. Declaramos e inicializamos una matriz cuyas dimensiones sean las de la máscara y cuyos valores esten sean unos.

2. Declaramos un vector cuyo tamaño será nxm, siendo n y m las dimensiones de la máscara antes mencionada

3. Iteraremos sobre la matriz de la imagen

4. Centramos la mascara sobre un pixel de la matriz. Notese que el pixel el pixel de la matriz de la imagen sobre el que este centrado la máscara puede hacer que la máscara sobresalga respecto a la matriz de la imagen. Para evitar dichas situaciones y hacer generico el calculo de la mediana, entonce determinemos los valores de la mascara que si entran dentro de la matriz de la imagen. Para ello, partamos del hecho que dichos valores definirán una submatriz. Entendido esto, entonces la tarea será determinar la fila y columna de la matriz de la imagen sobre la cual esta definido dicha submatriz junto a sus dimensiones. Dichos valores los utilizaremos despues para evitar crear y trabajar con submatrices porque supondria una pérdida en la eficiencia debido a un consumo innecesario en memoria. Tras decir lo anterior solo quedará volcar los valores de la matriz en el vector. Este vector sera ordenado, pero solo los valores que queremos. Como volcamos de fila en fila, entonces el tema sera comenzar la ordenación a partir de la fila válida y columna válida hasta la cantidad definida por el producto de las dimensiones de la supuesta submatriz. Una vez ordenado dicho subvector, entonces calculamos de él la mediana. Para ellos partiremos de los indices tanto de inicio como de fin del subvector, los sumamos (los indices), le sumamos uno por motivo de seguridad (sigo sin entender por qué) y dividimos entre dos. El valor obtenido es el indice donde se encontrará el elemento que representará la mediana y será el valor que reemplace al pixel. En el proceso anterior estuvimos multiplicando los valores de la mascara por los pixeles de la region de la matriz de la imagen sobre la cual esta definida, entonces el siguiente paso será hacer que los valores de la mascara vuelvan a sus valores iniciales, es decir, uno. Jugando con los datos antes dichos estaremos re-inicializando los valores de la mascara a uno despues de cada iteración. De igual modo, debe decirse que antes de la siguiente iteracion, el vector debe ser limpiado, caso contrario, en las siguientes iteraciones estaremos trabajando con valores previos y afectará a los resultado. El proceso seguirá sobre cada píxel de la matriz de la imagen. Cabe mencionar que en el mejor de los casos los valores de la mascara si entraran en la matriz de la imagen, entonces el proceso sera tan simple como realizar la multiplicacion de la mascara por la region de la matriz de la imagen sobre la cual esta definida, volcar su elementos en el vector, ordenar el vector, calcular la mediana, re-inicializar todos los valores de la mascara a uno y limpiamos el vector. 

El proceso anterior ha eliminado parte del ruido, pero no todo. Como resultado, procedamos hacia una limpieza del ruido más profunda. Para ello, nos basaremos en la convolucion. Más concretamente estaremos empleando el filtro de la media. En esencia, la convolución es el proceso de partir de una mascara, centrarla sobre el pixel de una matriz, multiplicar sus valores y sumarlos. En función del filtro usado los valores de la mascara serán de un tipo u otro. Dicho esto, el método encargado de la convolución recibirá la matriz de la imágen, las dimensiones de la máscara y el nombre del filtro. En nuestro caso, el nombre del filtro será "media". Una vez dicho esto, detallemos el cuerpo del método:

1. Se tienen diferentes condicionales cuyo proposito será declarar e inicializar los valores de la máscara según el nombre del filtro que estemos utilizando. En nuestro caso, el filtro utilizado será la media, entonces en el correspondiente condicional declararemos e inicializaremos los valores a 1/(nxm)

2. Fuera del condicional, realizaremos un proceso muy similar al que hemos hecho en la mediana. Multiplicamos la mascara por los elementos de la matriz de la imagen sobre la cual este centrado y volcaremos sus elementos en el vector. A partir de este punto tenemos la unica diferencia y es que en vez de ordenar los elementos del vector, los estaremos sumando, obteniendo la media. Dicho valor reemplazará al pixel, re-inicializamos los elementos de la mascara a 1/9 y limpiaremos el vector.

Una vez hecho esto, el trabajo estaría hecho. Opcionalmente se tiene la detección de bordes que estaría siendo hecho a través de convolución utilizando el filtro de Sobel, este último basado en la aplicación de la primera derivada. En cualquier cirscuntancia, el siguiente paso sería transformar la matriz que hemos estado transformando en una imagen. Para ello, emplearemos un método de la libreria del profesor que teniendo la matriz de una imagen, genera su imagen indicando su ruta y su nombre.

# Aclaraciones

1. La imagen de partida debe estar en formato BMP.  Este formato facilitará trabajar sobre la imagen porque no aplica sobre la imagen ningun algoritmo de compresión.
    Además, como veremos dentro de la librería del profesor, más allá de tener un método generico de lectura de imagenes de distinto formato, tenemos un método exclusivo para leer imagenes dentro del formato BMP.

2. En ciertos filtros utilizados en la convolución es necesario agregar ceros o algún parecido cuando trabajemos sobre pixeles que hagan que la mascara sobresalga de la matriz de la imagen. En dicha circunstancia, el trabajo hecho en la convolución sería una ampliación de lo antes dicho donde los elementos de la máscara que esten fuera respecto a la matriz de la imagen serán multiplicados por cero u otro valor (p.e. el valor más probable, es decir, el más cercano).

3. Por motivos de eficiencia, se tiene que cuando creemos alguna estructura de datos (vector o matriz), trabajemos sobre ella y queramos volver a trabajar sobre ella, entonces la reutilicemos siempre que sea posible, no volvamos a crear otra estructura de datos del mismo tipo.
