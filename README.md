# Instruções de Uso

Este projeto implementa dois tipos de temporização no Raspberry Pi Pico W utilizando o Pico SDK e a linguagem C.

## Requisitos

- Raspberry Pi Pico W
- 3 LEDs para o semáforo (vermelho, amarelo e verde)
- 3 LEDs para o temporizador one-shot (azul, vermelho e verde)
- 3 resistores de 330Ω
- 1 botão (pushbutton)
- Ambiente de desenvolvimento configurado com VS Code e Pico SDK
- Simulador Wokwi para testes

## Compilação e Execução

1. Clone o repositório e entre na pasta do projeto:
   ```sh
   git clone <URL_DO_REPOSITORIO>
   cd <NOME_DO_REPOSITORIO>
   ```
2. Compile o código com CMake:
   ```sh
   mkdir build && cd build
   cmake ..
   make
   ```
3. Transfira o binário gerado para o Raspberry Pi Pico W:
   ```sh
   cp nome_do_binario.uf2 /media/usuario/RPI-RP2
   ```
4. Para testes no Wokwi, carregue o código no simulador e configure os pinos conforme indicado.

## Funcionamento

### 1. Temporizador Periódico (Semáforo)
- O semáforo inicia na cor vermelha.
- A cada 3 segundos, muda para a próxima cor (amarelo -> verde -> vermelho).
- A mudança ocorre na função `repeating_timer_callback()`.
- Mensagens de status são enviadas via porta serial a cada 1 segundo.

### 2. Temporizador de Um Disparo (One Shot)
- Pressionar o botão aciona os LEDs na sequência:
  - Todos acendem.
  - Após 3 segundos, apaga um LED por vez.
  - O botão só pode ser pressionado novamente após todos os LEDs apagarem.
- O controle ocorre nas funções `one_shot_callback()` e `button_callback()`.

### 3. Debounce do Botão
- Foi implementada uma rotina de debounce por software para evitar acionamentos indesejados.
- O botão só registra um novo clique se tiver passado ao menos 200ms desde o último acionamento.
- A função `debounce_button()` controla esse comportamento.

## Considerações

- A debouncing do botão pode ser ajustada conforme necessidade.
- O código está preparado para testes no Wokwi e execução no hardware real.
- Modificações podem ser feitas para ajustar tempos e comportamento dos LEDs.

## Autor
Brunna Barreto da Silva

## Contato
20241bsifsa0027@ifba.edu.br
