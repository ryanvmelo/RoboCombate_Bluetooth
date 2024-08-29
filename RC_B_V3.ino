#include <Servo.h>

#define p1m1 4
#define p2m1 5
#define p1m2 7
#define p2m2 8
#define pin_esc 12  // Pino PWM para o sinal do ESC

int vel;
Servo esc;

class Motor {
public:
  int p1, p2;
  Motor(int p1, int p2) {
    this->p1 = p1;
    this->p2 = p2;
    pinMode(p1, OUTPUT);
    pinMode(p2, OUTPUT);
    digitalWrite(p1, LOW);
    digitalWrite(p2, LOW);
  }
  void frente(int v) {
    digitalWrite(p1, HIGH);
    digitalWrite(p2, LOW);
  }
  void tras(int v) {
    digitalWrite(p1, LOW);
    digitalWrite(p2, HIGH);  
  }
  void para() {
    digitalWrite(p1, LOW);
    digitalWrite(p2, LOW);  
  }
  void freia() {
    digitalWrite(p1, HIGH);
    digitalWrite(p2, HIGH);  
  }
};

Motor *md = new Motor(p1m1, p2m1);
Motor *me = new Motor(p1m2, p2m2);

void setup() {
  Serial.begin(9600);
  vel = 255;

  // Inicialização do ESC
  esc.attach(pin_esc);

  // Calibração do ESC (pode variar de acordo com o fabricante)
  esc.writeMicroseconds(1000);  // Pulso mínimo (motor parado)
  delay(2000);  // Aguarde 2 segundos
  esc.writeMicroseconds(2000);  // Pulso máximo (motor na velocidade máxima)
  delay(2000);  // Aguarde 2 segundos
  esc.writeMicroseconds(1500);  // Pulso neutro (ponto médio)
  delay(2000);  // Aguarde 2 segundos para garantir que o ESC está calibrado

  // Inicialização dos motores de corrente contínua
  md->para();
  me->para();
}

void loop() {
  char c = Serial.read();
  if (c == 'F') {
    md->frente(vel);
    me->frente(vel);
  }
  if (c == 'T') {
    md->tras(vel);
    me->tras(vel);
  }
  if (c == 'E') {
    md->tras(vel);
    me->frente(vel);
  }
  if (c == 'D') {
    md->frente(vel);
    me->tras(vel);
  }
  if (c == 'P') {
    md->para();
    me->para();
  }

  // Controle do motor brushless via ESC
  if (c == 'A') {  // Exemplo: 'A' para acelerar o motor brushless
    esc.writeMicroseconds(1600);  // Envie um sinal PWM para aumentar a velocidade
  }
  if (c == 'S') {  // Exemplo: 'S' para parar o motor brushless
    esc.writeMicroseconds(1500);  // Envie um sinal PWM neutro para parar o motor
  }

  delay(100);  // Pequena pausa para estabilidade do loop
}
