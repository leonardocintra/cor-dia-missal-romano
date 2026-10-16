# Proposal

## Why

O poc, o site e o ESP32 hoje só colorizam as ~20 celebrações fixas mais importantes do calendário romano. As festas `[F]`, memórias obrigatórias `[M]` e solenidades `[S]` do Calendário Romano Geral (~150 por ano) não têm cor aplicada — por exemplo, uma memória de santo no Tempo Comum fica verde em vez de branca/vermelha. Isso diverge do Missal Romano e do ICS de referência (2026/2027).

## What Changes

- Adicionar uma tabela embutida de celebrações de data fixa (`[S]`, `[F]` e `[M]`) com sua cor litúrgica correta, construída a partir dos ICS `2026-pt-General-G.ics` e `2027-pt-General-G.ics`, replicada no **poc**, no **site** e no **ESP32**.
- Ignorar memórias facultativas `[m]` e `[m*]`: esses dias continuam com a cor padrão da estação.
- Suprimir `[F]`/`[M]` de data fixa que caem num domingo do Tempo Comum (onde o ICS as omite), mantendo a cor do domingo — fidelidade ao calendário geral.
- Preservar as solenidades móveis derivadas da Páscoa já resolvidas (Ascensão, Pentecostes, Corpus Christi etc.) e completar o suporte quando necessário.
- Adicionar um teste/validação que compara a cor computada com a cor do ICS para todos os dias de 2026 e 2027.

## Capabilities

### New Capabilities
- `cor-liturgica`: Comportamento durável de determinar a cor litúrgica de um dia a partir da estação e das celebrações do Calendário Romano Geral, incluindo data-fixa, precedência e supressão em domingos. Dona das regras de cor que evoluem junto com o projeto.

### Modified Capabilities
Nenhuma (projeto ainda sem specs existentes).

## Impact

- `poc/src/tipos/liturgico.ts` e `poc/src/calendario/calendario-liturgico.ts` (lógica de cores + tabela).
- `poc/web/app.ts` (reflete a cor computada automaticamente; apenas confirmação/ajuste).
- `esp32/ControladorLiturgico/TiposLiturgicos.h` e `esp32/ControladorLiturgico/CalendarioLiturgico.cpp` (tabela e regra de supressão em C++).
- Testes existentes em `poc/tests/` e novo dataset de validação derivado dos ICS.
