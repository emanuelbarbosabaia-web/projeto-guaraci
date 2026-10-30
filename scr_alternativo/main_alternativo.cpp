#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 meuINA219;

const byte LDRs = 2;
const byte pinoLDR[LDRs] = {A0, A1}; 
int valorBrutoLDR[LDRs] = {0, 0}; 
float valorFiltradoLDR[LDRs] = {0.0, 0.0}; 
const float alpha = 0.3; 
float tolerancia = 20;
float mediaLDRs = 0.0;
float tensaoShunt = 0, tensaoBarramento = 0, corrente_mA = 0, tensaoCarga = 0, potencia_mW = 0;

float Ema(int brutos[], float filtrados[], byte t);
int statusSistema(float m);

void setup() {
  Serial.begin(9600); 
  Serial.print("Início do programa\n");

  if (!meuINA219.begin()) {
    Serial.print("Falha ao encontrar o INA219\n");
    while (1) delay(10); 
  }
}

void loop() {
  tensaoShunt = meuINA219.getShuntVoltage_mV();
  tensaoBarramento = meuINA219.getBusVoltage_V();
  corrente_mA = meuINA219.getCurrent_mA();
  potencia_mW = meuINA219.getPower_mW();
  tensaoCarga = tensaoBarramento + (tensaoShunt / 1000);

  for (int i = 0; i < LDRs; i++) {
    valorBrutoLDR[i] = analogRead(pinoLDR[i]);
  }
  
  mediaLDRs = Ema(valorBrutoLDR, valorFiltradoLDR, LDRs);

  // Saída em JSON
  Serial.print("{");
  Serial.print("\"ldr_media\":"); Serial.print(mediaLDRs); Serial.print(",");
  Serial.print("\"tensao_V\":"); Serial.print(tensaoCarga); Serial.print(",");
  Serial.print("\"corrente_mA\":"); Serial.print(corrente_mA); Serial.print(",");
  Serial.print("\"potencia_mW\":"); Serial.print(potencia_mW); Serial.print(",");
  Serial.print("\"status\":"); Serial.print(statusSistema(mediaLDRs));
  Serial.print("}\n");

  delay(2000); 
}

float Ema(int brutos[], float filtrados[], byte t) {
  if (t == 0) return 0.0;

  if (t == 1){ 
    filtrados[0] = (brutos[0] * alpha) + (filtrados[0] * (1 - alpha));
    return filtrados[0];
  }

  float soma = 0.0;
  byte vivos = 0;
  for (byte i = 0; i < t; i++) {
    if (brutos[i] > 0 && brutos[i] < 1023) {
      filtrados[i] = (brutos[i] * alpha) + (filtrados[i] * (1 - alpha));
      soma += filtrados[i];
      vivos++;
    }
  }
  if (vivos > 0) return soma / vivos;
  return 0.0;
}

int statusSistema(float m) {
  byte mortos = 0;
  int vivos = -1;
  
  if (LDRs == 0) return 3;
  
  if (LDRs == 1) {
    if (m == 0 || m == 1023) return 3;  
    if (m > tolerancia) return 2;
    if (m < 200.0 && tensaoCarga < 1.5) {
      tolerancia = (m * 0.15 + 20); 
      return 1; 
    }
    if (potencia_mW < 5.0) return 4; 
    if ((m < 200.0 && tensaoCarga > 2.5) || (m > 500.0 && potencia_mW < 1.0)) return 5;
    tolerancia = (m * 0.15 + 20); 
    return 0;
  }

  for (int i = 0; i < LDRs; i++) {
    if (valorBrutoLDR[i] == 0 || valorBrutoLDR[i] == 1023) {
      mortos++;
    } else {
      vivos = i; 
    }
  }
  
  if (mortos == LDRs) { 
    return 3; 
  }

  if (mortos == 1) {
    tolerancia = (valorFiltradoLDR[vivos] * 0.15 + 20); 
    return 2; 
  }

  float maior = valorFiltradoLDR[0], menor = valorFiltradoLDR[0];
  for (int i = 1; i < LDRs; i++) {
    if (valorFiltradoLDR[i] > maior) maior = valorFiltradoLDR[i];
    if (valorFiltradoLDR[i] < menor) menor = valorFiltradoLDR[i];
  }

  if (abs(maior - menor) > tolerancia) {
    return 2; 
  }

  if (m < 200.0 && tensaoCarga < 1.5) {
    tolerancia = (m * 0.15 + 20); 
    return 1; 
  }
  
  if (potencia_mW < 5.0) return 4;

  if ((m < 200.0 && tensaoCarga > 2.5) || (m > 500.0 && potencia_mW < 1.0)) {
    return 5; 
  }
  
  tolerancia = (m * 0.15 + 20); 
  return 0;
}