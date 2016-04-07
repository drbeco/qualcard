= qualcard =

== Introdução ==

Projeto de Aprendizado de temas diversos via repetição espaçada de cartões (flash cards).

== Regras Implementadas ==

* A cada execução são apresentadas 10 cartões
* Pelo menos um desses 10 cartões deve ser novo
* Se não houverem cartões novos, todos os 10 cartões serão reforçados por repetição
* No máximo nove cartões que precisam de reforço por repetição são apresentados
* Se não houver cartões que precisam de repetição por reforço, todos os 10 cartões serão novos
* Os cartões apresentados são auto-avaliados pelo estudante com uma nota N de 0 a 5 (sendo 0 para não conhecer/lembrar, e 5 para acerto com facilidade e confiança)
* Uma vez apresentados os cartões são agendados para reapresentação com a seguintes escala:
    - (A) Média < 2.00   -> reagendar para o dia seguinte
    - (B) Média < 3.25   -> reagendar para 3 dias
    - (C) Média < 4.50   -> reagendar para 5 dias
    - (D) Caso contrário -> reagendar para 7 dias
* A nova média M1 é calculada com a fórmula: M1 = (M0 + N) / 2
* Quando todos os cartões estiverem com média (D) o programa parabeniza o estudante e imprime um certificado com:
    - Nome completo
    - Data de início
    - Data de conclusão
    - Carga horária
    - Número de cartões aprendidos
    - Tema
    - Número de série de certificação
    - Assinatura do Prof. Dr. Ruben Carlo Benante <rcb@beco.cc>
* As médias usadas para agendamento têm um decaimento suave contínuo pelo período que o programa deixa de ser usado. Para evitar este decaimento, deve-se usar o programa no mínimo uma vez a cada dois dias.

== Licença  ==

=== Autor ===
Prof. Dr. Ruben Carlo Benante
Email: <rcb@beco.cc>
Data: 2016-04-07

=== Colaboradores ===
Gerônimo de Sá Barreto Neto
Email: <geronimoneto1994@gmail.com>

=== Licensa ===

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

=== License ===

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




