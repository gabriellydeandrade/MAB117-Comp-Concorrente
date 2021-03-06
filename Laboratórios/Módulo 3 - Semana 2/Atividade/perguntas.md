## Laboratório 8

### Objetivo

O objetivo deste Laboratorio é praticar o uso de semáforos para implementar exclusão
mútua e sincronização condicional. Para cada atividade, siga o roteiro proposto e responda as 
questões colocadas.

### Atividade

O algoritmo abaixo implementa uma solução para o problema dos leitores e escritores
(mais de um leitor pode ler ao mesmo tempo; apenas um escritor pode escrever de cada
vez e nenhum leitor pode estar lendo) com prioridade para escrita (sempre que ha um escritor esperando, o acesso para novos leitores é bloqueado até que o escritor seja atendido).

OBS.: Algoritmo encontrado no [pdf da atividade](lab8.pdf)

#### (a) Quais devem ser os valores iniciais dos semaforos (em_e, em_l, escr, leit) para que o algoritmo funcione corretamente?

Como `em_e` e `em_l` se referem as variáveis de mutex do algoritmo, ambos os valores devem começar com 1.
Já para as variáveis de condição, temos que:
 - `escr`: no caso da condição para o escritor também temos que ter o valor inicial como 1 pois não podemos ter dois escritores
atuando juntos no nosso programa
- `leit`: na condição para leitor, também temos que o valor inicial deve ser 1 pois se não teríamos um cenário de starvation
para leitura e não garantindo uma prioridade para escritores na maneira que foi projetado o algoritmo


#### (b) É possível simplificar esse código (reduzir o uso dos mecanismos de sincronização)?

Os mecanismos de sincronização relacionados com o mutex precisam ser utilizados para evitar condição de corrida. 
Na implementação, são utilizados dois mutex para acessar variáveis globais diferentes. Poderíamos reduzir para ter somente
um mutex, mas realizando de forma separada estaríamos permitindo que outra thread que esteja utilizando uma outra
variável global seja executada, tendo um menor overhead na aplicação. 
Logo, o `em_e` e `em_l` é aconselhável não serem reduzidos para somente um mutex.

Não há como reduzir também o uso dos mecanismos de sincronização relacionados com condição. Uma vez que queremos prioridade
para os escritores, precisamos garantir que assim que chegar novas threads escritoras, essas tarefas sejam executadas logo a 
seguir colocando a leitura em espera. Nesse fluxo, precisamos de variáveis de condição adicionais (condição para leitura e
exclusão mútua para a escrita).


#### (c) Implemente um programa em C para experimentar esse algoritmo. Inclua mensagens de log para acompanhar a execução e verificar a sua corretude. Teste seu programa variando o numero de threads “leitoras” e “escritoras”. Certifique-se que ele funciona em todos os casos.

O programa encontra-se no arquivo [semaforo_exclusivo.c](semaforo_exclusivo.c)

#### (d) Altere o código das duas threads substituindo o maior número possível de semáforos por locks.

O programa encontra-se no arquivo [semaforo_com_locks.c](semaforo_com_locks.c)

#### (e) Descreva se os locks usados são recursivos ou não e justifique a escolha feita.

Para o programa [semaforo_com_locks.c](semaforo_com_locks.c) modificamos as variáveis de exclusão mútua para utilizar o
mutex ao invés do semáforo com pthread_mutex_lock. Nesse caso não foi utilizado nenhum lock recursivo pois se trata apenas
de acesso á região crítica do código (variável global).


**Extra:**

Podemos também modificar a solução para não ter mais nenhum uso de semáforo, mas pra isso precisaremos de uma variável 
adicional para controlar a quantidade de tarefas que querem escrever para que elas tenham prioridade. Esta versão foi 
desenvolvida no arquivo [sem_semaforo.c](sem_semaforo.c)

1- Locks de mutex

Esses locks utilizados não são recursivos pois eles foram utilizado somente para o uso do mutex, esse uso é para não acessar
regiões críticas do código que podem gerar condições de corrida. Trocamos o uso do semáforo pelo pthread_mutex_lock.
Como foi modificado o trecho para usar a seção crítica, estaremos utilizando apenas um mutex.

2- Locks de condição

Foi utilizado locks recursivos para validar as condições de leitura e escrita. Para permitir a mesma lógica de prioridade para
escritores, foi utilizado uma variável a mais auxiliar para informar o tamanho da fila que estará se formando para a escrita.
Se essa fila não estiver vazia, novos leitores não poderão iniciar seus trabalhos e eles deverão ficar retidos no
pthread_cond_wait. O uso do lock recursivo é necessário pois como as tarefas podem demorar para ganhar CPU novamente na execução,
poderíamos ter um furo na nossa lógica de permitir leitura e escrita no momento certo pois a condição não seria validada
novamente.