// ============================================================================
// ARQUIVO: 3986553-Ativ01-B.c
// ALUNO: Alicio Rodrigues de Souza
// RU: 3986553
// DESCRICAO:
// PIC16F877A
// PLACA PIC-DIP40 Versao 2.0
// Contador de pessoas com 2 portas utilizando sensores LDR e LCD 16x2
// LCD ligado em modo 8 bits conforme placa
// ============================================================================

#include <xc.h>

// ================= CONFIGURACAO =================
#pragma config FOSC = XT        // Oscilador externo de 4 MHz
#pragma config WDTE = OFF       // Watchdog desligado
#pragma config PWRTE = ON       // Power-up timer ligado
#pragma config BOREN = ON       // Brown-out reset ligado
#pragma config LVP = OFF        // Programacaoo em baixa tensao desligada
#pragma config CPD = OFF        // Protecao EEPROM desligada
#pragma config WRT = OFF        // Protecao escrita desligada
#pragma config CP = OFF         // Protecao codigo desligada

#define _XTAL_FREQ 4000000      // Frequencia do cristal

// ================= ENTRADAS =================
#define S1_6553 RB0             // Sensor 1 porta 1
#define S2_6553 RB1             // Sensor 2 porta 1
#define S3_6553 RB2             // Sensor 3 porta 2
#define S4_6553 RB3             // Sensor 4 porta 2
#define RESET_6553 RB4          // Bot�o reset

// ================= LEDs =================
#define LED_STATUS_6553 RC1     // LED piscante de funcionamento
#define LED_OCUPADO_6553 RC2    // LED que indica pessoas dentro
#define LED_ENTRADA_6553 RC3    // LED de entrada
#define LED_SAIDA_6553 RC4      // LED de sa�da

// ================= LCD =================
#define LCD_RS_6553 RE0         // RS do LCD
#define LCD_RW_6553 RE1         // RW do LCD
#define LCD_EN_6553 RE2         // EN do LCD

// ================= CONSTANTES =================
#define TIMEOUT_6553 1000       // 1000 ciclos x 2 ms = 2 segundos

// ================= VARI�VEIS =================
unsigned int pessoas_6553 = 0;              // Quantidade atual de pessoas dentro
unsigned int entradas_6553 = 0;             // Total de entradas
unsigned int saidas_6553 = 0;               // Total de sa�das

unsigned int t1_6553 = 0;                   // Temporizador porta 1
unsigned int t2_6553 = 0;                   // Temporizador porta 2

unsigned char estado1_6553 = 0;             // Estado da porta 1
unsigned char estado2_6553 = 0;             // Estado da porta 2

unsigned int ledEntradaTimer_6553 = 0;      // Temporizador do LED de entrada
unsigned int ledSaidaTimer_6553 = 0;        // Temporizador do LED de sa�da

unsigned int timerStatus_6553 = 0;          // Temporizador do LED de status

unsigned char lcdAtualizar_6553 = 1;        // Flag para atualizar LCD apenas quando necess�rio

unsigned int pessoasAnt_6553 = 65535;       // Guarda �ltimo valor mostrado de pessoas
unsigned int entradasAnt_6553 = 65535;      // Guarda �ltimo valor mostrado de entradas
unsigned int saidasAnt_6553 = 65535;        // Guarda �ltimo valor mostrado de sa�das

// ================= FUN��ES LCD =================
void lcdCmd_6553(unsigned char cmd){
    LCD_RS_6553 = 0;                        // Seleciona modo comando
    LCD_RW_6553 = 0;                        // Seleciona escrita
    PORTD = cmd;                            // Coloca comando nas linhas de dados
    LCD_EN_6553 = 1;                        // Gera pulso de enable
    __delay_us(20);                         // Pequeno atraso
    LCD_EN_6553 = 0;                        // Finaliza pulso
    __delay_ms(2);                         // Aguarda LCD processar
}

