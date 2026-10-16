import { CorLiturgica, TipoCelebracao } from '../tipos/liturgico.js';
import type { CelebracaoLiturgica, DiaLiturgico } from '../tipos/liturgico.js';
import { determinarPeriodoLiturgico, calcularDomingoAdvento } from './periodos.js';
import { adicionarDias, calcularPascoa } from './pascoa.js';
import { dataUtc } from './regras.js';

type CelebracaoDefinicao = {
  nome: string;
  tipo: TipoCelebracao;
  cor: CorLiturgica;
  dataBase: Date;
  dataEfetiva?: Date;
  calendario: 'romano-geral';
  dataFixa?: boolean;
};

type CelebracaoFixa = {
  nome: string;
  tipo: TipoCelebracao;
  cor: CorLiturgica;
  mes: number;
  dia: number;
};

const precedenciaPorTipo: Record<TipoCelebracao, number> = {
  [TipoCelebracao.SOLENIDADE]: 5,
  [TipoCelebracao.FESTA]: 4,
  [TipoCelebracao.DOMINGO]: 3,
  [TipoCelebracao.MEMORIA]: 2,
  [TipoCelebracao.SEMANA]: 1,
};

// Celebrações de data fixa do Calendário Romano Geral (fonte: calendario/20xx-pt-General-G.ics),
// conforme a proposta `cores-festas-memorias-solenidades`. Apenas solenidades, festas e memórias
// obrigatórias; as comemorações facultativas [m]/[m*] são ignoradas e mantêm a cor da estação.
const CELEBRACOES_FIXAS: CelebracaoFixa[] = [
  { nome: 'Santa Maria, Mãe de Deus', mes: 1, dia: 1, tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO },
  { nome: 'Santos Basilio Magno e Gregório de Nazianzo, bispos e doutores da Igreja', mes: 1, dia: 2, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Epifania do Senhor', mes: 1, dia: 6, tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO },
  { nome: 'Santo Antão, abade', mes: 1, dia: 17, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Santa Inês, virgem e mártir', mes: 1, dia: 21, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.VERMELHO },
  { nome: 'São Francisco de Sales, bispo e doutor da Igreja', mes: 1, dia: 24, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Conversão de São Paulo, apóstolo', mes: 1, dia: 25, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.BRANCO },
  { nome: 'Santos Timóteo e Tito, bispos', mes: 1, dia: 26, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'São Tomás de Aquino, presbítero e doutor da Igreja', mes: 1, dia: 28, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'São João Bosco, presbítero', mes: 1, dia: 31, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Apresentação do Senhor', mes: 2, dia: 2, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.BRANCO },
  { nome: 'Santa Águeda, virgem e mártir', mes: 2, dia: 5, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.VERMELHO },
  { nome: 'Santos Paulo Miki e Companheiros, mártires', mes: 2, dia: 6, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.VERMELHO },
  { nome: 'Santa Escolástica, virgem', mes: 2, dia: 10, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Santos Cirilo, monge, e Metódio, bispo', mes: 2, dia: 14, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Cadeira de São Pedro, apóstolo', mes: 2, dia: 22, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.BRANCO },
  { nome: 'São José, esposo da Virgem Santa Maria', mes: 3, dia: 19, tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO },
  { nome: 'Anunciação do Senhor', mes: 3, dia: 25, tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO },
  { nome: 'São João Batista de La Salle, presbítero', mes: 4, dia: 7, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'São Marcos, evangelista', mes: 4, dia: 25, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.VERMELHO },
  { nome: 'Santa Catarina de Siena, virgem e doutor da Igreja', mes: 4, dia: 29, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Santo Atanasio, bispo e doutor da Igreja', mes: 5, dia: 2, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Santos Filipe e Tiago, apóstolos', mes: 5, dia: 3, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.VERMELHO },
  { nome: 'São Matias, apóstolo', mes: 5, dia: 14, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.VERMELHO },
  { nome: 'São Filipe Néri, presbítero', mes: 5, dia: 26, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Visitação da Nossa Senhora', mes: 5, dia: 31, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.BRANCO },
  { nome: 'São Justino, mártir', mes: 6, dia: 1, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.VERMELHO },
  { nome: 'Santos Carlos Lwanga e Companheiros, mártires', mes: 6, dia: 3, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.VERMELHO },
  { nome: 'São Bonifácio, bispo e mártir', mes: 6, dia: 5, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.VERMELHO },
  { nome: 'São Barnabé, apóstolo', mes: 6, dia: 11, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.VERMELHO },
  { nome: 'São Luís Gonzaga, religioso', mes: 6, dia: 21, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Nascimento de São João Batista', mes: 6, dia: 24, tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO },
  { nome: 'Santo Ireneu, bispo, mártir e doutor da Igreja', mes: 6, dia: 28, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.VERMELHO },
  { nome: 'Santos Pedro e Paulo, apóstolos', mes: 6, dia: 29, tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.VERMELHO },
  { nome: 'São Tomé, apóstolo', mes: 7, dia: 3, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.VERMELHO },
  { nome: 'São Bento, abade', mes: 7, dia: 11, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'São Boaventura, bispo e doutor da Igreja', mes: 7, dia: 15, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Santa Maria Madalena', mes: 7, dia: 22, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.BRANCO },
  { nome: 'São Tiago, apóstolo', mes: 7, dia: 25, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.VERMELHO },
  { nome: 'Santos Joaquim e Ana, pais da Virgem Santa Maria', mes: 7, dia: 26, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Santos Marta, Maria e Lázaro', mes: 7, dia: 29, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Santo Inácio de Loiola, presbítero', mes: 7, dia: 31, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Santo Afonso de Ligório, bispo e doutor da Igreja', mes: 8, dia: 1, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'São João Maria Vianney, presbítero', mes: 8, dia: 4, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Transfiguração do Senhor', mes: 8, dia: 6, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.BRANCO },
  { nome: 'São Domingos, presbítero', mes: 8, dia: 8, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'São Lourenço, diácono e mártir', mes: 8, dia: 10, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.VERMELHO },
  { nome: 'Santa Clara, virgem', mes: 8, dia: 11, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'São Maximiliano Maria Kolbe, presbitero e mártir', mes: 8, dia: 14, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.VERMELHO },
  { nome: 'Assunção da Virgem Santa Maria', mes: 8, dia: 15, tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO },
  { nome: 'São Bernardo, abade e doutor da Igreja', mes: 8, dia: 20, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'São Pio X, papa', mes: 8, dia: 21, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Virgem Santa Maria, Rainha', mes: 8, dia: 22, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'São Bartolomeu, apóstolo', mes: 8, dia: 24, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.VERMELHO },
  { nome: 'Santa Mónica', mes: 8, dia: 27, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Santo Agostinho, bispo e doutor da Igreja', mes: 8, dia: 28, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Martírio de São João Batista', mes: 8, dia: 29, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.VERMELHO },
  { nome: 'São Gregorio Magno, papa e doutor da Igreja', mes: 9, dia: 3, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Natividade da Virgem Santa Maria', mes: 9, dia: 8, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.BRANCO },
  { nome: 'São João Crisóstomo, bispo e doutor da Igreja', mes: 9, dia: 13, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Exaltação da Santa Cruz', mes: 9, dia: 14, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.VERMELHO },
  { nome: 'Nossa Senhora das Dores', mes: 9, dia: 15, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Santos Cornélio, papa, e Cipriano, bispo, mártires', mes: 9, dia: 16, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.VERMELHO },
  { nome: 'Santos André Kim Taegon, presbítero, Paulo Chong Hasang e Companheiros, mártires', mes: 9, dia: 20, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.VERMELHO },
  { nome: 'São Mateus, apóstolo e evangelista', mes: 9, dia: 21, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.VERMELHO },
  { nome: 'São Pio de Pietrelcina, presbítero', mes: 9, dia: 23, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'São Vicente de Paulo, presbítero', mes: 9, dia: 27, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Santos Miguel, Gabriel e Rafael, Arcanjos', mes: 9, dia: 29, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.BRANCO },
  { nome: 'São Jerônimo, presbítero e doutor da Igreja', mes: 9, dia: 30, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Santa Teresa do Menino Jesus, virgem e doutor da Igreja', mes: 10, dia: 1, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Santos Anjos da Guarda', mes: 10, dia: 2, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'São Francisco de Assis, religioso', mes: 10, dia: 4, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Nossa Senhora do Rosário', mes: 10, dia: 7, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Santa Teresa de Jesus, virgem e doutor da Igreja', mes: 10, dia: 15, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Santo Inácio de Antioquia, bispo e mártir', mes: 10, dia: 17, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.VERMELHO },
  { nome: 'São Lucas, evangelista', mes: 10, dia: 18, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.VERMELHO },
  { nome: 'Santos Simão e Judas, apóstolos', mes: 10, dia: 28, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.VERMELHO },
  { nome: 'Todos os Santos', mes: 11, dia: 1, tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO },
  { nome: 'São Carlos Borromeu, bispo', mes: 11, dia: 4, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Dedicação da Basílica de Latrão', mes: 11, dia: 9, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.BRANCO },
  { nome: 'São Leão Magno, papa e doutor da Igreja', mes: 11, dia: 10, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'São Martinho de Tours, bispo', mes: 11, dia: 11, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'São Josafá, bispo e mártir', mes: 11, dia: 12, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.VERMELHO },
  { nome: 'Santa Isabel da Hungría, religiosa', mes: 11, dia: 17, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Apresentação de Nossa Senhora', mes: 11, dia: 21, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Santa Cecília, virgem e mártir', mes: 11, dia: 22, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.VERMELHO },
  { nome: 'Santos André Dũng Lạc, presbítero, e Companheiros, mártires', mes: 11, dia: 24, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.VERMELHO },
  { nome: 'Santo André, apóstolo', mes: 11, dia: 30, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.VERMELHO },
  { nome: 'São Francisco Xavier, presbítero', mes: 12, dia: 3, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Santo Ambrósio, bispo e doutor da Igreja', mes: 12, dia: 7, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Imaculada Conceição da Virgem Santa Maria', mes: 12, dia: 8, tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO },
  { nome: 'Santa Luzia, virgem e mártir', mes: 12, dia: 13, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.VERMELHO },
  { nome: 'São João da Cruz, presbítero e doutor da Igreja', mes: 12, dia: 14, tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO },
  { nome: 'Natal do Senhor', mes: 12, dia: 25, tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO },
  { nome: 'Santo Estêvão, primeiro mártir', mes: 12, dia: 26, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.VERMELHO },
  { nome: 'São João, apóstolo e evangelista', mes: 12, dia: 27, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.BRANCO },
  { nome: 'Santos Inocentes, mártires', mes: 12, dia: 28, tipo: TipoCelebracao.FESTA, cor: CorLiturgica.VERMELHO },
];

export function criarCelebracao(nome: string, tipo: TipoCelebracao, cor?: CorLiturgica): CelebracaoLiturgica {
  return {
    nome,
    tipo,
    ...(cor !== undefined ? { cor } : {}),
  };
}

function nomeDoDomingo(periodo: { nome: string }): string {
  switch (periodo.nome) {
    case 'Advento':
      return 'Domingo do Advento';
    case 'Quaresma':
      return 'Domingo da Quaresma';
    case 'Semana Santa':
      return 'Domingo da Semana Santa';
    case 'Páscoa':
      return 'Domingo de Páscoa';
    case 'Tempo Pascal':
      return 'Domingo do Tempo Pascal';
    case 'Natal':
      return 'Domingo do Tempo de Natal';
    case 'Tempo Comum':
      return 'Domingo do Tempo Comum';
    default:
      return 'Domingo';
  }
}

const NOMES_OITAVA_PASCOA = [
  'Segunda-Feira na Oitava da Páscoa',
  'Terça-Feira na Oitava da Páscoa',
  'Quarta-Feira na Oitava da Páscoa',
  'Quinta-Feira na Oitava da Páscoa',
  'Sexta-Feira na Oitava da Páscoa',
  'Sábado na Oitava da Páscoa',
];

// Domingo dentro da oitava do Natal; se o Natal cair num domingo, a festa vai para 30/12.
function calcularSagradaFamilia(ano: number): Date {
  const natal = dataUtc(ano, 11, 25);
  const proximoDomingo = adicionarDias(natal, (7 - natal.getUTCDay()) % 7);
  if (proximoDomingo.getTime() === natal.getTime()) {
    return dataUtc(ano, 11, 30);
  }
  return proximoDomingo;
}

function construirCelebracoesMoviveis(ano: number): CelebracaoDefinicao[] {
  const pascoa = calcularPascoa(ano);
  const epifania = dataUtc(ano, 0, 6);
  const batismoDoSenhor = adicionarDias(epifania, (7 - epifania.getUTCDay()) % 7);

  const moviveis: CelebracaoDefinicao[] = [
    { nome: 'Quarta-feira de Cinzas', tipo: TipoCelebracao.SEMANA, cor: CorLiturgica.VIOLETA, dataBase: adicionarDias(pascoa, -46), calendario: 'romano-geral' },
    { nome: 'Domingo de Ramos', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.VERMELHO, dataBase: adicionarDias(pascoa, -7), calendario: 'romano-geral' },
    { nome: 'Quinta-feira Santa', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO, dataBase: adicionarDias(pascoa, -3), calendario: 'romano-geral' },
    { nome: 'Sexta-feira Santa', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.VERMELHO, dataBase: adicionarDias(pascoa, -2), calendario: 'romano-geral' },
    { nome: 'Sábado Santo', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.VERMELHO, dataBase: adicionarDias(pascoa, -1), calendario: 'romano-geral' },
    { nome: 'Domingo de Páscoa', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO, dataBase: pascoa, calendario: 'romano-geral' },
    { nome: 'Ascensão do Senhor', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO, dataBase: adicionarDias(pascoa, 42), calendario: 'romano-geral' },
    { nome: 'Pentecostes', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.VERMELHO, dataBase: adicionarDias(pascoa, 49), calendario: 'romano-geral' },
    { nome: 'Santíssima Trindade', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO, dataBase: adicionarDias(pascoa, 56), calendario: 'romano-geral' },
    { nome: 'Santíssimo Corpo e Sangue de Cristo', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO, dataBase: adicionarDias(pascoa, 63), calendario: 'romano-geral' },
    { nome: 'Sagrado Coração de Jesus', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO, dataBase: adicionarDias(pascoa, 68), calendario: 'romano-geral' },
    { nome: 'Nosso Senhor Jesus Cristo, Rei do Universo', tipo: TipoCelebracao.SOLENIDADE, cor: CorLiturgica.BRANCO, dataBase: adicionarDias(calcularDomingoAdvento(ano), -7), calendario: 'romano-geral' },
    { nome: 'Sagrada Família de Jesus, Maria e José', tipo: TipoCelebracao.FESTA, cor: CorLiturgica.BRANCO, dataBase: calcularSagradaFamilia(ano), calendario: 'romano-geral' },
  ];

  for (let dia = 1; dia <= 6; dia++) {
    moviveis.push({
      nome: NOMES_OITAVA_PASCOA[dia - 1]!,
      tipo: TipoCelebracao.SOLENIDADE,
      cor: CorLiturgica.BRANCO,
      dataBase: adicionarDias(pascoa, dia),
      calendario: 'romano-geral',
    });
  }

  return [...moviveis, { nome: 'Batismo do Senhor', tipo: TipoCelebracao.FESTA, cor: CorLiturgica.BRANCO, dataBase: batismoDoSenhor, calendario: 'romano-geral' }];
}

function construirCelebracoesDefinidas(ano: number): CelebracaoDefinicao[] {
  const pascoa = calcularPascoa(ano);
  const celebracoesMoviveis = construirCelebracoesMoviveis(ano);

  // Memórias obrigatórias móveis do bloco pascal, derivadas da Páscoa.
  const moviveisPascais: CelebracaoDefinicao[] = [
    { nome: 'Bem-aventurada Virgem Maria, Mãe da Igreja', tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO, dataBase: adicionarDias(pascoa, 50), calendario: 'romano-geral' },
    { nome: 'Imaculado Coração da Virgem Santa Maria', tipo: TipoCelebracao.MEMORIA, cor: CorLiturgica.BRANCO, dataBase: adicionarDias(pascoa, 69), calendario: 'romano-geral' },
  ];

  const porDataFixa: CelebracaoDefinicao[] = CELEBRACOES_FIXAS.map((celebracao) => ({
    nome: celebracao.nome,
    tipo: celebracao.tipo,
    cor: celebracao.cor,
    dataBase: dataUtc(ano, celebracao.mes - 1, celebracao.dia),
    calendario: 'romano-geral' as const,
    dataFixa: true,
  }));

  return [...celebracoesMoviveis, ...moviveisPascais, ...porDataFixa];
}

export function calcularDiaLiturgico(data: Date): DiaLiturgico {
  const periodo = determinarPeriodoLiturgico(data);
  const ano = data.getUTCFullYear();
  const celebracoesAplicaveis: CelebracaoLiturgica[] = [];

  const definicoes = construirCelebracoesDefinidas(ano);

  const ehDomingo = data.getUTCDay() === 0;

  for (const definicao of definicoes) {
    const dataEfetiva = definicao.dataEfetiva ?? definicao.dataBase;
    if (dataEfetiva.getTime() !== data.getTime()) continue;

    // Supressão: festa ou memória de DATA FIXA que cai num domingo é omitida
    // (o dia fica com a cor do domingo da estação), conforme o Calendário Romano Geral.
    if (ehDomingo && definicao.dataFixa
      && (definicao.tipo === TipoCelebracao.FESTA || definicao.tipo === TipoCelebracao.MEMORIA)) {
      continue;
    }

    celebracoesAplicaveis.push(criarCelebracao(definicao.nome, definicao.tipo, definicao.cor));
  }

  if (ehDomingo && periodo.nome !== 'Semana Santa') {
    celebracoesAplicaveis.push(criarCelebracao(nomeDoDomingo(periodo), TipoCelebracao.DOMINGO, periodo.corPadrao));
  }

  if (celebracoesAplicaveis.length === 0) {
    const nomePadrao = periodo.nome === 'Natal' ? 'Tempo de Natal' : periodo.nome;
    celebracoesAplicaveis.push(criarCelebracao(nomePadrao, TipoCelebracao.SEMANA, periodo.corPadrao));
  }

  const celebracoesOrdenadas = [...celebracoesAplicaveis].sort(
    (a, b) => (precedenciaPorTipo[b.tipo] ?? 0) - (precedenciaPorTipo[a.tipo] ?? 0),
  );

  const celebracaoEfetiva = celebracoesOrdenadas[0] ?? criarCelebracao('Tempo Comum', TipoCelebracao.SEMANA, periodo.corPadrao);
  const cor = celebracaoEfetiva.cor ?? periodo.corPadrao;

  return { data, periodo, celebracoes: celebracoesOrdenadas, cor };
}

export function calcularCorDoDia(data: Date): CorLiturgica {
  return calcularDiaLiturgico(data).cor;
}
