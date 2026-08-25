Sobre o projeto

Projeto desenvolvido em equipe utilizando um Arduino UNO, com o objetivo de construir um robô autônomo capaz de identificar um robô adversário e empurrá-lo para fora da arena durante uma competição de sumô.

O projeto envolveu o desenvolvimento do código de controle, montagem dos componentes eletrônicos, construção da estrutura mecânica e preparação do robô de acordo com as regras estabelecidas para a competição.

Objetivos

O principal objetivo foi desenvolver um robô 100% autônomo, capaz de identificar a presença de um adversário e tomar decisões de movimentação para tentar empurrá-lo para fora da arena.

O robô também deveria respeitar as condições de operação da competição, incluindo o atraso obrigatório de 5 segundos antes de iniciar a movimentação.

A arena utilizada possui formato circular, com 77 cm de diâmetro, e conta com uma faixa branca de borda de 2,5 cm de largura.

Funcionamento

Ao ser ativado no início de um round, o robô permanece parado durante os 5 segundos obrigatórios de atraso. Após esse período, inicia sua movimentação de forma autônoma.

Durante a partida, o robô utiliza seus sensores para identificar a presença do adversário e executar sua estratégia de movimentação, buscando posicionar-se de forma adequada para empurrá-lo para fora da arena.

A partida pode ter até 3 rounds de 1 minuto, podendo ser estendida para até 1 minuto e 30 segundos a critério do juiz. A vitória é determinada pela obtenção de 2 pontos de Yukô.

Um ponto de Yukô pode ser concedido quando o robô empurra o adversário para fora da arena ou em outras situações previstas no regulamento da competição.

O sistema também foi desenvolvido considerando as penalidades relacionadas à movimentação antecipada, falha de partida e imobilidade durante o round.

Estrutura mecânica

[Estrutura por escrito](docs/Hardware.md)
[Fotos da estrutura](assets)

Equipe

[Organização da equipe](docs/Equipe.md)

Edital

[Regras do campeonato](docs/Regras_Campeonato.pdf)

Código

[Código do robô](codigo)