void lcdChar_6553(unsigned char data){
    LCD_RS_6553 = 1;                        // Seleciona modo dado
    LCD_RW_6553 = 0;                        // Seleciona escrita
    PORTD = data;                           // Coloca caractere nas linhas de dados
    LCD_EN_6553 = 1;                        // Gera pulso de enable
    __delay_us(20);                         // Pequeno atraso
    LCD_EN_6553 = 0;                        // Finaliza pulso
    __delay_ms(2);                         // Aguarda LCD processar
}

void lcdInit_6553(){
    __delay_ms(20);                         // Aguarda energiza��o do LCD
    lcdCmd_6553(0x38);                      // Modo 8 bits, 2 linhas
    lcdCmd_6553(0x0C);                      // Display ligado, cursor desligado
    lcdCmd_6553(0x06);                      // Incrementa cursor automaticamente
    lcdCmd_6553(0x01);                      // Limpa LCD
    __delay_ms(2);                          // Aguarda final da limpeza
}

void lcdSet_6553(unsigned char linha, unsigned char coluna){
    unsigned char pos_6553;                 // Vari�vel para posi��o DDRAM

    if(linha == 1)                          // Verifica se � linha 1
        pos_6553 = 0x80 + (coluna - 1);     // Calcula posi��o da linha 1
    else                                    // Caso contr�rio, usa linha 2
        pos_6553 = 0xC0 + (coluna - 1);     // Calcula posi��o da linha 2

    lcdCmd_6553(pos_6553);                  // Envia posi��o ao LCD
}

void lcdStr_6553(const char *s){
    while(*s)                               // Continua at� final da string
        lcdChar_6553(*s++);                 // Escreve caractere e avan�a ponteiro
}

void lcdNum2_6553(unsigned int v){
    if(v > 99) v = 99;                      // Limita em 2 d�gitos
    lcdChar_6553((v / 10) + '0');           // Escreve dezena
    lcdChar_6553((v % 10) + '0');           // Escreve unidade
}

void lcdNum3_6553(unsigned int v){
    if(v > 999) v = 999;                    // Limita em 3 d�gitos
    lcdChar_6553((v / 100) + '0');          // Escreve centena
    lcdChar_6553(((v / 10) % 10) + '0');    // Escreve dezena
    lcdChar_6553((v % 10) + '0');           // Escreve unidade
}

// Atualiza tela apenas quando houver mudan�a real
void atualizarLCD_6553(){
    if((pessoas_6553 != pessoasAnt_6553) || (entradas_6553 != entradasAnt_6553) || (saidas_6553 != saidasAnt_6553) || (lcdAtualizar_6553 == 1)){
        
        lcdSet_6553(1,1);                   // Vai para linha 1, coluna 1
        lcdStr_6553("ENT:");                // Escreve texto
        lcdNum2_6553(entradas_6553);        // Escreve valor de entradas

        lcdSet_6553(1,9);                   // Vai para linha 1, coluna 9
        lcdStr_6553("SAI:");                // Escreve texto
        lcdNum2_6553(saidas_6553);          // Escreve valor de sa�das

        lcdSet_6553(2,1);                   // Vai para linha 2, coluna 1
        lcdStr_6553("DENTRO:");             // Escreve texto
        lcdNum3_6553(pessoas_6553);         // Escreve valor de pessoas dentro
        lcdStr_6553(" ");                   // Espa�o extra para limpar sobra visual

        pessoasAnt_6553 = pessoas_6553;     // Atualiza valor anterior de pessoas
        entradasAnt_6553 = entradas_6553;   // Atualiza valor anterior de entradas
        saidasAnt_6553 = saidas_6553;       // Atualiza valor anterior de sa�das

        lcdAtualizar_6553 = 0;              // Limpa flag de atualiza��o
    }
}

// ================= FUN��ES DE LED =================

// Pisca LED de status em velocidade adequada
void ledStatus_6553(){
    timerStatus_6553++;                     // Incrementa temporizador

    if(timerStatus_6553 >= 125) {           // 125 x 2 ms = 250 ms
        LED_STATUS_6553 = !LED_STATUS_6553; // Inverte estado do LED
        timerStatus_6553 = 0;               // Reinicia temporizador
    }
}

