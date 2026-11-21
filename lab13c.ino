void setup() {
 
  //DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB3);  // PB0-PB3 salidas Esta instruccion se utiliza solo cuando se intenta compilar el reto 2
  asm volatile (
    "in   r24, 0x04   \n\t"   // r24 = DDRB
    "ori  r24, 0x0F   \n\t"   // r24 |= 0b00001111  (PB0-PB3)
    "out  0x04, r24   \n\t"   // DDRB = r24
    : : : "r24"
  );
}

void loop() {
//Codigo dado por el profesor
//  for (int i = 0; i < 4; i++) {
//    PORTB = (1 << i);         // Enciende solo el LED i (1, 2, 4, 8)
//    delay(200);
//  }

// Codigo del primer reto
//  for (uint8_t i = 0; i < 4; i++) {
//    uint8_t valor = (1 << i);                                  // 0b0001, 0b0010, 0b0100, 0b1000
//    asm volatile ("out %0, %1" : : "I" (0x05), "r" (valor));   // PORTB = valor
//    delay(200);
//  }

// Codigo del segundo reto
  // Secuencia: LED1 -> LED2 -> LED3 -> LED4 -> LED3 -> LED2 -> (repite desde LED1)
  // r18 guarda el patron del LED encendido.
  asm volatile (
    "    ldi  r18, 0x01        \n\t"  // patron inicial: LED1 (PB0)

    // ---------- IDA: 0x01 -> 0x02 -> 0x04 -> 0x08 ----------
    "1:  out  0x05, r18        \n\t"  // PORTB = r18 (enciende un LED)
    "    rcall 9f              \n\t"  // retardo 200 ms
    "    cpi  r18, 0x08        \n\t"  // llegamos al LED4?
    "    breq 3f               \n\t"  //   si -> empezar la vuelta
    "    lsl  r18              \n\t"  // desplazar a la izquierda (siguiente LED)
    "    rjmp 1b               \n\t"

    // ---------- VUELTA: 0x04 -> 0x02 ----------
    "3:  lsr  r18              \n\t"  // desplazar a la derecha (LED anterior)
    "    out  0x05, r18        \n\t"
    "    rcall 9f              \n\t"  // retardo 200 ms
    "    cpi  r18, 0x02        \n\t"  // llegamos al LED2?
    "    brne 3b               \n\t"  //   no -> seguir regresando
    "    rjmp 8f               \n\t"  // si -> salir (loop() vuelve a empezar en LED1)

    // ---------- Subrutina de retardo: 200 ms 
    "9:  ldi  r24, lo8(200)    \n\t"
    "    ldi  r25, hi8(200)    \n\t"
    "4:  ldi  r26, lo8(3999)   \n\t"
    "    ldi  r27, hi8(3999)   \n\t"
    "5:  sbiw r26, 1           \n\t"
    "    brne 5b               \n\t"
    "    sbiw r24, 1           \n\t"
    "    brne 4b               \n\t"
    "    ret                   \n\t"
    "8:                        \n\t"
    :
    :
    : "r18", "r24", "r25", "r26", "r27"
  );

}


