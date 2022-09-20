# GPS-NMEA
**Visualización de mensajes de GPS en formato NMEA**

## Objetivo

El objetivo de este T.P. consiste en desarrollar un aplicaivo de consola con comandos en línea de órdenes y escrito en lenguaje ANSI C, que permita interpretar mensajes GPS en fomrato NMEA a partir de un archivo y presentado en formato CSV y [KML](https://developers.google.com/kml/documentation/kmlreference?hl=es1).

## Desarrollo 
Dicha aplicación es capas de generar archivos de salida en formato CSV y KML con la información de localización geográfica de la trayectoria provista a partir de una archivo de texto.


El programa ejecutable fue desarrollado para Unix, y debe ser invocado de la siguiente manera:

```shell
./gpsviewer -fmt <formato> -out <salida> <entrada>
```

### Formato de salida

*\<formato\>*

|  Comando |           Descripcion        |       Valor       |       Tipo de Dato        |
|----------|------------------------------|-------------------|---------------------------|
| - fmt    | Formato del índice a generar | "csv" <br/> "kml" |   char[] <br/> char[]     |

#### CSV
Archivo CSV sin encabezado, campos sin calificador y carácter separado por "|".
> \<Fecha>|\<Latitud>|\<Longitud>|\<Altura sobre el nivel del mar>

El formato de la fecha es el siguiente `AAAAMMDDHHmmSS`. ( Año, Mes, Día, Horas, minutos, Segundos).

Los archivos no contienen Año, Mes y Día, por lo que se utiliza la hora local del sistema para completarlo.

#### KML
Ejemplo. 

```kml
<?xml version="1.0" encoding="UTF-8"?>
<kml xmlns="http://www.opengis.net/kml/2.2">
  <Document>
    <name>Rutas</name>
    <description>Ejemplos de rutas</description>
    <Style id="yellowLineGreenPoly">
      <LineStyle>
        <color>7f00ffff</color>
        <width>4</width>
      </LineStyle>
      <PolyStyle>
         <color>7f00ff00</color>
      </PolyStyle>
    </Style>
    <Placemark>
      <name>Relieve absoluto</name>
      <description>Pared verde transparente con contornos amarillos</description>
      <styleUrl>#yellowLineGreenPoly</styleUrl>
      <LineString>
        <extrude>1</extrude>
        <tessellate>1</tessellate>
        <altitudeMode>absolute</altitudeMode>
         <coordinates> -112.2550785337791,36.07954952145647,2357
-112.2549277039738,36.08117083492122,2357
-112.2552505069063,36.08260761307279,2357
-112.2564540158376,36.08395660588506,2357
-112.2580238976449,36.08511401044813,2357
-112.2595218489022,36.08584355239394,2357
-112.2608216347552,36.08612634548589,2357
-112.262073428656,36.08626019085147,2357
-112.2633204928495,36.08621519860091,2357
-112.2644963846444,36.08627897945274,2357
-112.2656969554589,36.08649599090644,2357 </coordinates>
      </LineString>
    </Placemark>
  </Document>
</kml>
```

Para más información dirigirse a la [documentación oficial](https://developers.google.com/kml/documentation/kml_tut). 

Estos archivos pueden ser procesados por el programa *Google Earth*, lo que permite visualiza la trayectoria. 

### Archivo de salida

*\<salida\>*: Ruta del archivo índice que se desea construir.

### Archivo de entrada

*\<entrada\>*: Ruta del archiov de texto que contiene los mensajes GPS.
  
#### Formato de entrada

  Los archivos de entrada contienen una serie de mensajes GPS en fomrato NMEA. Con el eunciado del trabajo práctivo se adjunta el manual de referencia de un aparato GPS marca SIRF, en donde se describen los distintos mensajes GPS basados en el estándar [NMEA-0183](https://www.nmea.org/content/STANDARDS/NMEA_0183_Standard). Los datos de localización geográfica se deberá obtener de los mensajes de tipo GGA que proveen la información esencial de ubicación sobre el geoide terrestre y la información de laprecisión de la solución del receptor GPS.
  
**Ejemplo de datagrama GGA**
>$GPGGA,002153.000,3342.6618,N,11751.3858,W,1,10,1.2,27.0,M,-34.2,M,,0000*5E

|          Nombre               |  Ejemplo   | Unidad |                 Descripción                  |
|-------------------------------|------------|--------|----------------------------------------------|
|              ID               |   $GPGGA   |        |        Encabezado del protocolo              |
|          Tiempo UTC           |    2153    |        |                  hhmmss.sss                  | 
|            Latitud            | 3342.6618  |        |                  ddmm.mmmm                   |
|          Indicador N/S        |     N      |        |            N (norte) o S (sur)               |
|           Longitud            | 11751.3858 |        |                  ddmm.mmmm                   |
|          Indicador E/W        |     W      |        |            E (este) o W (oeste)              |
|   Indicador de posición fija  |     1      |        |            Ver documentación.                |
|     Satélites utilizados      |    10      |        |               Rango: 0 a 12                  |
|            HDOP               |     1.2    |        |     Dilución de prescisión horizontal        |
| Altura sobre el nivel del mar |    27      | metros |                                              |
|           Unidad              |     M      | metros |                                              |
|     Separación de geoide      |   -34.2    | metros |                                              |
|           Unidad              |     M      | metros |                                              |
|      *Age of Diff. Corr*      |            |   sec  | Campo nulo para receptores no diferenciales  |
|      Suma de verificación     |    *5E     |        |                                              |
|       \<CR\> \<LF\>           |            |        |               Fin de mensaje                 |


La *suma de verificación* es una verificación compuesta de un carácter en formato hexadecimal definido según la especificación NMEA. Consiste de una operación OR de los 7 bits menos significativos de cara carácter después del "$" y antes del "*". El resultado binario de la operación es representado en el equivalente ASCII de dos caracteres hexadeximales. Cada mensajes interpretado deberá ser verificado.

Todos los mensajes de tipos distintos al `GGA` son ignorados. 

El programa ordena por el campo de `Tiempo UTC` para poder reconstruir las trayectorias correctamente.

## Ejemplos de ejecución

### Compilación

Posee un archivo `makefile`, por lo que para la compilación solo basta con ejecutar el siguiente comando en la carpeta de la aplicación.

```shell
make all
```

### Ejecución

Una vez compilado el programa, se puede ejecutar los comandos en el [formato especificado anteriormente](#desarrollo). A continuación unos ejemplos que el usuario mismo puede replicar.

```shell
./gpsviewer -fmt csv -out out/ejemplo.csv trayectoria-A.txt
```

```shell
./gpsviewer -fmt kml -out out/ejemplo.kml trayectoria-B.txt
```

