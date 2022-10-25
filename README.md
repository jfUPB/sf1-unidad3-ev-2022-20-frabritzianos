# Codigo Controlador

Hecho por Andres Ferrer y Juan David Angel

Se declaran las bibliotecas y variables necesarias para el código.


![image](https://user-images.githubusercontent.com/71991263/197092736-65428f58-2f3a-4b32-bcb5-bc02d5b90f22.png)



## Initial setup

Declaramos las variables necesarias para el funcionamiento de las librerias y se calibra el controlador mpu para evitar daños sobre si mismo.

![image](https://user-images.githubusercontent.com/71991263/197093560-76c84493-8eb7-46e8-ba59-a26b340fb051.png)

En esta parte en especifico inicializamos el serial y la velocidad de transmicion de bytes (115200) e inicializamos el acelerometro (mpu6050).

![image](https://user-images.githubusercontent.com/71991263/197093990-166229b0-f28a-4496-ae48-7385505b15d8.png)



## Main Program Loop

En este apartado del codigo está la funcionalidad central del acelerometro donde manda los datos de este a unity usando un metodo de comunicación binario y transformandolo al ASCII en unity

![image](https://user-images.githubusercontent.com/71991263/197095790-8049cff3-5088-4dda-a7f9-2ff24ebb8d8e.png)

Verificamos que el mpu este funcionando y mandando señales.

![image](https://user-images.githubusercontent.com/71991263/197095716-9a69e05b-ecc5-4a9a-bf1e-13dd38a6d955.png)

Recoge los paquetes e información del mpu y permite utilizar esta información de forma de Quaternion para una mejor transferencia de datos.

![image](https://user-images.githubusercontent.com/71991263/197096536-f8a7b9f5-8586-4b50-aa80-cba1707d7013.png)

Se crea un arreglo de 16 datos el cual recibe los datos de w,x,y,z en bytes de 4 osea en binary; por ende el arreglo va aumentando de 4 en 4.
cada uno de los mempcpy ordena los datos para que lleguen de la forma w,x,y,z.

![image](https://user-images.githubusercontent.com/71991263/197097104-4dd6a2db-156b-40c8-9277-9932465c2a01.png)

Ahora no podemos esta enviando datos contantemente ya que esto sobrecargaría el microcontrolador y es una mala practica, por ende tenemos una variables llamada "permiso" que verifica que cada cierto tiempo se envien estos datos, siendo un arreglo de 16 bytes.

![image](https://user-images.githubusercontent.com/71991263/197097690-3b5b64f7-e8b1-41c5-b918-feef0f9101e5.png)


# Código Unity

Estas son las librerias que utilizamos en Unity

![image](https://user-images.githubusercontent.com/71991263/197098026-f3614904-55a9-4e3c-a093-524dc2a702bd.png)

Se inicializan las variables necesarias para la lectura de datos que manda el microcontrolador y el puerto donde se va a recibir y la velocidad de este, 
declaramos un buffer de 16 bytes para recibir los datos del microcontrolador, implementamos un timer para dar permiso a los datos, un intervalo que se encarga de controlar el tiempo de recibimiento de los datos y declaramos las variables del Quaternion qw,qx,qy,qz declarandolas publicas para ver los datos recibidos. 

![image](https://user-images.githubusercontent.com/71991263/197098739-1ba5db87-415a-47a1-afd2-2216514349a2.png)

## void Start

Se envia un enter desde el serialport para que la el comando de permisos "j" funcione
Luego utilizamos serialPort.DtrEnable = true; para que el microcontrolador ejecute los procesos para inicializarse antes de que ejecute el codigo
Y por último se abre el puerto del seria.

![image](https://user-images.githubusercontent.com/71991263/197099367-810a7f2c-a958-4e58-8aba-5face65faa03.png)

## void Update

El serialport lee los bytes que sean mayores o iguales a 16 los cuales llegan desde el buffer, y luego los convierte en un número por medio del comando BiteConverter.ToSingle cada 4 bytes salen estos datos y se colocan en el orden correspondiente a las variables float qw,qx,qy,qz transformandose en un Quaternion.


![image](https://user-images.githubusercontent.com/71991263/197100382-17bdd017-b8b2-4e07-8e22-ac8c3527df94.png)

En este timer le preguntamos al microcontrolador por los datos del acelerometro cada 0.06f por frame y así no saturar el microcontrolador con la pregunta.

![image](https://user-images.githubusercontent.com/71991263/197100803-782f93ed-9f86-4b7e-942e-55163ff03767.png)

Este codigo se implementa en el objeto que se desea transformar.




