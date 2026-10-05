import { describe, expect, it } from 'vitest';
import { determinarPeriodoLiturgico } from '../src/calendario/periodos.js';
import { CorLiturgica } from '../src/tipos/liturgico.js';

describe('Períodos litúrgicos', () => {
  it('reconhece Advento', () => {
    const periodo = determinarPeriodoLiturgico(new Date(Date.UTC(2026, 10, 29))); // 29/11/2026
    expect(periodo.nome).toBe('Advento');
  });

  it('reconhece Quaresma', () => {
    const periodo = determinarPeriodoLiturgico(new Date(Date.UTC(2026, 1, 18))); // 18/02/2026
    expect(periodo.nome).toBe('Quaresma');
  });

  it('reconhece Tempo Comum', () => {
    const periodo = determinarPeriodoLiturgico(new Date(Date.UTC(2026, 7, 15))); // 15/08/2026
    expect(periodo.nome).toBe('Tempo Comum');
  });

  it('contem a cor base do período', () => {
    const periodo = determinarPeriodoLiturgico(new Date(Date.UTC(2026, 7, 15)));
    expect(periodo.corPadrao).toBe(CorLiturgica.VERDE);
  });
});
