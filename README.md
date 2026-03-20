# 3986553-Ativ01B

contador de pessoas com visor LCD

## Autor
Alicio Rodrigues de Souza

## RU
3986553

## Descrição
Projeto desenvolvido utilizando o microcontrolador PIC16F877A para controle de entrada e saída de pessoas com exibição em display LCD 16x2.

## Hardware utilizado
- PIC16F877A
- LCD 16x2
- Sensores LDR
- LEDs de indicação
- Placa PIC-DIP40
- PICkit 3.5

## Arquivo principal
- 3986553-Ativ01B.c

- Este projeto consiste no desenvolvimento de um sistema embarcado para contagem de pessoas em um ambiente com duas portas, utilizando o microcontrolador PIC16F877A como unidade de processamento principal.

O sistema foi projetado para identificar automaticamente se uma pessoa está entrando ou saindo do ambiente, com base na ordem de acionamento de sensores ópticos instalados em cada porta. Para isso, cada porta utiliza dois sensores com LDR, formando uma barreira óptica com LED emissor de luz. Quando a pessoa interrompe o feixe luminoso, o microcontrolador detecta a mudança lógica do sensor.

A lógica de funcionamento foi implementada de forma a reconhecer:

entrada, quando o primeiro sensor da porta é ativado e em seguida o segundo sensor é ativado dentro de uma janela de tempo configurada;

saída, quando a sequência ocorre na ordem inversa.

O sistema possui uma janela máxima de aproximadamente 2 segundos entre o primeiro e o segundo sensor. Caso esse tempo seja excedido, a leitura é descartada. Após um evento válido, a porta entra em estado de bloqueio até que os dois sensores retornem ao estado normal, evitando múltiplas contagens indevidas.

Além da lógica de contagem, o sistema possui os seguintes recursos de indicação:

LED de status, que pisca continuamente para indicar que o sistema está em funcionamento;

LED de ocupado, que indica se há pelo menos uma pessoa dentro do ambiente;

LED de entrada, que sinaliza a ocorrência de uma entrada válida;

LED de saída, que sinaliza a ocorrência de uma saída válida.

Como interface de visualização, o projeto utiliza um display LCD 16x2, conectado em modo de 8 bits, já conforme a ligação física disponível na placa utilizada. O visor apresenta:

total de entradas;

total de saídas;

quantidade de pessoas dentro do ambiente.

Foi implementada também uma regra de segurança lógica importante:
o sistema só registra saída se houver pelo menos uma pessoa dentro do ambiente. Assim, se o contador estiver em zero, qualquer tentativa de saída é ignorada, sem atualizar LED de saída, sem incrementar total de saídas e sem alterar o LCD.

O projeto foi desenvolvido em linguagem C, utilizando o compilador MPLAB XC8, com cristal de 4 MHz, e foi gravado no PIC16F877A por meio do PICkit 3.5.

Esse sistema representa uma aplicação prática de sistemas embarcados, envolvendo integração entre sensores, microcontrolador, lógica sequencial, controle de saídas e interface homem-máquina, sendo adequado para fins acadêmicos, laboratoriais e de demonstração didática.

TABELA DE LIGAÇÕES
LCD

| Pino do LCD | Nome | Ligação                          |
| ----------- | ---- | -------------------------------- |
| 1           | GND  | GND                              |
| 2           | VCC  | +5 V                             |
| 3           | VO   | Cursor do potenciômetro de 10 kΩ |
| 4           | RS   | RE0                              |
| 5           | RW   | RE1                              |
| 6           | EN   | RE2                              |
| 7           | D0   | RD0                              |
| 8           | D1   | RD1                              |
| 9           | D2   | RD2                              |
| 10          | D3   | RD3                              |
| 11          | D4   | RD4                              |
| 12          | D5   | RD5                              |
| 13          | D6   | RD6                              |
| 14          | D7   | RD7                              |
| 15          | BLA  | +5 V                             |
| 16          | BLK  | GND                              |

PINAGEM DO PIC16F877A
| Pino | Nome     | Uso no projeto             |
| ---- | -------- | -------------------------- |
| 1    | MCLR/VPP | Programação / reset físico |
| 2    | RA0      | Livre                      |
| 3    | RA1      | Livre                      |
| 4    | RA2      | Livre                      |
| 5    | RA3      | Livre                      |
| 6    | RA4      | Livre                      |
| 7    | RA5      | Livre                      |
| 8    | RE0      | LCD_RS_6553                |
| 9    | RE1      | LCD_RW_6553                |
| 10   | RE2      | LCD_EN_6553                |
| 11   | VDD      | +5 V                       |
| 12   | VSS      | GND                        |
| 13   | OSC1     | Cristal 4 MHz              |
| 14   | OSC2     | Cristal 4 MHz              |
| 15   | RC0      | Livre                      |
| 16   | RC1      | LED_STATUS_6553            |
| 17   | RC2      | LED_OCUPADO_6553           |
| 18   | RC3      | LED_ENTRADA_6553           |
| 19   | RD0      | LCD_D0_6553                |
| 20   | RD1      | LCD_D1_6553                |
| 21   | RD2      | LCD_D2_6553                |
| 22   | RD3      | LCD_D3_6553                |
| 23   | RC4      | LED_SAIDA_6553             |
| 24   | RC5      | Livre                      |
| 25   | RC6      | Livre                      |
| 26   | RC7      | Livre                      |
| 27   | RD4      | LCD_D4_6553                |
| 28   | RD5      | LCD_D5_6553                |
| 29   | RD6      | LCD_D6_6553                |
| 30   | RD7      | LCD_D7_6553                |
| 31   | VSS      | GND                        |
| 32   | VDD      | +5 V                       |
| 33   | RB0      | SENSOR_S1_6553             |
| 34   | RB1      | SENSOR_S2_6553             |
| 35   | RB2      | SENSOR_S3_6553             |
| 36   | RB3      | SENSOR_S4_6553             |
| 37   | RB4      | BOTAO_RESET_6553           |
| 38   | RB5      | Livre                      |
| 39   | RB6      | PICkit PGC                 |
| 40   | RB7      | PICkit PGD                 |

Como testar o sistema
Teste de entrada na porta 1
acionar S1
depois acionar S2

resultado esperado:
incrementa total de entradas
incrementa número de pessoas dentro
LED_ENTRADA_6553 pulsa
LED_OCUPADO_6553 acende, se o contador ficar maior que zero

Teste de saída na porta 1
acionar S2
depois acionar S1

resultado esperado:
decrementa número de pessoas dentro
incrementa total de saídas
LED_SAIDA_6553 pulsa

Regra de segurança

Se não houver ninguém dentro, o sistema:
não registra saída
não pulsa LED de saída
não incrementa total de saídas
não altera o LCD

Porta 2

S3 → S4 = entrada
S4 → S3 = saída