// Atualiza LED que indica pessoas dentro
void ledOcupado_6553(){
    if(pessoas_6553 > 0)                    // Se houver pessoas dentro
        LED_OCUPADO_6553 = 0;               // Liga LED (ativo baixo)
    else
        LED_OCUPADO_6553 = 1;               // Desliga LED
}

// Inicia pulso do LED de entrada
void pulsoEntrada_6553(){
    LED_ENTRADA_6553 = 0;                   // Liga LED entrada
    ledEntradaTimer_6553 = 60;              // 60 x 2 ms = 120 ms
}

// Inicia pulso do LED de sa�da
void pulsoSaida_6553(){
    LED_SAIDA_6553 = 0;                     // Liga LED sa�da
    ledSaidaTimer_6553 = 60;                // 60 x 2 ms = 120 ms
}

// Atualiza pulsos dos LEDs sem travar o programa
void atualizaPulsos_6553(){
    if(ledEntradaTimer_6553 > 0){           // Verifica se pulso de entrada est� ativo
        ledEntradaTimer_6553--;             // Decrementa timer
        if(ledEntradaTimer_6553 == 0)       // Quando terminar
            LED_ENTRADA_6553 = 1;           // Desliga LED entrada
    }

    if(ledSaidaTimer_6553 > 0){             // Verifica se pulso de sa�da est� ativo
        ledSaidaTimer_6553--;               // Decrementa timer
        if(ledSaidaTimer_6553 == 0)         // Quando terminar
            LED_SAIDA_6553 = 1;             // Desliga LED sa�da
    }
}

// ================= EVENTOS =================

// Registra entrada v�lida
void registraEntrada_6553(){
    pessoas_6553++;                         // Soma pessoa dentro
    entradas_6553++;                        // Soma entrada
    pulsoEntrada_6553();                    // Aciona LED entrada
    ledOcupado_6553();                      // Atualiza LED ocupado
    lcdAtualizar_6553 = 1;                  // Solicita atualiza��o do LCD
}

// Registra sa�da v�lida somente se houver pessoas dentro
void registraSaida_6553(){
    if(pessoas_6553 > 0){                   // S� permite sa�da se houver algu�m dentro
        pessoas_6553--;                     // Subtrai pessoa dentro
        saidas_6553++;                      // Soma sa�da
        pulsoSaida_6553();                  // Aciona LED sa�da
        ledOcupado_6553();                  // Atualiza LED ocupado
        lcdAtualizar_6553 = 1;              // Solicita atualiza��o do LCD
    }
}

// ================= PORTAS =================

// L�gica da porta 1
void porta1_6553(){
    if(estado1_6553 == 0){                  // Porta livre
        if(S1_6553 && !S2_6553){            // Sensor 1 ativou primeiro
            estado1_6553 = 1;               // Poss�vel entrada
            t1_6553 = 0;                    // Zera temporizador
        }
        else if(S2_6553 && !S1_6553){       // Sensor 2 ativou primeiro
            estado1_6553 = 2;               // Poss�vel sa�da
            t1_6553 = 0;                    // Zera temporizador
        }
    }
    else if(estado1_6553 == 1){             // Estado de poss�vel entrada
        t1_6553++;                          // Incrementa tempo

        if(S2_6553){                        // Se segundo sensor ativar
            registraEntrada_6553();         // Confirma entrada
            estado1_6553 = 3;               // Bloqueia at� liberar sensores
        }
        else if(t1_6553 > TIMEOUT_6553){    // Timeout de 2 segundos
            estado1_6553 = 0;               // Cancela evento
        }
    }
    else if(estado1_6553 == 2){             // Estado de poss�vel sa�da
        t1_6553++;                          // Incrementa tempo

        if(S1_6553){                        // Se segundo sensor ativar
            registraSaida_6553();           // Confirma sa�da
            estado1_6553 = 3;               // Bloqueia at� liberar sensores
        }
        else if(t1_6553 > TIMEOUT_6553){    // Timeout de 2 segundos
            estado1_6553 = 0;               // Cancela evento
        }
    }
    else if(estado1_6553 == 3){             // Estado bloqueado
        if(!S1_6553 && !S2_6553)            // Quando ambos sensores forem liberados
            estado1_6553 = 0;               // Libera nova leitura
    }
}

