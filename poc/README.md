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

## Observação

Este projeto possui também a biblioteca JavaScript/TypeScript para uso direto no código, sem depender de HTTP.
