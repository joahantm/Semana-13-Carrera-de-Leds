# Semana-13-Carrera-de-Leds
Contenido del repositorio
Semana13/ ├── lab13a/lab13a.ino → Configuración de registros (C puro) ├── lab13b/lab13b.ino → ASM inline AVR (sbi, cbi, out) ├── lab13c/lab13c.ino → Carrera de LEDs (registros + ASM inline) └── README.md

Cada sketch tiene los codigos comentados asi solo subimos el que nos interese para ver el funcionamiento

Lab 13a — Configuración de registros (C puro)
Parpadeo del LED integrado (pin 13 = PB5) usando directamente los registros DDRB y PORTB, sin pinMode ni digitalWrite.

| Descripción |

| 0 | Código base: parpadeo cada 500 ms | | 1 | Reto 1: parpadeo cada 200 ms | | 2 | Reto 2: parpadeo sin delay(), con bucle for y variable volatile |

Conceptos

DDRB |= (1 << PB5) configura PB5 como salida.
PORTB |= (1 << PB5) pone el pin en alto (LED encendido); PORTB &= ~(1 << PB5) lo pone en bajo.
DDRB debe configurarse antes que PORTB: si el pin sigue como entrada, escribir 1 en PORTB solo activa la resistencia pull-up interna.
volatile evita que el compilador elimine el bucle de retardo por considerarlo inútil.
Lab 13b — ASM inline AVR
Mismo parpadeo del pin 13, ahora controlado con instrucciones de ensamblador AVR.

| Descripción |

| 0 | Código base: encendido/apagado con sbi / cbi | | 1 | Reto 1: encendido/apagado con out | | 2 | Reto 2 (bono): parpadeo y retardo 100 % en ASM (sin delay()) |

Conceptos

Direcciones de I/O: PINB = 0x03, DDRB = 0x04, PORTB = 0x05.
sbi 0x05, 5 pone en 1 solo el bit 5 de PORTB; cbi 0x05, 5 lo pone en 0.
out escribe el byte completo del puerto y necesita el valor en un registro.
¿sbi u out? out tarda 1 ciclo pero requiere un ldi previo (2 ciclos en total, igual que sbi). Para cambiar un solo bit conviene sbi/cbi (no altera otros pines ni usa registros); para cambiar varios bits a la vez, out es más eficiente.
El retardo en ASM es una subrutina con dos bucles anidados (sbiw / brne): el interno dura ≈ 1 ms a 16 MHz (3999 vueltas × 4 ciclos) y el externo lo repite N veces.
Lab 13c — Carrera de LEDs
Secuencia de 4 LEDs externos conectados en los pines 8–11 (PB0–PB3).

LED	Pin	Bit
LED 1	8	PB0
LED 2	9	PB1
LED 3	10	PB2
LED 4	11	PB3
Cada LED: ánodo al pin, cátodo → resistencia de 220 Ω → GND.

| Descripción |

| 0 | Código base en C puro (PORTB = (1 << i)) | | 1 | Reto 1: el for escribe el puerto con out (ASM inline) | | 2 | Reto 2 + Reto 3 (bono): ida y vuelta LED 1→4→1, todo en ASM, incluido el retardo (sin delay()) |

Conceptos clave

in / ori / out sobre DDRB configuran PB0–PB3 como salidas.
lsl / lsr desplazan el patrón para avanzar o regresar de LED; cpi + breq/brne detectan los extremos.
out 0x05, r18 copia el registro r18 al puerto B; necesita un registro porque la instrucción out no acepta un valor inmediato.
Video
Link donde se encuentran los videos de los resultados divididos por carpetas

(https://drive.google.com/drive/folders/1VKNhdraiv7_pj9wBOLGbvb6UOopU4ifE?usp=sharing)
