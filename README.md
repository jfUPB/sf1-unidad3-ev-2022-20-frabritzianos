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

