import { describe, expect, it } from 'vitest';
import { calcularCorDoDia, calcularDiaLiturgico } from '../src/calendario/calendario-liturgico.js';
import { CorLiturgica, TipoCelebracao } from '../src/tipos/liturgico.js';

describe('Calendário litúrgico', () => {
  it('define a cor da Quarta-feira de Cinzas', () => {
    const cor = calcularCorDoDia(new Date(Date.UTC(2026, 1, 18)));
    expect(cor).toBe(CorLiturgica.VIOLETA);
  });

  it('define a cor do Domingo de Ramos', () => {
    const cor = calcularCorDoDia(new Date(Date.UTC(2026, 2, 29)));
    expect(cor).toBe(CorLiturgica.VERMELHO);
  });

  it('define a cor da Quinta-feira Santa', () => {
    const cor = calcularCorDoDia(new Date(Date.UTC(2026, 3, 2)));
    expect(cor).toBe(CorLiturgica.BRANCO);
  });

  it('define a cor da Sexta-feira Santa', () => {
    const cor = calcularCorDoDia(new Date(Date.UTC(2026, 3, 3)));
    expect(cor).toBe(CorLiturgica.VERMELHO);
  });

  it('define a cor do Domingo de Páscoa', () => {
    const cor = calcularCorDoDia(new Date(Date.UTC(2026, 3, 5)));
    expect(cor).toBe(CorLiturgica.BRANCO);
  });

  it('define a cor do Natal', () => {
    const cor = calcularCorDoDia(new Date(Date.UTC(2026, 11, 25)));
    expect(cor).toBe(CorLiturgica.BRANCO);
  });

  it('produz um dia litúrgico com celebração e precedência', () => {
    const dia = calcularDiaLiturgico(new Date(Date.UTC(2026, 3, 5)));
    expect(dia.periodo.nome).toBe('Páscoa');
    expect(dia.celebracoes.length).toBeGreaterThan(0);
    expect(dia.celebracoes[0]?.tipo).toBe(TipoCelebracao.SOLENIDADE);
    expect(dia.cor).toBe(CorLiturgica.BRANCO);
  });
});