// L�gica da porta 2
void porta2_6553(){
    if(estado2_6553 == 0){                  // Porta livre
        if(S3_6553 && !S4_6553){            // Sensor 3 ativou primeiro
            estado2_6553 = 1;               // Poss�vel entrada
            t2_6553 = 0;                    // Zera temporizador
        }
        else if(S4_6553 && !S3_6553){       // Sensor 4 ativou primeiro
            estado2_6553 = 2;               // Poss�vel sa�da
            t2_6553 = 0;                    // Zera temporizador
        }
    }
    else if(estado2_6553 == 1){             // Estado de poss�vel entrada
        t2_6553++;                          // Incrementa tempo

        if(S4_6553){                        // Se segundo sensor ativar
            registraEntrada_6553();         // Confirma entrada
            estado2_6553 = 3;               // Bloqueia at� liberar sensores
        }
        else if(t2_6553 > TIMEOUT_6553){    // Timeout de 2 segundos
            estado2_6553 = 0;               // Cancela evento
        }
    }
    else if(estado2_6553 == 2){             // Estado de poss�vel sa�da
        t2_6553++;                          // Incrementa tempo

        if(S3_6553){                        // Se segundo sensor ativar
            registraSaida_6553();           // Confirma sa�da
            estado2_6553 = 3;               // Bloqueia at� liberar sensores
        }
        else if(t2_6553 > TIMEOUT_6553){    // Timeout de 2 segundos
            estado2_6553 = 0;               // Cancela evento
        }
    }
    else if(estado2_6553 == 3){             // Estado bloqueado
        if(!S3_6553 && !S4_6553)            // Quando ambos sensores forem liberados
            estado2_6553 = 0;               // Libera nova leitura
    }
}

// ================= MAIN =================
void main(){

    ADCON1 = 0x06;                          // Configura tudo como digital

    TRISB = 0b00011111;                     // RB0 a RB4 como entradas
    TRISC = 0b00000000;                     // PORTC como sa�da
    TRISD = 0x00;                           // PORTD como sa�da
    TRISE = 0x00;                           // PORTE como sa�da

    PORTB = 0x00;                           // Inicializa PORTB
    PORTC = 0xFF;                           // Inicializa LEDs apagados em l�gica ativa baixa
    PORTD = 0x00;                           // Inicializa LCD dados
    PORTE = 0x00;                           // Inicializa LCD controle

    lcdInit_6553();                         // Inicializa LCD
    lcdCmd_6553(0x01);                      // Limpa LCD uma �nica vez
    __delay_ms(2);                          // Aguarda limpeza

    lcdAtualizar_6553 = 1;                  // For�a atualiza��o inicial do visor
    atualizarLCD_6553();                    // Mostra valores iniciais

    while(1){

        ledStatus_6553();                   // Pisca LED de funcionamento
        atualizaPulsos_6553();              // Atualiza LEDs de entrada/sa�da sem travar

        porta1_6553();                      // Processa porta 1
        porta2_6553();                      // Processa porta 2

        // Tratamento do reset
        if(RESET_6553){                     // Se bot�o reset pressionado
            pessoas_6553 = 0;               // Zera pessoas dentro
            entradas_6553 = 0;              // Zera total de entradas
            saidas_6553 = 0;                // Zera total de sa�das
            estado1_6553 = 0;               // Reinicia estado porta 1
            estado2_6553 = 0;               // Reinicia estado porta 2
            t1_6553 = 0;                    // Zera temporizador porta 1
            t2_6553 = 0;                    // Zera temporizador porta 2
            ledOcupado_6553();              // Atualiza LED ocupado
            lcdAtualizar_6553 = 1;          // Solicita atualiza��o do LCD
            __delay_ms(50);                 // Pequeno debounce
            while(RESET_6553);              // Espera soltar bot�o
        }

        ledOcupado_6553();                  // Garante atualiza��o do LED ocupado
        atualizarLCD_6553();                // Atualiza LCD somente se necess�rio

        __delay_ms(2);                      // Loop do programa r�pido e est�vel
    }
}
# Auto detect text files and perform LF normalization
* text=auto
