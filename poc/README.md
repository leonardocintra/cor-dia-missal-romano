# Cor do Dia Missal Romano

Biblioteca e API simples para calcular a cor litúrgica de um dia específico, conforme o calendário litúrgico da Igreja Católica.

## Instalação

```bash
npm install
```

## Executar a API

```bash
npm start
```

A API ficará disponível em:

```text
http://localhost:3000
```

## Endpoint

### GET /cor

Consulta a cor do dia para uma data informada.

### Parâmetro

- `data`: data no formato `YYYY-MM-DD`

### Exemplo

```bash
curl "http://localhost:3000/cor?data=2026-03-05"
```

### Resposta esperada

```json
{
  "data": "2026-03-05",
  "cor": "violeta",
  "periodo": {
    "nome": "Quaresma",
    "codigo": "quaresma",
    "corPadrao": "violeta"
  },
  "celebracoes": [
    {
      "nome": "Quaresma",
      "tipo": "semana",
      "cor": "violeta"
    }
  ]
}
```

## Página web (GitHub Pages)

Página estática, mobile-first, que mostra a cor litúrgica de hoje e dos próximos 30 dias. O cálculo roda inteiramente no navegador (sem servidor).

```bash
npm run build:web
```

O resultado é gerado em `web/dist/` (`index.html`, `styles.css`, `bundle.js`). Para pré-visualizar:

```bash
npx serve web/dist
```

O deploy é automático via GitHub Actions (`.github/workflows/pages.yml`) a cada push na `main`.

## Observação

Este projeto possui também a biblioteca JavaScript/TypeScript para uso direto no código, sem depender de HTTP.
