# GuitarPedal-Arduino

• Pedaleira de Guitarra com Arduíno


• Projeto para a disciplina de Projeto Integrado de Computação (PIC) 2023/02 - UFES


• Grupo: Thales Melo, Gustavo Cassaro, Daniel Almeida

• Efeitos: 
  1. Chorus
  2. Distortion
  3. Clean
  4. Tremolo
  5. UpDown

## Observações sobre o funcionamento do código
• Para utilizar o código na Pedaleira, é necessário baixar o conteúdo do repositório, compilar e dar upload somente na pasta "main", que contém o código principal e os efeitos implementados. A pasta standby existente possui os efeitos que estão sob processo de implementação e, portanto, ainda não são utilizados.

• Implementamos diferentes bibliotecas próprias para cada efeito, de forma que a função de interrupção ISR no main.ino trabalhe com funções de processamento diferentes toda vez que o efeito é alterado com o encoder;

• Criamos o “efeitos.h” e “efeitos.cpp” com uma struct que guarda ponteiros de função para o processamento dos diferentes efeitos. Portanto, ao alterar o valor do encoder, uma função diferente é selecionada por meio dessa struct.

• Possui espaço para quantos efeitos quisermos adicionar

• Alguns efeitos utilizam grandes buffers, então, para evitar problemas, trabalhamos com alocação dinâmica de memória. Essa memória é alocada no início do processamento do efeito e liberada após o término. Isso é necessário pois o Arduino UNO tem memória bastante limitada
