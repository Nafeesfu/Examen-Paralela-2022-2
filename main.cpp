#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <time.h>
#include <cstring>
#include <cmath>
#include <omp.h> //Libreria necesaria para paralelizar
#include <stdio.h>
#include <ctime>
#include <stdlib.h>
#include <Windows.h>//Libreria para el tiempo unix, de momento no se usa

using namespace std;



/*void calcularFecha(float numeroFecha) {
    time_t seconds = numeroFecha;
    struct tm *timeinfo = localtime(&seconds);
    SYSTEMTIME st;
    st.wYear = timeinfo->tm_year + 1900;
    st.wMonth = timeinfo->tm_mon + 1;
    st.wDay = timeinfo->tm_mday;
    st.wHour = timeinfo->tm_hour;
    st.wMinute = timeinfo->tm_min;
    st.wSecond = timeinfo->tm_sec;
    st.wMilliseconds = 0;

    WCHAR buffer[80];
    GetDateFormatEx(LOCALE_NAME_USER_DEFAULT, DATE_SHORTDATE, &st, L"dd/MM/yyyy HH:mm:ss", buffer, 80, NULL);
    std::wcout << buffer << std::endl;
}*/ //Esta funcion utilza unix para trabajar con las fechas, pero no hay forma de que funcione bien con el codigo, mas abajo esta la funcion que sirve

double fecha_a_segundos(char* anio, char* mes, char* dia) {
  // Crear una estrcutura tm con la fecha
  tm fecha;
  fecha.tm_year = atoi(anio) - 1900;;
  fecha.tm_mon = atoi(mes) - 1;
  fecha.tm_mday = atoi(dia);

  // Convertir el objeto tm a un objeto time_t
  time_t t = mktime(&fecha);

  // Obtener el número de segundos transcurridos desde la madrugada del 1 de enero de 1970
  return t;
}

int main()
{
    // Establece el número de hilos a usar
    ifstream Archivo;

    Archivo.open("datos_examen.csv");

    string linea = "";

    float aux = 0;
    float sumA = 0;
    float sumY = 0;
    float sumF = 0;
    float sumxY = 0;
    float sumXX = 0;
    float sec;

    getline(Archivo,linea);// salta la primera línea
   
    omp_set_num_threads(4); //establece el numero de hilos en 4

    #pragma omp parallel //for reduction (+:sumA,sumxY,sumXX,sumY)
//Lo ideal hubiese sido utilizar esta funcion para paralelizar pero hay que cambiar el for
    for(string linea; getline(Archivo,linea);){
        string fecha;
        int vdependiente; //variable dependiente corresponde a los accidentes

        string tempString = "";

        stringstream inputString(linea);

        getline(inputString, fecha, ';');
        //Dividimos la fecha en componentes utilizando strtok()
        char* date_str = strdup(fecha.c_str());
        char* day_str = strtok(date_str, "-");
        char* month_str = strtok(NULL, "-");
        char* year_str = strtok(NULL, "-");


        sec =  sec = fecha_a_segundos(year_str , month_str, day_str);

        sumF = sumF + sec;


        getline(inputString,tempString,';');
        vdependiente = atoi(tempString.c_str());


        aux = aux + 1;//contador de datos debe dar 1892
        //cout << segundos <<endl;Mensaje de prueba
        sumA = sumA + vdependiente;
        sumxY = sumxY +(sec * vdependiente);
        sumXX = sumXX + (sec*sec);
        sumY = sumY + (vdependiente*vdependiente);
        linea = "";
}
    float promx = sumA/aux;
    float promeFecha = sumF/aux;
    float nxy = aux * promeFecha * promx;
    float nx2 = aux* promeFecha * promeFecha;
    float pendiente = (sumxY - nxy)/(sumXX - nx2);
    float y = promx - pendiente * promeFecha;
    float sumatoriaYCuadrado = sumY;

    

    string input; //input corresponde a los datos ingresados por el usuario
    while (true){ //Este ciclo es para ingresar la fecha y validarla, si se ingresa una fecha con el mes 13 por ejemplo, pedira una nueva fecha
    cout << "Ingrese una fecha en formato ISO 8601 (YYYY-MM-DD): " << endl;
    cin >> input;

    // Verifica si la cadena de caracteres tiene el formato correcto
    if (input.size() != 10 || input[4] != '-' || input[7] != '-') {
      cout << "Por favor, ingrese una fecha valida" << endl;
      continue;
    }

    // Verifica si los componentes de la fecha son números válidos
    int year, month, day;
    if (!(istringstream(input.substr(0, 4)) >> year) ||
        !(istringstream(input.substr(5, 2)) >> month) ||
        !(istringstream(input.substr(8, 2)) >> day)) {
      cout << "Por favor, ingrese una fecha valida" << endl;
      continue;
    }
        if (month < 1 || month > 12) {
      cout << "Por favor, ingrese una fecha valida" << endl;
      continue;
        }
    // Crea una estructura tm con los valores de la fecha
    struct tm date = { 0 };
    date.tm_year = year - 1900;
    date.tm_mon = month - 1;
    date.tm_mday = day;

    // Convierte la fecha a un valor de tiempo
    time_t t = mktime(&date);

    if (t != -1) {
      // La fecha es válida
      break;
    }else {
      // La fecha es inválida
      cout << "Por favor, ingrese una fecha valida" << endl;
        //Aqui termina la validacion
    }
    }
    char* date_str = strdup(input.c_str());
    char* year_str = strtok(date_str, "-");
    char* month_str = strtok(NULL, "-");
    char* day_str = strtok(NULL, "-");
    //Convertimos los componentes a números y asignamos a la estructura tm
    tm t;
    t.tm_year = atoi(year_str) - 1900;
    t.tm_mon = atoi(month_str) - 1;
    t.tm_mday = atoi(day_str);

        // Convertimos la estructura tm a un valor de tiempo en segundos
    time_t seg = mktime(&t);
    float prediccion = y + (pendiente * sec);
    
    cout << "La prediccion de accidentes para la fecha " << input << " es: " << prediccion << endl;

    return 0;
}
