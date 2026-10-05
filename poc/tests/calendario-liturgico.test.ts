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

  it('trata o Sábado Santo como vigília pascal específica', () => {
    const dia = calcularDiaLiturgico(new Date(Date.UTC(2026, 3, 4)));
    expect(dia.periodo.nome).toBe('Semana Santa');
    expect(dia.celebracoes[0]?.nome).toBe('Sábado Santo');
    expect(dia.celebracoes[0]?.tipo).toBe(TipoCelebracao.SOLENIDADE);
    expect(dia.cor).toBe(CorLiturgica.VERMELHO);
  });

  it('reconhece Ascensão do Senhor no calendário romano geral', () => {
    const dia = calcularDiaLiturgico(new Date(Date.UTC(2026, 4, 14)));
    expect(dia.periodo.nome).toBe('Tempo Pascal');
    expect(dia.celebracoes[0]?.nome).toBe('Ascensão do Senhor');
    expect(dia.celebracoes[0]?.tipo).toBe(TipoCelebracao.SOLENIDADE);
    expect(dia.cor).toBe(CorLiturgica.BRANCO);
  });

  it('reconhece Pentecostes como celebração específica com a cor vermelha', () => {
    const dia = calcularDiaLiturgico(new Date(Date.UTC(2026, 4, 24)));
    expect(dia.periodo.nome).toBe('Tempo Pascal');
    expect(dia.celebracoes[0]?.nome).toBe('Pentecostes');
    expect(dia.cor).toBe(CorLiturgica.VERMELHO);
  });

  it('reconhece Corpus Christi no calendário romano geral sem perder o período', () => {
    const dia = calcularDiaLiturgico(new Date(Date.UTC(2026, 5, 4)));
    expect(dia.periodo.nome).toBe('Tempo Comum');
    expect(dia.celebracoes[0]?.nome).toBe('Santíssimo Corpo e Sangue de Cristo');
    expect(dia.celebracoes[0]?.tipo).toBe(TipoCelebracao.SOLENIDADE);
    expect(dia.cor).toBe(CorLiturgica.BRANCO);
  });

  it('reconhece domingos como celebrações próprias', () => {
    const dia = calcularDiaLiturgico(new Date(Date.UTC(2026, 4, 17)));
    expect(dia.periodo.nome).toBe('Tempo Pascal');
    expect(dia.celebracoes.some((celebracao) => celebracao.nome === 'Domingo do Tempo Pascal')).toBe(true);
    expect(dia.celebracoes[0]?.tipo).toBe(TipoCelebracao.DOMINGO);
    expect(dia.cor).toBe(CorLiturgica.BRANCO);
  });

  it('mantém 2026-11-28 no Tempo Comum antes do Advento', () => {
    const dia = calcularDiaLiturgico(new Date(Date.UTC(2026, 10, 28)));
    expect(dia.periodo.nome).toBe('Tempo Comum');
    expect(dia.cor).toBe(CorLiturgica.VERDE);
    expect(dia.celebracoes.some((celebracao) => celebracao.nome === 'Advento')).toBe(false);
  });

  it('reconhece a Solenidade de Santa Maria, Mãe de Deus, no Tempo de Natal', () => {
    const dia = calcularDiaLiturgico(new Date(Date.UTC(2027, 0, 1)));
    expect(dia.periodo.nome).toBe('Natal');
    expect(dia.celebracoes[0]?.nome).toBe('Santa Maria, Mãe de Deus');
    expect(dia.cor).toBe(CorLiturgica.BRANCO);
  });

  it('reconhece a Epifania do Senhor no Tempo de Natal', () => {
    const dia = calcularDiaLiturgico(new Date(Date.UTC(2027, 0, 6)));
    expect(dia.periodo.nome).toBe('Natal');
    expect(dia.celebracoes[0]?.nome).toBe('Epifania do Senhor');
    expect(dia.cor).toBe(CorLiturgica.BRANCO);
  });

  it('mantém o período de Natal separado da celebração em 31/12', () => {
    const dia = calcularDiaLiturgico(new Date(Date.UTC(2026, 11, 31)));
    expect(dia.periodo.nome).toBe('Natal');
    expect(dia.celebracoes.some((celebracao) => celebracao.nome === 'Natal do Senhor')).toBe(false);
    expect(dia.celebracoes.some((celebracao) => celebracao.nome === 'Tempo de Natal')).toBe(true);
    expect(dia.cor).toBe(CorLiturgica.BRANCO);
  });

  it('identifica o domingo do Tempo de Natal em 2027-01-03', () => {
    const dia = calcularDiaLiturgico(new Date(Date.UTC(2027, 0, 3)));
    expect(dia.periodo.nome).toBe('Natal');
    expect(dia.celebracoes.some((celebracao) => celebracao.nome === 'Domingo do Tempo de Natal')).toBe(true);
    expect(dia.cor).toBe(CorLiturgica.BRANCO);
  });

  it('identifica as principais festas do Tempo de Natal', () => {
    const datas = [
      { data: new Date(Date.UTC(2026, 11, 26)), nome: 'Santo Estêvão', cor: CorLiturgica.VERMELHO },
      { data: new Date(Date.UTC(2026, 11, 27)), nome: 'São João Evangelista', cor: CorLiturgica.BRANCO },
      { data: new Date(Date.UTC(2026, 11, 28)), nome: 'Santos Inocentes', cor: CorLiturgica.VERMELHO },
      { data: new Date(Date.UTC(2027, 0, 1)), nome: 'Santa Maria, Mãe de Deus', cor: CorLiturgica.BRANCO },
      { data: new Date(Date.UTC(2027, 0, 6)), nome: 'Epifania do Senhor', cor: CorLiturgica.BRANCO },
      { data: new Date(Date.UTC(2027, 0, 10)), nome: 'Batismo do Senhor', cor: CorLiturgica.BRANCO },
    ];

    datas.forEach(({ data, nome, cor }) => {
      const dia = calcularDiaLiturgico(data);
      expect(dia.celebracoes[0]?.nome).toBe(nome);
      expect(dia.cor).toBe(cor);
    });
  });

  it('reconhece festas fixas do calendário romano geral', () => {
    const datas = [
      { data: new Date(Date.UTC(2026, 0, 25)), nome: 'Conversão de São Paulo', cor: CorLiturgica.BRANCO },
      { data: new Date(Date.UTC(2026, 1, 2)), nome: 'Apresentação do Senhor', cor: CorLiturgica.BRANCO },
      { data: new Date(Date.UTC(2026, 1, 22)), nome: 'Cátedra de São Pedro', cor: CorLiturgica.BRANCO },
      { data: new Date(Date.UTC(2026, 5, 24)), nome: 'Natividade de São João Batista', cor: CorLiturgica.BRANCO },
      { data: new Date(Date.UTC(2026, 5, 29)), nome: 'São Pedro e São Paulo', cor: CorLiturgica.VERMELHO },
      { data: new Date(Date.UTC(2026, 7, 15)), nome: 'Assunção da Bem-Aventurada Virgem Maria', cor: CorLiturgica.BRANCO },
      { data: new Date(Date.UTC(2026, 10, 1)), nome: 'Todos os Santos', cor: CorLiturgica.BRANCO },
    ];

    datas.forEach(({ data, nome, cor }) => {
      const dia = calcularDiaLiturgico(data);
      expect(dia.celebracoes[0]?.nome).toBe(nome);
      expect(dia.cor).toBe(cor);
    });
  });

  it('aplica precedência correta em domingo de Quaresma com solenidade', () => {
    const dia = calcularDiaLiturgico(new Date(Date.UTC(2026, 1, 22)));
    expect(dia.celebracoes[0]?.nome).toBe('Cátedra de São Pedro');
    expect(dia.celebracoes[0]?.tipo).toBe(TipoCelebracao.SOLENIDADE);
    expect(dia.cor).toBe(CorLiturgica.BRANCO);
    expect(dia.periodo.nome).toBe('Quaresma');
  });

  it('aplica precedência correta em festividade dentro do Tempo Pascal', () => {
    const dia = calcularDiaLiturgico(new Date(Date.UTC(2026, 4, 24)));
    expect(dia.celebracoes[0]?.nome).toBe('Pentecostes');
    expect(dia.celebracoes[0]?.tipo).toBe(TipoCelebracao.SOLENIDADE);
    expect(dia.cor).toBe(CorLiturgica.VERMELHO);
  });

  it('aplica precedência correta em celebração específica dentro do Tempo de Natal', () => {
    const dia = calcularDiaLiturgico(new Date(Date.UTC(2027, 0, 6)));
    expect(dia.periodo.nome).toBe('Natal');
    expect(dia.celebracoes[0]?.nome).toBe('Epifania do Senhor');
    expect(dia.cor).toBe(CorLiturgica.BRANCO);
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
