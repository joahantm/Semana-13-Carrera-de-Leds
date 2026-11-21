# Semana 13: Carrera de LEDs (Registros AVR y ASM Inline)



Prácticas de programación a bajo nivel en microcontroladores AVR (Arduino) manipulando registros directamente en **C puro** y **Ensamblador (ASM) inline**, eliminando la abstracción de funciones estándar como `pinMode()`, `digitalWrite()` y `delay()`.



> **Nota de uso:** Cada archivo `.ino` contiene las distintas variantes y retos comentados internamente. Descomente únicamente la sección que desee probar antes de compilar y cargar el programa en la placa.



---



## Estructura del Repositorio



```text

Semana13/

├── lab13a/

│   └── lab13a.ino    # Configuración de registros (C puro)

├── lab13b/

│   └── lab13b.ino    # ASM inline AVR (sbi, cbi, out)

├── lab13c/

│   └── lab13c.ino    # Carrera de LEDs (registros + ASM inline)

└── README.md         # Documentación del proyecto

```



---



## Prácticas y Retos



### 1. Lab 13a — Configuración de Registros (C puro)

Control de parpadeo del LED integrado en placa (**Pin 13 / `PB5`**) manipulando directamente los registros `DDRB` y `PORTB`.



| Variante | Descripción |

| :---: | :--- |

| **`0`** | **Código base:** Parpadeo estándar cada `500 ms`. |

| **`1`** | **Reto 1:** Parpadeo rápido cada `200 ms`. |

| **`2`** | **Reto 2:** Parpadeo sin `delay()`, implementando un bucle `for` y una variable `volatile`. |



#### Conceptos clave

* **`DDRB |= (1 << PB5)`**: Configura el bit 5 del puerto B como salida.

* **`PORTB |= (1 << PB5)`** / **`PORTB &= ~(1 << PB5)`**: Enciende (`HIGH`) y apaga (`LOW`) el LED mediante operaciones a nivel de bits.

* **Orden de inicialización**: `DDRB` debe configurarse antes que `PORTB`; de lo contrario, escribir un `1` en un pin configurado como entrada solo activa su resistencia *pull-up* interna.

* **Calificador `volatile`**: Evita que el compilador optimice y elimine el bucle vacío de retardo al considerarlo código muerto.



---



### 2. Lab 13b — ASM Inline AVR

Misma funcionalidad de parpadeo en el **Pin 13**, controlada mediante instrucciones de ensamblador incrustadas en C.



| Variante | Descripción |

| :---: | :--- |

| **`0`** | **Código base:** Encendido y apagado utilizando `sbi` / `cbi`. |

| **`1`** | **Reto 1:** Encendido y apagado escribiendo el puerto completo con `out`. |

| **`2`** | **Reto 2 (Bono):** Parpadeo y subrutina de retardo 100 % en ASM (sin `delay()`). |



#### Conceptos clave

* **Direcciones de I/O (Puerto B):** `PINB = 0x03`, `DDRB = 0x04`, `PORTB = 0x05`.

* **Manipulación de bit único:** `sbi 0x05, 5` pone en `1` únicamente el bit 5 de `PORTB`; `cbi 0x05, 5` lo limpia a `0`.

* **¿`sbi` u `out`?** `out` ejecuta en 1 ciclo de reloj, pero requiere cargar previamente el valor en un registro con `ldi` (sumando 2 ciclos en total, igual que `sbi`). Para un solo bit conviene `sbi`/`cbi` (no altera otros pines ni ocupa registros); para modificar múltiples pines simultáneamente, `out` es más eficiente.

* **Retardo en ASM:** Subrutina de dos bucles anidados (`sbiw` / `brne`). El bucle interno consume $\approx 1\text{ ms}$ a $16\text{ MHz}$ ($3999\text{ iteraciones} \times 4\text{ ciclos}$) y el externo lo repite $N$ veces.



---



### 3. Lab 13c — Carrera de LEDs

Secuencia luminosa bidireccional utilizando 4 LEDs externos conectados a los pines digitales **8 al 11** (`PB0`–`PB3`).



#### Esquema de Conexión

*(Ánodo al pin correspondiente $\rightarrow$ Cátodo a resistencia de $220\ \Omega$ $\rightarrow$ `GND`)*



| Componente | Pin Arduino | Bit AVR |

| :---: | :---: | :---: |

| **LED 1** | Pin `8` | `PB0` |

| **LED 2** | Pin `9` | `PB1` |

| **LED 3** | Pin `10` | `PB2` |

| **LED 4** | Pin `11` | `PB3` |



#### Variantes implementadas



| Variante | Descripción |

| :---: | :--- |

| **`0`** | **Código base:** Secuencia en C puro mediante desplazamiento (`PORTB = (1 << i)`). |

| **`1`** | **Reto 1:** Bucle `for` en C que escribe el estado del puerto usando `out` en ASM inline. |

| **`2`** | **Reto 2 + 3 (Bono):** Efecto ida y vuelta (`LED 1 → 4 → 1`) 100 % en ASM, incluyendo el retardo sin `delay()`. |



#### Conceptos clave

* **Configuración de pines:** Combinación de `in` / `ori` / `out` sobre `DDRB` para habilitar `PB0`–`PB3` como salidas sin alterar los bits restantes del puerto.

* **Desplazamiento y límites:** Las instrucciones `lsl` (desplazamiento lógico a la izquierda) y `lsr` (a la derecha) mueven el LED activo; `cpi` junto con `breq`/`brne` detectan los extremos de la carrera para invertir el sentido.

* **Transferencia al puerto:** `out 0x05, r18` copia el contenido del registro `r18` hacia `PORTB` (es obligatorio usar un registro intermedio, ya que `out` no admite valores inmediatos).



---



## Evidencia en Video



Los videos demostrativos de cada laboratorio y reto en funcionamiento se encuentran organizados por carpetas en el siguiente enlace:



* **[Repositorio de videos en Google Drive](https://drive.google.com/drive/folders/1VKNhdraiv7_pj9wBOLGbvb6UOopU4ifE?usp=sharing)**
Video
Link donde se encuentran los videos de los resultados divididos por carpetas

(https://drive.google.com/drive/folders/1VKNhdraiv7_pj9wBOLGbvb6UOopU4ifE?usp=sharing)
