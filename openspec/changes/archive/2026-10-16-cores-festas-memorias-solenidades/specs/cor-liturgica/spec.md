# Spec Delta

## Purpose

Determina a cor litúrgica correta de um dia do Calendário Romano Geral, combinando a cor padrão da estação com as cores das celebrações de data fixa (solenidades, festas e memórias) e respeitando precedência e supressão em domingos.

## ADDED Requirements

### Requirement: Cores de solenidades, festas e memórias de data fixa
O sistema SHALL aplicar a cor litúrgica da celebração correspondente a um dia que contenha uma solenidade, festa ou memória obrigatória de data fixa do Calendário Romano Geral. A cor da celebração MUST prevalecer sobre a cor padrão da estação.

#### Scenario: Memória obrigatória num dia da semana
- **WHEN** o dia contém uma memória obrigatória de mártir num dia comum do Tempo Comum
- **THEN** a cor do dia é vermelho

#### Scenario: Solenidade sobre a cor da estação
- **WHEN** o dia é uma solenidade de Nossa Senhora durante o Tempo Comum
- **THEN** a cor do dia é branco

### Requirement: Ignorar memórias facultativas
O sistema SHALL NOT alterar a cor do dia por causa de uma comemoração facultativa (memória `[m]` ou `[m*]`). Nesses dias a cor MUST ser a padrão da estação.

#### Scenario: Memória facultativa no Tempo Comum
- **WHEN** o dia contém apenas uma memória facultativa de santo no Tempo Comum
- **THEN** a cor do dia continua verde

#### Scenario: Comemoração facultativa na Quaresma
- **WHEN** o dia contém uma comemoração facultativa violácea do tipo `[m*]` na Quaresma
- **THEN** a cor do dia continua violeta

### Requirement: Supressão em domingo
O sistema SHALL suprimir uma festa ou memória obrigatória de data fixa que caia num domingo, mantendo a cor do domingo da estação, em conformidade com o Calendário Romano Geral. Solenidades MUST NOT ser suprimidas.

#### Scenario: Festa suprimida num domingo do Tempo Comum
- **WHEN** uma festa de data fixa cai num domingo do Tempo Comum
- **THEN** a cor do dia é a do domingo da estação (verde) em vez da cor da festa

#### Scenario: Memória suprimida num domingo do Tempo Comum
- **WHEN** uma memória obrigatória de data fixa cai num domingo do Tempo Comum
- **THEN** a cor do dia é a do domingo da estação em vez da cor da memória

#### Scenario: Festa suprimida num domingo de Quaresma
- **WHEN** uma festa de data fixa cai num domingo de Quaresma (ex.: Cátedra de São Pedro em 2026)
- **THEN** a cor do dia é a do domingo da estação (violeta) em vez da cor da festa

#### Scenario: Solenidade não é suprimida em domingo
- **WHEN** uma solenidade cai num domingo
- **THEN** a cor do dia é a cor da solenidade

### Requirement: Precedência entre celebrações
Quando mais de uma celebração ocorre no mesmo dia, o sistema SHALL escolher a cor da celebração de maior precedência, na ordem solenidade > festa > domingo > memória > semana.

#### Scenario: Solenidade e memória no mesmo dia
- **WHEN** uma solenidade e uma memória ocorrem no mesmo dia
- **THEN** a cor do dia é a da solenidade

### Requirement: Cores suportadas
O sistema SHALL suportar as quatro cores litúrgicas do Calendário Romano Geral: branco, vermelho, verde e violeta.

#### Scenario: Dia da estação
- **WHEN** o dia não contém celebração relevante
- **THEN** a cor do dia é uma das cores suportadas derivadas da estação
