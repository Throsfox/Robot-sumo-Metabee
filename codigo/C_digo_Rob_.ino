#include "AFMotor_R4.h"

// =====================================================
// MOTORES
// =====================================================
AF_DCMotor motorEsq(2);
AF_DCMotor motorDir(1);

// =====================================================
// Pinos do Sensor Ultrassônico
// =====================================================
#define ECHO A0  
#define TRIG A1  

#define IR_Frente A5

// =====================================================
// ESTADOS
// =====================================================
enum Estado {
  ESPERA,
  PROCURA,
  BATALHA
};

Estado estadoAtual = ESPERA;
unsigned long tempoInicial;
unsigned long tempoPerdaVisao = 0; 
bool emPerdaDeVisao = false;       

// =====================================================
// MEDIR DISTÂNCIA
// =====================================================
// =====================================================
// MEDIR DISTÂNCIA (FILTRADA)
// =====================================================
long medirDistancia() {

  long soma = 0;
  int leiturasValidas = 0;

  for (int i = 0; i < 5; i++) {

    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG, LOW);

    long duracao = pulseIn(ECHO, HIGH, 15000);

    if (duracao > 0) {
      long distancia = duracao * 0.034 / 2;

      soma += distancia;
      leiturasValidas++;
    }

    delayMicroseconds(500);
  }

  if (leiturasValidas == 0) {
    return 999;
  }

  return soma / leiturasValidas;
}

// =====================================================
// SETUP
// =====================================================
void setup() {
  Serial.begin(9600);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  
  pinMode(IR_Frente, INPUT); 

  // Garante motores parados no início
  motorEsq.setSpeed(0);
  motorDir.setSpeed(0);
  motorEsq.run(RELEASE);
  motorDir.run(RELEASE);

  tempoInicial = millis();
  Serial.println("MODO ESPERA - 5s");
}

// =====================================================
// LOOP PRINCIPAL
// =====================================================
void loop() {
  long distancia = medirDistancia();
  int borda = digitalRead(IR_Frente);

  // =================================================
  // EVITAR BORDA (Com trava de segurança dos 5s)
  // =================================================
  if (estadoAtual != ESPERA && borda == LOW) {   // ajuste LOW/HIGH após testar
    Serial.println("BORDA DETECTADA!");

    // para
    motorEsq.run(RELEASE);
    motorDir.run(RELEASE);
    delay(100);

    // dá ré (Velocidades originais mantidas)
    motorEsq.setSpeed(80);
    motorDir.setSpeed(80);
    motorEsq.run(BACKWARD);
    motorDir.run(FORWARD);
    delay(400);

    // gira para dentro da arena
    motorEsq.run(BACKWARD);
    motorDir.run(BACKWARD);
    delay(300);

    tempoInicial = millis();
    estadoAtual = PROCURA;

    return;
  }

  Serial.print("Distancia: ");
  Serial.println(distancia);

  // =================================================
  // MÁQUINA DE ESTADOS
  // =================================================
  switch (estadoAtual) {

    case ESPERA:
      motorEsq.run(RELEASE);
      motorDir.run(RELEASE);

      // Espera os 5 segundos obrigatórios antes de iniciar
      if (millis() - tempoInicial >= 5000) {
        Serial.println("KICKSTART: VENCENDO ATRITO INICIAL");

        // Sistema de avanço inicial (Kickstart - Limitado a 150)
        motorEsq.setSpeed(150); 
        motorDir.setSpeed(150);
        motorEsq.run(FORWARD);
        motorDir.run(FORWARD); 
        
        delay(120); // Tempo curto para destravar o motor estático

        estadoAtual = PROCURA;
        Serial.println("INICIANDO PROCURA");
      }
      break;

    case PROCURA:
      // Zera as variáveis de controle de perda de visão ao entrar na busca
      emPerdaDeVisao = false; 

      // Patrulha com proporções mantidas (50/55)
      motorEsq.setSpeed(55); 
      motorDir.setSpeed(60);
      
      motorEsq.run(FORWARD);
      motorDir.run(FORWARD); 

      if (distancia > 0 && distancia < 40) {
        Serial.println("ALVO ENCONTRADO -> APLICANDO FREIO");

        // Rotina de Freio Anti-Inércia
        motorEsq.setSpeed(55);
        motorDir.setSpeed(60);
        motorEsq.run(BACKWARD);
        motorDir.run(BACKWARD);
        delay(150); 

        estadoAtual = BATALHA;
        Serial.println("MODO BATALHA -> INDO COM TUDO");
      }
      break;

    case BATALHA:
      
      // =========================================================
      // ETAPA 1: APROXIMAÇÃO (Antes de tocar)
      // =========================================================
      if (distancia > 4 && distancia <= 40) {
        //Serial.println("BATALHA: APROXIMANDO...");
        // Proporções mantidas (85/80)
        motorEsq.setSpeed(90); 
        motorDir.setSpeed(85);
        motorEsq.run(FORWARD);
        motorDir.run(BACKWARD); 
      } 
      // =========================================================
      // ETAPA 2: CONTATO (Tocando no oponente)
      // =========================================================
      else if (distancia > 0 && distancia <= 8) {
        //Serial.println("BATALHA: CONTATO! EMPURRANDO MAX...");
        // Força "MÁXIMA" permitida pelo seu limite (150/150)
        motorEsq.setSpeed(150); 
        motorDir.setSpeed(150);
        motorEsq.run(FORWARD);
        motorDir.run(BACKWARD);
      }

      // =========================================================
      // CONTROLE DE PERDA DE VISÃO (Tolerância de 500ms)
      // =========================================================
      if (distancia > 40 || distancia == 999 || distancia <= 0) {
        
        // Se acabou de perder o contato, inicia a contagem de meio segundo
        if (!emPerdaDeVisao) {
          tempoPerdaVisao = millis();
          emPerdaDeVisao = true;
          Serial.println("ALVO SUMIU: MANTENDO AVANÇO POR 500MS...");
        }
        
        // Se já se passaram 500ms sem o alvo voltar, desiste e procura
        if (millis() - tempoPerdaVisao >= 500) {
          estadoAtual = PROCURA;
          Serial.println("TEMPO ESGOTADO -> VOLTANDO PARA PROCURA");
        }
        
      } else {
        // Se o robô recuperou a visão do oponente, zera o temporizador
        if (emPerdaDeVisao) {
          emPerdaDeVisao = false;
          Serial.println("ALVO RECUPERADO -> MANTENDO BATALHA");
        }
      }
      break;
  }
  
  delay(30); 
}
