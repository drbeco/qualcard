# qualcard #

## Introdução ##

Projeto de Aprendizado de temas diversos via repetição espaçada de cartões (flash cards).

## Regras Implementadas ##

* A cada execução são apresentados 10 cartões
* Prioridades nos 10 cartões:
    - 9 cartões para reforço por repetição se houver atrasados
    - 1 ou mais cartões novos, se houverem, até completar 10 cartões
    - Qualquer quantidade de cartões repetidos não atrasados para completar a lista de 10
* Os cartões apresentados são auto-avaliados pelo estudante com uma nota N de 0 a 5 (sendo 0 para não conhecer/lembrar, e 5 para acerto com facilidade e confiança)
* Cartões que recebem a nota zero são repetidos ao final, quantas vezes forem necessárias, até que receba nota maior. Os zeros acumulam para cálculo da nota que passa a ser uma média.
* De posse da nota (N), uma nova média (M1) é calculada com a fórmula: M1 = (M0 + N) / 2, onde M0 é a média anterior. No primeiro ciclo, M1 = N - 23%.
* Uma vez apresentados, os cartões são agendados para reapresentação com a seguinte escala:
    - (H) Nota  == 0.00   -> reapresentar hoje ao final
    - (G) Média <= 0.60   -> reagendar para  1 dia
    - (F) Média <= 1.35   -> reagendar para  2 dias
    - (E) Média <= 2.20   -> reagendar para  3 dias
    - (D) Média <= 3.05   -> reagendar para  5 dias
    - (C) Média <= 3.90   -> reagendar para  7 dias
    - (B) Média <= 4.92   -> reagendar para  9 dias
    - (A) Caso contrário  -> reagendar para 11 dias
* Iniciando um cartão com a nota máxima 5.0, registrada como 5-23%=3.84, e recebendo sempre a nota máxima 5.0, sua progressão é: 3.84 (C), 4.42 (B), 4.71 (B), 4.85 (B), 4.93 (A).
* A nota, para efeito da porcentagem final, decai com o tempo:
    - Decaimento com taxa de 1 ponto a cada meia semana (3.5 dias).
    - Isso significa que se um cartão estiver com nota máxima 5.0, seu valor será considerado zero para efeito de cálculo de porcentagem após 25 dias vencido.
    - No mesmo exemplo, a nota máxima 5.0 será avaliada em 4.0 (perda de 1 ponto) em aproximadamente 10.5 dias.
    - Quando na reapresentação do cartão atrasado, para cálculo de M1, a nota M0 continua tendo o valor 5.0.
* Quando todos os cartões estiverem com média (A) ou (B), i.é, Nota>3.90, e dentro do prazo de revisão, o programa parabeniza o estudante e imprime um certificado com:
    - Nome completo e usuário
    - Data de início
    - Data de conclusão
    - Carga horária
    - Número de cartões aprendidos
    - Pontuação obtida (coluna Compl.%)
    - Tema do estudo
    - Número de série de certificação
    - Assinatura do Prof. Dr. Ruben Carlo Benante <rcb@beco.cc>
* As médias usadas para agendamento têm um decaimento suave contínuo pelo período que o programa deixa de ser usado. Para evitar este decaimento, deve-se usar o programa no mínimo uma vez a cada dois dias.

## Licença ##

### Autor ###

* Prof. Dr. Ruben Carlo Benante
    - Email: <rcb@beco.cc>
    - Data da criação: 2016-04-07

### Licença pt\_BR ###

qualcosa : um programa para estudar cartões usando repetição espaçada.

Copyright (C) 2016 Ruben Carlo Benante <rcb@beco.cc>

Este programa é software livre; você pode redistribuí-lo e/ou
modificá-lo sob os termos da Licença Pública Geral GNU, conforme
publicada pela Free Software Foundation; tanto a versão 2 da
Licença como (a seu critério) qualquer versão mais nova.

Este programa é distribuído na expectativa de ser útil, mas SEM
QUALQUER GARANTIA; sem mesmo a garantia implícita de
COMERCIALIZAÇÃO ou de ADEQUAÇÃO A QUALQUER PROPÓSITO EM
PARTICULAR. Consulte a Licença Pública Geral GNU para obter mais
detalhes.

Você deve ter recebido uma cópia da Licença Pública Geral GNU
junto com este programa; se não, escreva para a Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307, USA.

### License en\_US ###

qualcosa : a program to study cards using spaced repetition.

Copyright (C) 2016 Ruben Carlo Benante <rcb@beco.cc>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

