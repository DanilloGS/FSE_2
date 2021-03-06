# FSE_2
 
## Info
| Aluno                      | Matricula  | Github    |
| -------------------------- | ---------- | --------- |
| Danillo Gonçalves de Souza | 17/0139981 | DanilloGS |
 
 
## Introdução
 
Este trabalho tem por objetivo a criação de um sistema distribuído de automação predial para monitoramento e acionamento de sensores e dispositivos de um prédio de 2 andares. O sistema deve ser desenvolvido para funcionar em um conjunto de placas Raspberry Pi com um servidor central responsável pelo controle e interface com o usuário e servidores distribuídos para leitura e acionamento dos dispositivos. Dentre os dispositivos envolvidos estão o monitoramento de temperatura e umidade, sensores de presença, sensores de fumaça, sensores de contagem de pessoas, sensores de abertura e fechamento de portas e janelas, acionamento de lâmpadas, aparelhos de ar-condicionado, alarme e aspersores de água em caso de incêndio.
 
![arquitetura](assets/arquitetura.png)
 
## Executar
 
- Dentro de cada diretorio (Central e Distribuido) possui um arquivo _Makefile_ para compilar cada um dos servidores. Para executar o arquivo _Makefile_ utilize o comando ```make``` no terminal para cada um dos diretórios.
 
- Então basta utilizar o comando ```./bin/bin``` na pasta raíz de cada diretório.
 
- Deve-se executar o binário do servidor Central e então do Distribuido, respectivamente.
 
- Assim que o código começar a executar e conectar-se uns com os outros, deve ser renderizada a seguinte página.
 
![menu](assets/menu.png)
 
 
## Instrução
 
- Para acionar uma lâmpada ou ar condicionado deve-se apertar as teclas ```Ctrl + \``` e então escolher qual pino o valor será alterado.
 
- Para trocar o andar que está sendo monitorado deve-se apertar as teclas ```Ctrl + Z``` e aguardar.

- Para encerrar o programa deve-se apertar as teclas ```Ctrl + C``` em ambos os processos.
 
## Checklist
 
| ITEM                                        | DETALHE                                                                                                                                                                                                   |                  Implementado                   |
| ------------------------------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | :---------------------------------------------: |
| **Servidor Central**                        |                                                                                                                                                                                                           |                                                 |
| **Interface (Monitoramento)**               | Interface gráfica (via terminal, web, etc) apresentando o estado de cada dispositivo (entradas e saídas), a temperatura, umidade e o número de pessoas ocupando o prédio sendo atualizada periodicamente. |                        Y                        |
| **Interface (Acionamento de Dispositivos)** | Mecanismo para acionamento de lâmpadas e aparelhos de ar-condicionado.                                                                                                                                    |                        Y                        |
| **Acionamento do Alarme**                   | Mecanismo de ligar/desligar alarme e acionamento do alarme de acordo com o estado dos sensores.                                                                                                           |                        N                        |
| **Alarme de Incêndio**                      | Implementação da rotina de acionamento do alarme de incêndio com o correto acionamento dos aspersores.                                                                                                    |                        N                        |
| **Log (CSV)**                               | Geração de Log em arquivo CSV.                                                                                                                                                                            |                        N                        |
| **Servidores Distribuídos**                 |                                                                                                                                                                                                           |                                                 |
| **Inicialização (Arquivo de Configuração)** | Inicialização do serviço a partir do arquivo de configuração JSON.                                                                                                                                        | Y/N (Socket distribuído possui porta aleatória) |
| **Leitura de Temperatura / Umidade**        | Leitura, armazenamento e envio dos valores de temperatura / umidade.                                                                                                                                      |                        Y                        |
| **Acionamento de Dispositivos**             | Acionamento de lâmpadas, aparelhos de ar-condicionado e aspersor pelo comando do Servidor Central.                                                                                                        |                        Y                        |
| **Estado dos Sensores**                     | Leitura e envio (*mensagem push*) para o Servidor Central um alerta pelo acionamento dos sensores de presença / abertura de portas/janelas e sensor de fumaça.                                            |                        N                        |
| **Contagem de pessoas**                     | Leitura dos sensores de contagem de pessoas.                                                                                                                                                              |                        Y                        |
| **Geral**                                   |                                                                                                                                                                                                           |                                                 |
| **Comunicação TCP/IP**                      | Implementação de comunicação entre os servidores usando o protocolo TCP/IP, incluindo as mensagens do tipo *push*.                                                                                        |                        Y                        |
 
 

