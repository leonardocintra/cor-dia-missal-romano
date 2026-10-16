import { describe, expect, it } from 'vitest';
import { calcularCorDoDia } from '../src/calendario/calendario-liturgico.js';
import { CorLiturgica } from '../src/tipos/liturgico.js';
import { coresPorDia, parsearIcs } from './fixtures/ics.js';

const CAMINHO_ICS = {
  2026: new URL('../../calendario/2026-pt-General-G.ics', import.meta.url).pathname,
  2027: new URL('../../calendario/2027-pt-General-G.ics', import.meta.url).pathname,
} as const;

describe('Parsing dos ICS do Calendário Romano Geral', () => {
  it('extrai o conjunto completo de solenidades, festas e memórias obrigatórias', () => {
    const eventos = parsearIcs(CAMINHO_ICS[2026]).concat(parsearIcs(CAMINHO_ICS[2027]));
    const porAno = { 2026: 0, 2027: 0 };

    for (const evento of eventos) {
      const tag = evento.tag;
      if (tag === 'S' || tag === 'F' || tag === 'M') {
        porAno[evento.data.getUTCFullYear() as 2026 | 2027]++;
      }
    }

    expect(porAno[2026]).toBe(97);
    expect(porAno[2027]).toBe(96);
  });

  it('cobre apenas as quatro cores litúrgicas do calendário geral', () => {
    const cores = new Set<CorLiturgica>();
    for (const ano of [2026, 2027]) {
      for (const evento of parsearIcs(CAMINHO_ICS[ano])) {
        cores.add(evento.cor);
      }
    }

    expect(cores).toEqual(new Set([
      CorLiturgica.BRANCO,
      CorLiturgica.VERMELHO,
      CorLiturgica.VERDE,
      CorLiturgica.VIOLETA,
    ]));
  });
});

describe('Cor computada coincide com o ICS para solenidades, festas e memórias', () => {
  for (const ano of [2026, 2027]) {
    it(`compara todas as S/F/M de ${ano} com a cor do ICS`, () => {
      // Baseline (red) antes da implementação: 74 divergências em 2026 e 71 em 2027 —
      // a maioria de dias de memória/festa/solenidade que ficam com a cor da estação.
      const esperado = coresPorDia(parsearIcs(CAMINHO_ICS[ano]));
      const divergencias: string[] = [];

      for (const [dataIso, cor] of esperado) {
        const [y, m, d] = dataIso.split('-').map(Number);
        const obtida = calcularCorDoDia(new Date(Date.UTC(y, m - 1, d)));
        if (obtida !== cor) {
          divergencias.push(`${dataIso}: esperado ${cor}, obtido ${obtida}`);
        }
      }

      expect(divergencias, divergencias.join('\n')).toEqual([]);
    });
  }
});
