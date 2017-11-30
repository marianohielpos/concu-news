## Concu-news

*Segundo Trabajo práctico de Técnicas de Programación Concurrente I (75.59)*

Este trabajo consiste en una simulación de un simple servidor con dos
microservicios, y clientes que consultan y actualizan la información de dichos
microservicios.


### Compilar y correr

Se compila vía:

```bash
cmake .
make
```

El servidor se corre con:

```bash
./bin/server
```

El cliente se corre con:

```bash
./bin/cliente
```

### Opciones de linea de comando

#### Cliente

```
Uso:

cliente [Opciones]

Flags:
-a      Para hacer una actualización como admin
-c/-m   Para hacer consulta/actualización sobre ciudad/moneda

Opciones:
-l      Archivo de log (default: ninguno, se escribe a stdout)
-n      Nivel de log (default: info)

-k      La clave a consultar/setear.
        Por ejemplo, si el modo es -c (ciudad), la clave puede ser "Londres"
-v      El valor a guardar en la clave especificada.
        Solo permitido en consulta de admin.

Si se elige hacer una actualización de los datos de una ciudad como admin, se
pueden definir valores numéricos de temperatura, presión y humedad por separado:

-t      Temperatura en grados Celsius.
-p      Presión atmosférica en mm.
-u      Humedad en porcientos.
```


#### Servidor

```
Uso:

server [Opciones]

Opciones:
-l      Archivo de log (default: ninguno, se escribe a stdout)
-n      Nivel de log (default: info)
```
