Descrição do trabalho:

Faça um programa que identifique, a partir de um arquivo-fonte de código de 3 endereços, os blocos básicos e a estrutura do grafo listando, para cada bloco básico, os comandos que o compõem e seus blocos sucessores, como no exemplo abaixo (podem ocorrer pequenas variações no formato da saída sugerido aqui):

Bloco 2: succs { 4 3 }
Bloco 3: succs { 4 6 }

<bb 2>:
if a>0 goto bb4
<bb 3>:
t0 = t0 +1
goto bb4;

<bb 4>

t1 = t0+1

A saída deve ser gerada em um arquivo.

O programa deve reconhecer labels, comandos de desvio condicional e incondicional e comandos de atribuicao de 1 e dois operandos.

Pode ser desenvolvido em duplas. 