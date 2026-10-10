#ifndef FILTRO_STATUS_H
#define FILTRO_STATUS_H

#include <Arduino.h>

extern const byte LDRs;
extern const byte pinoLDR[];
extern int valorBrutoLDR[];
extern float valorFiltradoLDR[];
extern const float alpha;
extern float tolerancia;
extern float tensaoCarga;
extern float potencia_mW;

float Ema(int brutos[], float filtrados[], byte t);
int status(float m);

#endif
