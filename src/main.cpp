#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include <FiltroStatus.h> 

Adafruit_INA219 meuINA219;

const byte LDRs = 2;
const byte pinoLDR[LDRs] = {A0, A1}; 
int valorBrutoLDR[LDRs] = {0, 0}; 
float valorFiltradoLDR[LDRs] = {0.0, 0.0}; 
const float alpha = 0.3; 
float tolerancia = 20;
float mediaLDRs = 0.0;
float tensaoShunt = 0, tensaoBarramento = 0, corrente_mA = 0, tensaoCarga = 0, potencia_mW = 0;

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

  //JSON
  Serial.print("{");
  Serial.print("\"ldr_media\":"); Serial.print(mediaLDRs); Serial.print(",");
  Serial.print("\"tensao_V\":"); Serial.print(tensaoCarga); Serial.print(",");
  Serial.print("\"corrente_mA\":"); Serial.print(corrente_mA); Serial.print(",");
  Serial.print("\"potencia_mW\":"); Serial.print(potencia_mW); Serial.print(",");
  Serial.print("\"status\":"); Serial.print(status(mediaLDRs));
  Serial.print("}\n");

  delay(2000); 
}
