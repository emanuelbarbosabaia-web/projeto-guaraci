# Projeto Guaraci — Mapeamento e Extensão do Código

Este documento apresenta a aplicação do firmware e serve como referência para modificar, calibrar ou expandir o sistema, mantendo sua estrutura modular.

## 1. Aplicação do projeto

O firmware foi desenvolvido para monitoramento de eficiência energética e rastreamento solar (*solar tracking*). Ele coleta dados de dois grupos de sensores:

1. **Luminosidade:** dois sensores LDR posicionados para comparar a incidência de luz.
2. **Grandezas elétricas:** um sensor INA219, utilizado para medir tensão e corrente e obter a potência elétrica da carga.

Os dados são processados localmente e enviados pela interface Serial em formato JSON. Isso permite integrá-los a ferramentas como dashboards, Node-RED e scripts em Python.

## 2. Estrutura do código

O código utiliza variáveis de configuração e bibliotecas de apoio para facilitar a manutenção.

- **`FiltroStatus.h`:** reúne a função `Ema()`, que aplica um filtro de média móvel exponencial às leituras dos LDRs, e a função `status()`, que classifica o estado do sistema.
- **Arrays de sensores:** os LDRs são percorridos por um laço `for`, facilitando a alteração da quantidade de sensores e dos pinos utilizados.

## 3. Como modificar o sistema

Os principais ajustes são feitos no início do arquivo principal.

### 3.1. Sensibilidade do filtro dos LDRs

O parâmetro `alpha` controla a suavização das leituras:

```cpp
const float alpha = 0.3;
```

Valores menores produzem maior suavização e uma resposta mais lenta; valores maiores tornam a resposta mais rápida.

### 3.2. Tolerância de calibração

O parâmetro `tolerancia` define a margem usada pela lógica de classificação do sistema:

```cpp
float tolerancia = 20;
```

Esse valor pode ser ajustado conforme a calibração dos LDRs e a faixa de atuação desejada para o status.

### 3.3. Expansão do número de sensores LDR

Para adicionar sensores, atualize o tamanho do array e informe os novos pinos analógicos. Por exemplo, para utilizar quatro LDRs:

```cpp
const byte LDRs = 4;
const byte pinoLDR[LDRs] = {A0, A1, A2, A3};
```

Os trechos que percorrem o array com `for` podem acompanhar a alteração automaticamente. Ainda assim, é necessário verificar se a lógica de comparação entre sensores também precisa ser ajustada.

### 3.4. Intervalo entre leituras

O intervalo de envio pode ser alterado pelo valor de `delay()` no final de `loop()`, em milissegundos:

```cpp
delay(2000); // Aproximadamente 2 segundos entre ciclos
```

Para um intervalo de aproximadamente um segundo, use `delay(1000)`. O tempo total entre mensagens também depende do tempo gasto nas demais operações do programa.

## 4. Formato de saída Serial

Os dados são enviados como um objeto JSON em uma única linha, finalizada por uma quebra de linha (`\n`). Um exemplo de saída é:

```json
{"ldr_media":512.50,"tensao_V":5.02,"corrente_mA":120.40,"potencia_mW":604.40,"status":1}
```

Os campos representam:

- `ldr_media`: média das leituras dos LDRs;
- `tensao_V`: tensão, em volts;
- `corrente_mA`: corrente, em miliampères;
- `potencia_mW`: potência, em miliwatts;
- `status`: código que representa o estado classificado pelo firmware.

Os valores apresentados são apenas um exemplo do formato da mensagem.

### 4.1. Inclusão de campos no JSON

Ao adicionar campos, mantenha a estrutura do objeto JSON e separe os pares de chave e valor por vírgulas. Utilize `Serial.print()` para enviar os campos intermediários e finalize o objeto com uma quebra de linha, por exemplo, usando `Serial.println()`. Assim, cada registro continua sendo transmitido em uma linha independente.
