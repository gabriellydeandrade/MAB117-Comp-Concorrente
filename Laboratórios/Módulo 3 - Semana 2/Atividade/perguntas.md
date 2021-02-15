## Laboratório 8

### Objetivo

O objetivo deste Laboratorio é praticar o uso de semáforos para implementar exclusão
mútua e sincronização condicional. Para cada atividade, siga o roteiro proposto e responda as 
questões colocadas.

### Atividade

O algoritmo abaixo implementa uma solução para o problema dos leitores e escritores
(mais de um leitor pode ler ao mesmo tempo; apenas um escritor pode escrever de cada
vez e nenhum leitor pode estar lendo) com prioridade para escrita (sempre que ha um escritor esperando, o acesso para novos leitores e bloqueado até que o escritor seja atendido).

OBS.: Algoritmo encontrado no [pdf da atividade](lab8.pdf)

#### (a) Quais devem ser os valores iniciais dos semaforos (em_e, em_l, escr, leit) para que o algoritmo funcione corretamente?

Como `em_e` e `em_l` se referem as variáveis de mutex do algoritmo, ambos os valores devem começar com 1.
Já para as variáveis de condição, temos que:
 - `escr`: no caso da condição para o escritor também temos que ter o valor inicial como 1 pois não podemos ter dois escritores
atuando juntos no nosso programa
- `leit`: na condição para leitor, também temos que o valor inicial deve ser 1 pois se não teríamos um cenário de starvation
para leitura e não garantindo uma prioridade para escritores na maneira que foi projetado o algoritmo


#### (b) É possível simplificar esse código (reduzir o uso dos mecanismos de sincronização)?

Os mecanismos de sincronização relacionados com o mutex precisam ser utilizados para evitar condição de corrida. Logo, o
`em_e` e `em_l` não podem ser reduzidos.

Não há como reduzir também o uso dos mecanismos de sincronização relacionados com condição. Uma vez que queremos prioridade
para os escritores, precisamos garantir que assim que chegar novas threads escritoras, essas tarefas sejam executadas logo a 
seguir colocando a leitura em espera. Nesse fluxo, precisamos de variáveis de condição adicionais (condição para leitura e
exclusão mútua para a escrita).


#### (c) Implemente um programa em C para experimentar esse algoritmo. Inclua mensagens de log para acompanhar a execução e verificar a sua corretude. Teste seu programa variando o numero de threads “leitoras” e “escritoras”. Certifique-se que ele funciona em todos os casos.

O programa encontra-se no arquivo [semaforo_exclusivo.c](semaforo_exclusivo.c)

#### (d) Altere o codigo das duas threads substituindo o maior número possível de semáforos por locks.

#### (e) Descreva se os locks usados são recursivos ou não e justifique a escolha feita.