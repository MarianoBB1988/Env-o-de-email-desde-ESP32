# Envío de Correos con ESP32 y Google Apps Script

## Descripción

Este proyecto permite enviar correos electrónicos desde un ESP32 utilizando un **Google Apps Script** como Web App.  
El ESP32 lee por el **Serial Monitor** el destinatario, el asunto y el cuerpo del correo, y envía la información a través del script de Google.

---

## Requisitos

- ESP32
- Conexión WiFi
- Cuenta de Gmail
- Google Apps Script

---

## Configuración del Apps Script

1. Crear un nuevo proyecto en [Google Apps Script](https://script.google.com/).
2. Copiar el código que se encuentra en Script GOOGLE y pegarlo en el proyecto.

## Configuración del ESP32

1.Cargar el contenido el .ino en Arduino IDE.
2. Grabarlo en el ESP32
3. Realizar prueba de envío desde el serial monitor.

Autor:
Mariano Bastarreix
