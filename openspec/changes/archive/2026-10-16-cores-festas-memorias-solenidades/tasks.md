# Tasks

## 1. Baseline de validação contra o ICS

- [x] 1.1 Criar um teste em `poc/tests/` que lê `calendario/2026-pt-General-G.ics` e `calendario/2027-pt-General-G.ics`, extrai para cada dia `[S]`/`[F]`/`[M]` a data e a cor do summary, e verificar que o parser captura o conjunto completo (contagem por ano ~[S]+[F]+[M] e presença das 4 cores ⚪🟢🔴🟣)
- [x] 1.2 Adicionar teste que, para todos os dias de 2026 e 2027, compara `calcularCorDoDia(utc(data))` com a cor esperada do ICS e verificar que ele falha hoje (red), registrando a lista de divergências como baseline do estado atual

## 2. Tabela e regras no poc (TypeScript)

- [x] 2.1 Adicionar a tabela canônica de celebrações de data fixa (nome, mês, dia, tipo, cor) a `poc/src/calendario/calendario-liturgico.ts`, extraída dos ICS, e verificar (via `openspec`/review) que contém todos os `[S]`/`[F]`/`[M]` de data fixa
- [x] 2.2 Adicionar as celebrações móveis do bloco pascal computadas a partir da Páscoa (Oitava da Páscoa, Mãe da Igreja, Santíssima Trindade, Sagrado Coração e Imaculado Coração) e verificar que aparecem como solenidade/memória correta nos dias do ICS 2026/2027
- [x] 2.3 Implementar a regra de supressão de festa/memória em domingo do Tempo Comum em `calcularDiaLiturgico` e verificar que `Conversão de São Paulo` (jan/2026, domingo) fica verde enquanto (jan/2027, segunda) fica branca
- [x] 2.4 Rodar o teste do grupo 1 e verificar que todas as cores de 2026/2027 passam (green), ajustando com regras específicas apenas onde o teste apontar transferência/supressão dependentas do ano (ex.: Anunciação)

## 3. Site (poc/web)

- [x] 3.1 Confirmar que `poc/web/app.ts` renderiza `DiaLiturgico.cor` e que o mapa `CORES` cobre branco/vermelho/verde/violeta, e verificar que o build (`npm run build` em `poc/`) passa após as mudanças do grupo 2
- [x] 3.2 Abrir o site com a data de hoje e conferir visualmente que dias com memória/festa/solenidade exibem a faixa e o nome de cor corretos

## 4. ESP32 (C++)

- [x] 4.1 Adicionar em `esp32/ControladorLiturgico/CalendarioLiturgico.cpp` a tabela espelho de data fixa (mês, dia, nome, tipo, cor) e a regra de supressão em domingo/Tempo Comum em `obterDiaLiturgico`, espelhando a tabela do poc
- [x] 4.2 Adicionar no C++ as celebrações móveis do bloco pascal equivalentes ao grupo 2.2
- [x] 4.3 Registrar num comentário de código a correspondência com a tabela-fonte do poc e verificar que o sketch compila sem erros (compilação Arduino do `ControladorLiturgico.ino`)

## 5. Integração e confirmação

- [x] 5.1 Rodar a suíte completa de testes do poc (`npm test` em `poc/`) e o build, e verificar que todos passam e cobrem as 4 cores
- [x] 5.2 Revisar lado a lado as tabelas TS e C++ confrontando com o mesmo dataset dos ICS e verificar que coincidem em (dia/mês, cor) para 2026/2027

## Workflow follow-up

- Após revisão, arquivar o change com `openspec archive`.
- Verificar o resultado arquivado (spec principal de `cor-liturgica` criado).
