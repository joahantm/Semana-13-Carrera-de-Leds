
void setup() {
  DDRB |= (1 << PB5);
}

void loop() {

//  PORTB |= (1 << PB5);    // PORTB bit 5 = 1 -> pin en 5 V -> LED encendido
//  delay(500);
//  PORTB &= ~(1 << PB5);   // ~ invierte la mascara (0b11011111) -> bit 5 = 0 -> LED apagado
//  delay(500);


//  PORTB |= (1 << PB5);
//  delay(200);             // Reto 1: 200 ms
//  PORTB &= ~(1 << PB5);
//  delay(200);

  // Reto 2: retardo por bucle (sin delay()).
  volatile obliga al compilador a leer/escribir i en cada vuelta;
  PORTB |= (1 << PB5);
  for (volatile long i = 0; i < 100000; i++);
  PORTB &= ~(1 << PB5);
  for (volatile long i = 0; i < 100000; i++);

}

