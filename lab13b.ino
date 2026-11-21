void setup() {
 asm volatile ("sbi 0x04, 5");
}

void loop() {


 //  asm volatile ("sbi 0x05, 5");   // PORTB bit 5 = 1 -> LED encendido
 // delay(500);
 // asm volatile ("cbi 0x05, 5");   // cbi = "Clear Bit in I/O": PORTB bit 5 = 0 -> apagado
 // delay(500);


 // out escribe el BYTE COMPLETO en PORTB. No acepta un numero directo,
 // por eso el valor primero se carga en un registro ("r").
 // uint8_t valor = 0b00100000;     // solo PB5 = 1
 // asm volatile ("out %0, %1" : : "I" (0x05), "r" (valor));
 // delay(500);
 // valor = 0;                       // todos los bits en 0
 // asm volatile ("out %0, %1" : : "I" (0x05), "r" (valor));
 // delay(500);


  // Bono: parpadeo y retardo 100 % en ensamblador.
  asm volatile (
    "    sbi  0x05, 5          \n\t"  // LED encendido
    "    rcall 9f              \n\t"  // llamar subrutina de retardo (500 ms)
    "    cbi  0x05, 5          \n\t"  // LED apagado
    "    rcall 9f              \n\t"  // retardo (500 ms)
    "    rjmp 8f               \n\t"  // saltar la subrutina y salir

    // ---- Subrutina de retardo: 500 ms 
    "9:  ldi  r24, lo8(500)    \n\t"  // r25:r24 = 500 (contador de ms)
    "    ldi  r25, hi8(500)    \n\t"
    "1:  ldi  r26, lo8(3999)   \n\t"  // r27:r26 = 3999 (vueltas por 1 ms)
    "    ldi  r27, hi8(3999)   \n\t"
    "2:  sbiw r26, 1           \n\t"  // resta 1 al par r27:r26
    "    brne 2b               \n\t"  // si no es 0, repetir (1 ms)
    "    sbiw r24, 1           \n\t"  // un milisegundo menos
    "    brne 1b               \n\t"  // si faltan ms, repetir
    "    ret                   \n\t"  // regresar de la subrutina
    "8:                        \n\t"
    :
    :
    : "r24", "r25", "r26", "r27"     // registros que usamos 
  );
#endif
}


