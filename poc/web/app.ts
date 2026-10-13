import { calcularDiaLiturgico } from '../src/calendario/calendario-liturgico.js';
import { adicionarDias, dataUtc } from '../src/calendario/regras.js';
import type { DiaLiturgico } from '../src/tipos/liturgico.js';

interface InfoCor {
  nome: string;
  fundo: string;
  texto: string;
}

const CORES: Record<string, InfoCor> = {
  violeta: { nome: 'Violeta', fundo: '#6d28d9', texto: '#ffffff' },
  branco: { nome: 'Branco', fundo: '#f8fafc', texto: '#1f2937' },
  vermelho: { nome: 'Vermelho', fundo: '#b91c1c', texto: '#ffffff' },
  verde: { nome: 'Verde', fundo: '#15803d', texto: '#ffffff' },
  rosa: { nome: 'Rosa', fundo: '#db2777', texto: '#ffffff' },
};

const NOMES_DIAS = [
  'domingo',
  'segunda-feira',
  'terça-feira',
  'quarta-feira',
  'quinta-feira',
  'sexta-feira',
  'sábado',
];

const TOTAL_DIAS = 31;

function infoCor(cor: string): InfoCor {
  return CORES[cor] ?? { nome: cor, fundo: '#334155', texto: '#ffffff' };
}

function dataDeHoje(): Date {
  const agora = new Date();
  return dataUtc(agora.getFullYear(), agora.getMonth(), agora.getDate());
}

function formatarData(data: Date): string {
  const dia = String(data.getUTCDate()).padStart(2, '0');
  const mes = String(data.getUTCMonth() + 1).padStart(2, '0');
  return `${dia}/${mes}`;
}

function nomeDiaSemana(data: Date): string {
  return NOMES_DIAS[data.getUTCDay()] ?? '';
}

function criar<K extends keyof HTMLElementTagNameMap>(
  tag: K,
  classe?: string,
  texto?: string,
): HTMLElementTagNameMap[K] {
  const elemento = document.createElement(tag);
  if (classe) elemento.className = classe;
  if (texto !== undefined) elemento.textContent = texto;
  return elemento;
}

function renderizarDestaque(dia: DiaLiturgico): void {
  const container = document.getElementById('hoje');
  if (!container) return;

  const info = infoCor(dia.cor);
  container.style.setProperty('--cor-fundo', info.fundo);
  container.style.setProperty('--cor-texto', info.texto);
  container.replaceChildren();

  const data = criar(
    'p',
    'hero__data',
    `${formatarData(dia.data)} · ${nomeDiaSemana(dia.data)}`,
  );
  const nome = criar('p', 'hero__cor', info.nome);
  const periodo = criar('p', 'hero__periodo', dia.periodo.nome);

  const lista = criar('ul', 'hero__celebracoes');
  for (const celebracao of dia.celebracoes) {
    const item = criar('li', 'hero__celebracao', celebracao.nome);
    item.dataset.tipo = celebracao.tipo;
    lista.append(item);
  }

  container.append(data, nome, periodo, lista);
}

function renderizarDia(dia: DiaLiturgico, indice: number): HTMLLIElement {
  const info = infoCor(dia.cor);
  const item = criar('li', 'dia');
  if (indice === 0) item.classList.add('dia--hoje');

  const faixa = criar('span', 'dia__faixa');
  faixa.style.backgroundColor = info.fundo;

  const blocoInfo = criar('div', 'dia__info');
  blocoInfo.append(
    criar('span', 'dia__data', `${formatarData(dia.data)} · ${nomeDiaSemana(dia.data)}`),
    criar('span', 'dia__periodo', dia.periodo.nome),
  );

  const blocoCor = criar('div', 'dia__cor');
  const swatch = criar('span', 'dia__swatch');
  swatch.style.backgroundColor = info.fundo;
  blocoCor.append(swatch, criar('span', 'dia__nome-cor', info.nome));

  const principal = dia.celebracoes[0];
  const celebracao = criar('span', 'dia__celebracao', principal?.nome ?? '');

  item.append(faixa, blocoInfo, blocoCor, celebracao);
  return item;
}

function iniciar(): void {
  const hoje = dataDeHoje();
  const lista = document.getElementById('dias');
  lista?.replaceChildren();

  for (let i = 0; i < TOTAL_DIAS; i++) {
    const dia = calcularDiaLiturgico(adicionarDias(hoje, i));
    if (i === 0) renderizarDestaque(dia);
    lista?.append(renderizarDia(dia, i));
  }
}

iniciar();
