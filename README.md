## Projeto 1 - 2020/2 - FSE

|Matrícula | Aluno |
| -- | -- |
| 15/0135521 |  Leonardo dos S. S. Barreiros |


## Objetivo

<p align="justify">Este trabalho tem por objetivo a implementação de um sistema de controle de temperatura, em que, através de uma Raspberry pi 4 realizamos toda a comunicação e elaboração de um ambiente controlado. Neste ambiente controlado possuimos os seguintes sensores:

- 01 Sensor LM35 para a medição da temperatura interna (TI) do ambiente controlado;
- 01 Sensor BME280 (I2C) para a medição da temperatura externa (TE);
- 01 módulo Display LCD 16x2 com circuito I2C integrado (Controlador HD44780);
- 01 Conversor lógico bidirecional (3.3V / 5V);
- Circuito de potência com 2 relés;
- 01 Arduino Micro;
- Potenciômetro para regular qual a temperatura referencial a ser obtida;
- Raspberry Pi 4;

## Dos requisitos do trabalho

<p align="justify"> [1] - Possui um menu interativo, onde o usuário pode acompanhar as temperaturas que são solicitadas, além de poder alternar entre usar temperatura pelo potenciomentro e uma temperatura manual.
<p align="justify"> [2] - Realiza leitura da temperatura interna.
<p align="justify"> [3] - Realiza leitura da temperatura ambiente.
<p align="justify"> [4] - Realiza leitura da temperatura Potenciômentro.
<p align="justify"> [5] - Realiza leitura de temperaturas através do controlador PID.
<p align="justify"> [6] - Realiza a escrita e atualização de dados no Display LCD.
<p align="justify"> [7] - Realiza o armazenamento dos dados em um arquivo CSV - nomeado como <strong>temperature_control.csv</strong>.
<p align="justify"> [8] - Código Modularizado.

## Compilação

Para compilar o trabalho, através do terminal e na raiz do repositório execute o comando:

```sh
$ make all
```
Assim será realizado a compilação das bibliotecas e scripts do trabalho.

Em seguida para rodar o trabalho, rode o seguinte comando:

```sh
$ make run
```
E então será aberto o menu interativo.

## Utilização

![](./assets/menu.jpg)

O menu possui 4 áreas:

<p align="justify"> Em Dados a parte de leitura onde irá mostrar as labels de temperaturas e solicitações. Em Resultados é mostrado todos os valores aferidos.Em ajuste temos as labels de pgUp e pgDown que são funções para ajustar a temperatura referencial, quando está no modo <strong>TEMPERATURA MANUAL</strong>.

<p align="justify"> Em comandos tempos duas opções que podem ser escolhidas pelas arrow keys(setas) do seu teclado direita ou esquerda, para alternar entre <strong>POTENCIÔMETRO</strong> e <strong>TEMPERATURA MANUAL</strong>.

<p align="justify"> É importante observar que a temperatura referencial somente irá mudar se estiver com o comando <strong>TEMPERATURA MANUAL</strong> acionado, e para isto acontecer basta apertar a seta para <strong>direita</strong>. Caso contrario não irá funcionar.


<p align="justify"> A temperatura referencial é limitada entre a <strong>temperatura ambiente</strong> como sendo esta a menor possível e a a temperatura máxima de <strong>99 ºC</strong>. E para ajustar a temperatura bastar usar as <strong>setas</strong> para cima para <strong>aumentar</strong> e para <strong>baixo</strong> para diminuir.

![](./assets/manual.jpg)
![](./assets/display.jpg)

<p align="justify"> O programa foi divido em duas threads, a thread principal que executa as atualizações dos dados, e a secundaria que executa o menu. Dentro da que executa a atualização dos dados, enquanto os dados são validados e atualizados ao final da execução é feito a escrita em um arquivo csv, nomeado como <strong>temperature control.csv</strong>. Toda vez que um dado valido é solicitado este mesmo será escrito neste arquivo.

Dentro do csv encontramos a seguinte estrutura:

- timestamp: para registrar a data em formato DD-MM-AAAA HH:MM:SS;
- Temperatura Interna: registro da temperatura interna solicitada; 
- Temperatura Referencial: registro da temperatura referencial solicitada; 
- Temperatura Externa: registro da temperatura externa solicitada; 
- Ventoinha: registro da intensidade da ventoinha de acordo com o pid; 
- resistor: registro da intensidade do resistor de acordo com o pid;


<p align="justify"> Para encerrar o programa, apenas aperte <strong>ctrl+c</strong>. O programa irá receber o sinal sigint e executar a função <strong>cancelProcess</strong>,
que irá desligar a ventoinha, o resistor, limpar o display LCD, fechar a uart, desligar o sensor BME e por ultimo finalizar a execução do programa, tudo nesta ordem. 

## Dos Resultados

Título: Temperaturas x tempo (onde td veio do potenciômetro nesse gráfico):

Legendas
- TI = __Temperatura interna__, em ºC;
- TE = __Temperatura externa__, em ºC;
- TR = __Temperatura definida pelo usuário__ (potenciômetro ou manual), em ºC;

![](./assets/plotTemp.jpg)

Título: Resistor & ventoínha x tempo

Legendas: 
- FAN = Valor de acionamento da __ventoínha__, em percentagem;
- RES = Valor de acionamento do __resistor__, em percentagem.

![](./assets/plotRSVT.jpg)