import { cp, mkdir, rm } from 'node:fs/promises';
import { dirname, join } from 'node:path';
import { fileURLToPath } from 'node:url';
import { build } from 'esbuild';

const webDir = dirname(fileURLToPath(import.meta.url));
const distDir = join(webDir, 'dist');

await rm(distDir, { recursive: true, force: true });
await mkdir(distDir, { recursive: true });

await build({
  entryPoints: [join(webDir, 'app.ts')],
  bundle: true,
  format: 'esm',
  target: ['es2020'],
  minify: true,
  outfile: join(distDir, 'bundle.js'),
  logLevel: 'info',
});

await cp(join(webDir, 'index.html'), join(distDir, 'index.html'));
await cp(join(webDir, 'styles.css'), join(distDir, 'styles.css'));

console.log(`Página gerada em ${distDir}`);
