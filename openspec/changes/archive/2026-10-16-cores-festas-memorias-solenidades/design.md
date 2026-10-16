# Design

## Context

Ver `proposal.md - Why`. Hoje `poc/src/calendario/calendario-liturgico.ts` e `esp32/ControladorLiturgico/CalendarioLiturgico.cpp` resolvem a cor a partir da estação (`periodos.ts` / `obterPeriodoLiturgico`) e de uma lista curta (~20) de celebrações fixas. O `poc/web/app.ts` só renderiza `DiaLiturgico.cor`, então herda a lógica do poc — não precisa de outra fonte de cores.

As 4 cores do Calendário Romano Geral nos ICS (⚪ branco, 🔴 vermelho, 🟢 verde, 🟣 violeta) já existem nos enums de ambos (`CorLiturgica`). Não há arquivos de `specs/` ainda — este é o primeiro capability.

Fonte de referência: `calendario/2026-pt-General-G.ics` e `calendario/2027-pt-General-G.ics`, já presentes no repo. Extraí a lista canônica de `[S]`/`[F]`/`[M]` (verificado): ~150 por ano, quase todas de data fixa, exceto as derivadas da Páscoa.

## Goals / Non-Goals

**Goals:**
- Uma única fonte de dados (tabela de celebrações) espelhada no poc (TS), site (via poc) e ESP32 (C++), determinando a cor de `[S]`/`[F]`/`[M]` de data fixa.
- Regra de supressão de festa/memória que cai num domingo do Tempo Comum.
- Validação automatizada: cor computada == cor do ICS para todos os dias de 2026 e 2027.

**Non-Goals:**
- Calendários próprios/diocesanos (só Romano Geral).
- Cor rosa (Gaudete/Laetare) — não aparece como colorização no ICS geral; fica fora.
- Refatorar `periodos.ts` / precedência geral — apenas estender.
- Ler/parsear ICS em runtime no dispositivo (a tabela é gerada fora e embutida).

## Decisions

### D1 — Tabela embutida de data fixa como fonte de dados
Representar cada celebração de data fixa como uma entrada `(nome, mês, dia, tipo, cor)`. A cor do enum já cobre branco/vermelho/verde/violeta. As solenidades móveis derivadas da Páscoa continuam computadas; no calendário "General-G" Ascensão e Corpus Christi são transferidos para domingo (Páscoa +42 e +63, respectivamente, não +39/+60), e o bloco pascal também inclui Oitava da Páscoa, Mãe da Igreja, Santíssima Trindade, Sagrado Coração, Imaculado Coração, Cristo Rei e Sagrada Família.
- **Alternativa rejeitada:** parsear os `.ics` em runtime — inviável no ESP32 sem filesystem e não resolve anos futuros.
- **Alternativa rejeitada:** tempolar regra por tags `[m]` em vez de tabela — a tabela é o caminho que reproduz o ICS por data.

### D2 — Supressão de festa/memória de data fixa em domingo
Ao coletar celebrações de data fixa, se o dia for domingo **e** a celebração for `FESTA` ou `MEMORIA` (data fixa), a celebração é descartada (o dia fica com a cor do domingo da estação). Solenidades não são suprimidas. A supressão vale para qualquer domingo — inclusive domingos de Quaresma (ex.: Cátedra de São Pedro suprimida em 22/fev/2026, domingo I da Quaresma) —, batendo com o ICS. A regra só se aplica a celebrações de data fixa, não às móveis (ex.: Batismo do Senhor é festa celebrada em domingo).
- **Alternativa rejeitada:** suprimir em qualquer domingo de forma cega — quebraria celebrações móveis de domingo (Batismo/Sagrada Família) e solenidades.
- **Alternativa rejeitada:** limitar a supressão só ao Tempo Comum — falharia a supressão de festas fixas em domingos de Quaresma observada no ICS.

### D3 — Espelhamento entre poc e ESP32 sem gerador
O poc (TS) é a fonte; o ESP32 (C++) tem um espelho manual da mesma tabela (array de structs) registrado no mesmo padrão de `adicionarCelebracao`. Sem pipeline de geração entre as duas linguagens, o espelho é mantido por convenção e a assinatura é validada pelo teste que compara cor computada contra o ICS.
- **Alternativa rejeitada:** gerar o `.h` a partir do TS no build — não há toolchain comum; risco de quebrar o fluxo Arduino.

### D4 — Validação contra o ICS como critério de aceite
Um teste lê os dois `.ics` (fixtures já no repo em `calendario/`) e, para cada dia `[S]`/`[F]`/`[M]`, calcagna `calcularCorDoDia(data)` e compara com a cor do summary. Isso força fidelidade às transferências/supressões reais de 2026/2027 (ex.: `Conversão de São Paulo` ausente em 2026 porque caiu num domingo; `Anunciação` transferida para 5/abr/2027 após a Semana Santa).

## Risks / Trade-offs

- [Transferências/supressões de solenidades móveis e exceções anuais (Anunciação, São José) são não-triviais] → mitigado pelo teste D4 que valida todos os dias de 2026/2027; regras específicas só são adicionadas se o teste apontar divergência.
- [Drift entre tabela TS e espelho C++] → espelho revisado no mesmo PR e coberto pelo teste de validação do lado do poc; documentado em comentário apontando a tabela-fonte.
- [Crescimento da lista (~150 entradas) em C++ e flash] → tabela compacta (dia/mês/tipo/cor), bem dentro do orçamento de memória do ESP32.
- [ICS de referência só tem 2026/2027] → capítulo de validação cobre esses anos; regras são gerais (data-fixa + supressão), não hardcode por ano, salvo as transferências que o teste obriga.

## Migration Plan

- Evolução incremental no poc com teste primeiro (red→green sobre o ICS), depois espelho no ESP32 e confirmação do site.
- Sem migração de dados persistente (cores são computadas em runtime).
- Rollback: reverter o commit restaura a lista curta anterior; o site e o ESP32 voltam a compilar com a lógica antiga.
