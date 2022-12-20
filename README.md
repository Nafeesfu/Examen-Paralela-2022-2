# Examen-Paralela-2022-2
Codigo en C++ usando OpenMp, el codigo leera un archivo.csv con variables dependientes e independientes para ser utilizadas como datos para hacer la prediccion 
de una fecha futura sobre la cantidad de accidentes.
Para ejecutar el codigo es necesario, tener instalada la biblioteca gcc y habilitar el uso de openmp en el compilador.
Las fechas deben ser ingresadas en formato ISO 8601 de la forma YYYY-MM-DD, para fechas como Enero o Febrero, por ejemplo se debe ingresar de la siguiente manera Fecha (2023-02-02), de otra forma, el programa no funcionara y pedira una nueva fecha. Por otro lado el archivo.cvs, debe estar en la carpeta del programa.
Para el desarrollo del codigo, conside las fechas como variables independientes, y el numero de accidentes, como las variables dependientes.
Para trabajar las fechas, tome de referencia un año (1 de Enero de 1970) y con esto se busco llevar las fechas del archivo a un dato que se pudiera usar para la regresion lineal.
Para trabajar el numero de accidentes, solo se tomaron como datos enteros sin ningun cambio en su formato.
El codigo puede dar valores distintos si se ejecuta dos veces con la misma fecha.
