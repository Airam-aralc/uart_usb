![68747470733a2f2f736f667465782e62722f77702d636f6e74656e742f75706c6f6164732f323032342f30392f456d6261726361546563685f6c6f676f5f417a756c2d31303330783432382e706e67](https://github.com/user-attachments/assets/f14bd1a4-fe90-467a-ac58-8068b8722877)

# Projeto de Comunicação Serial no RP2040 com BitDogLab

video desmonstrativo da atividade: https://www.dropbox.com/scl/fi/qxn3bid9k4cmur3ok9fsq/atividade-04_02.mp4?rlkey=1u7ey7ahqjbnlkpqqf39za530&st=pjonoh0e&dl=0

**Descrição**

Este projeto tem como objetivo explorar e aplicar os conceitos de comunicação serial em microcontroladores utilizando a placa BitDogLab baseada no RP2040. O projeto envolve a interação com diversos componentes, como uma matriz de LEDs WS2812, um LED RGB, botões de entrada e um display SSD1306 via protocolo I2C.

**Objetivos**

* Compreender e implementar comunicação serial (UART e I2C).

* Manipular LEDs comuns e LEDs endereçáveis WS2812.

* Implementar o uso de botões com interrupções e debounce via software.

* Desenvolver um projeto funcional integrando hardware e software.

**Componentes Utilizados**

* Matriz 5x5 de LEDs WS2812 (GPIO 7)

* LED RGB- verde e azul (GPIOs 11 e 12)

* Botão A (GPIO 5)

* Botão B (GPIO 6)

* Display SSD1306 via I2C (GPIO 14 e 15)

**Funcionalidades Implementadas**

1. Inclusão de caracteres minúsculos na biblioteca font.h para exibição no display SSD1306.

2. Caracteres digitados no Serial Monitor são exibidos no display SSD1306.

3. Números de 0 a 9 exibem um símbolo correspondente na matriz 5x5 WS2812.

4. Interação com o Botão A 
   * Alterna o estado do LED RGB Verde (ON/OFF).
   * Exibe mensagem no display SSD1306.
   * Envia descrição da ação ao Serial Monitor.

5. Interação com o Botão B
   * Alterna o estado do LED RGB Azul (ON/OFF).
   * Exibe mensagem no display SSD1306.
   * Envia descrição da ação ao Serial Monitor.

**Requisitos do Projeto**

* Uso de Interrupções: Implementação de rotinas de interrupção (IRQ) para os botões.

* Debouncing via Software: Garantia de leitura confiável dos botões.

* Controle de LEDs: Implementação de controle eficiente de LEDs comuns e WS2812.

* Utilização do Display SSD1306: Exibição correta de caracteres e interação via I2C.

* Envio de Dados pela UART: Integração com o Serial Monitor.

* Organização e Comentários no Código: Estrutura limpa e compreensível.
