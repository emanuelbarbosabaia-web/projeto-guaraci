#include "FiltroStatus.h"

float Ema(int brutos[], float filtrados[], byte t) {//tamanho
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
  if (vivos > 0)return soma / vivos;
  return 0.0;
}
int status(float m) {
  byte mortos = 0;
  int vivos = -1;
  if (LDRs == 0) return 3;
  
  if (LDRs == 1){
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
  } // todos sensores mortos
  

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

 if ((m < 200.0 && tensaoCarga > 2.5) || (m > 500.0 && potencia_mW < 1.0)) return 5; // Sensores obstruídos, placa suja ou falha física de hardware
 
    tolerancia = (m * 0.15 + 20); 
  return 0;
}