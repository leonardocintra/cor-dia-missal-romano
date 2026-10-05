import { pathToFileURL } from 'node:url';
import fastify, { type FastifyInstance } from 'fastify';
import { calcularDiaLiturgico } from './calendario/calendario-liturgico.js';

interface QueryDataCor {
  data: string;
}

export function parseDataCor(data: string): Date {
  const date = new Date(`${data}T00:00:00.000Z`);

  if (Number.isNaN(date.getTime())) {
    throw new Error(`Data inválida: ${data}`);
  }

  return date;
}

export function buildServer(): FastifyInstance {
  const app = fastify({ logger: false });

  app.get('/', async () => ({
    name: 'Cor do Dia Missal Romano',
    endpoints: ['/cor?data=YYYY-MM-DD'],
  }));

  app.get<{ Querystring: QueryDataCor }>('/cor', {
    schema: {
      querystring: {
        type: 'object',
        required: ['data'],
        additionalProperties: false,
        properties: {
          data: {
            type: 'string',
            description: 'Data em formato ISO YYYY-MM-DD',
          },
        },
      },
    },
  }, async (request, reply) => {
    try {
      const data = parseDataCor(request.query.data);
      const dia = calcularDiaLiturgico(data);

      return {
        data: dia.data.toISOString().slice(0, 10),
        cor: dia.cor,
        periodo: {
          nome: dia.periodo.nome,
          codigo: dia.periodo.codigo,
          corPadrao: dia.periodo.corPadrao,
        },
        celebracoes: dia.celebracoes.map((celebracao) => ({
          nome: celebracao.nome,
          tipo: celebracao.tipo,
          ...(celebracao.cor ? { cor: celebracao.cor } : {}),
        })),
      };
    } catch (error) {
      const message = error instanceof Error ? error.message : 'Data inválida';
      return reply.code(400).send({
        error: 'data_invalida',
        message,
      });
    }
  });

  return app;
}

export async function startServer(port = Number(process.env.PORT ?? 3000), host = '0.0.0.0') {
  const app = buildServer();
  await app.listen({ port, host });
  return app;
}

if (process.argv[1] && import.meta.url === pathToFileURL(process.argv[1]).href) {
  startServer().then(() => {
    console.log('Servidor Fastify em execução na porta 3000');
  }).catch((error) => {
    console.error('Erro ao iniciar o servidor Fastify:', error);
    process.exit(1);
  });
}
