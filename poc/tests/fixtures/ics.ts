import { readFileSync } from 'node:fs';
import { CorLiturgica } from '../../src/tipos/liturgico.js';

interface EventoIcs {
  data: Date;
  tag: string;
  cor: CorLiturgica;
  nome: string;
}

const CORES_POR_GLIFO: Record<string, CorLiturgica> = {
  '⚪': CorLiturgica.BRANCO,
  '🔴': CorLiturgica.VERMELHO,
  '🟢': CorLiturgica.VERDE,
  '🟣': CorLiturgica.VIOLETA,
};

const PRECEDENCIA_TAG: Record<string, number> = {
  S: 5,
  F: 4,
  M: 3,
  'm*': 2,
  m: 1,
};

function desdobrarLinhas(conteudo: string): string[] {
  const linhas: string[] = [];
  for (const linhaBruta of conteudo.split('\n')) {
    const linha = linhaBruta.replace(/\r$/, '');
    if (linha.startsWith(' ')) {
      linhas[linhas.length - 1] += linha.slice(1);
    } else {
      linhas.push(linha);
    }
  }
  return linhas;
}

function extrairCampos(linhas: string[]): Record<string, string> {
  const campos: Record<string, string> = {};
  for (const linha of linhas) {
    const indice = linha.indexOf(':');
    if (indice === -1) continue;
    const chave = linha.slice(0, indice);
    const valor = linha.slice(indice + 1);
    campos[chave] = valor;
  }
  return campos;
}

export function parsearIcs(caminho: string): EventoIcs[] {
  const conteudo = readFileSync(caminho, 'utf8');
  const linhas = desdobrarLinhas(conteudo);
  const eventos: EventoIcs[] = [];

  let bloco: string[] | null = null;
  for (const linha of linhas) {
    if (linha === 'BEGIN:VEVENT') {
      bloco = [];
    } else if (linha === 'END:VEVENT') {
      if (bloco) {
        const campos = extrairCampos(bloco);
        const sumario = campos.SUMMARY ?? '';
        const glifo = Array.from(sumario)[0] ?? '';
        const cor = CORES_POR_GLIFO[glifo];
        const dataRaw = (campos['DTSTART;VALUE=DATE'] ?? '').match(/^(\d{4})(\d{2})(\d{2})/);
        const tag = sumario.match(/^\S+\s*\[([^\]]*)\]/)?.[1] ?? '';
        if (cor && dataRaw) {
          eventos.push({
            data: new Date(Date.UTC(+dataRaw[1], +dataRaw[2] - 1, +dataRaw[3])),
            tag,
            cor,
            nome: sumario.replace(/^\S+\s*\[[^\]]*\]\s*/, ''),
          });
        }
      }
      bloco = null;
    } else if (bloco) {
      bloco.push(linha);
    }
  }

  return eventos;
}

export function coresPorDia(eventos: EventoIcs[]): Map<string, CorLiturgica> {
  const porDia = new Map<string, EventoIcs[]>();
  for (const evento of eventos) {
    const chave = formatoDataIso(evento.data);
    if (!porDia.has(chave)) porDia.set(chave, []);
    porDia.get(chave)!.push(evento);
  }

  const resultado = new Map<string, CorLiturgica>();
  for (const [chave, lista] of porDia) {
    const eventosPrioritarios = lista
      .filter((evento) => PRECEDENCIA_TAG[evento.tag] !== undefined)
      .sort((a, b) => (PRECEDENCIA_TAG[b.tag] ?? 0) - (PRECEDENCIA_TAG[a.tag] ?? 0));
    const topo = eventosPrioritarios[0];
    if (topo && (topo.tag === 'S' || topo.tag === 'F' || topo.tag === 'M')) {
      resultado.set(chave, topo.cor);
    }
  }
  return resultado;
}

export function formatoDataIso(data: Date): string {
  const mes = String(data.getUTCMonth() + 1).padStart(2, '0');
  const dia = String(data.getUTCDate()).padStart(2, '0');
  return `${data.getUTCFullYear()}-${mes}-${dia}`;
}
