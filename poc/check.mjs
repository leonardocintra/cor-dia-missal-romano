import { calcularDiaLiturgico } from './src/calendario/calendario-liturgico.js';
import { dataUtc, adicionarDias } from './src/calendario/regras.js';
const hoje = dataUtc(2026, 9, 15);
for (let i=0;i<3;i++){
  const d=adicionarDias(hoje,i);
  const dia=calcularDiaLiturgico(d);
  console.log(d.toISOString().slice(0,10), dia.periodo.nome, dia.cor, '|', dia.celebracoes[0].nome);
}
