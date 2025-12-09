# MiniFS

Mini sistema de arquivos em C com interface interativa. Nesta versão, as operações usam o sistema de arquivos real do Linux: pastas e arquivos são criados/alterados dentro do diretório raiz onde o programa é executado.

## Visão Geral

- Diretório raiz: é o `cwd` (diretório atual) no momento da execução.
- Diretório atual: começa no raiz e muda com `cd`. Não é permitido navegar para fora do raiz.
- Nomes de arquivos e diretórios devem ser simples (sem `/` ou `\`). Use `cd` para entrar em subpastas.

## Comandos Disponíveis

- `mkdir <nome>`: cria uma subpasta real no diretório atual.
- `cd <nome>`: entra em uma subpasta existente. `cd /` volta ao raiz; `cd ..` sobe um nível (sem sair do raiz).
- `touch <arquivo>`: cria o arquivo (ou atualiza sua data) no diretório atual.
- `echo <arquivo> <conteudo>`: escreve o conteúdo no arquivo (substitui o conteúdo anterior). Na UI, use `_` para espaços.
- `cat <arquivo>`: lê e imprime o conteúdo do arquivo.
- `cp <origem> <destino>`: copia o arquivo no diretório atual.
- `mv <origem> <destino>`: renomeia o arquivo no diretório atual.
- `rm <arquivo>`: remove o arquivo.
- `chmod <arquivo> <permissao>`: altera permissões (ver seção abaixo).

## Sobre `chmod` no Linux

- O programa aceita permissões em formato octal (ex.: `755`, `644`).
- As permissões são aplicadas diretamente aos arquivos usando a syscall `chmod` do Linux.
- Os bits de permissão funcionam como esperado:
	- Leitura (r=4): permite ler o arquivo
	- Escrita (w=2): permite modificar o arquivo
	- Execução (x=1): permite executar o arquivo como programa
- As permissões são divididas em três grupos (usuário, grupo, outros).
- Exemplo: `chmod arquivo.txt 644` define rw-r--r-- (usuário lê/escreve, outros apenas leem).

## Compilação e Execução

### 🐧 Linux Nativo

#### Via Makefile (recomendado):
```bash
make
./mini_fs
```

#### Via script build.sh:
```bash
chmod +x build.sh
./build.sh
./mini_fs
```

#### Compilação manual:
```bash
gcc -Wall -Wextra -Iinclude -o mini_fs src/main.c src/filesystem.c
./mini_fs
```

#### Limpeza:
```bash
make clean
```

### 🪟 Windows (WSL)

#### Pré-requisitos:
Instale as ferramentas de desenvolvimento C no WSL:
```bash
wsl sudo apt update
wsl sudo apt install -y build-essential gdb
```

#### Compilação e execução:
```bash
wsl make
wsl ./mini_fs
```

Ou compile uma vez e execute várias vezes:
```bash
wsl make
wsl ./mini_fs
```

#### Limpeza:
```bash
wsl make clean
```

#### 💡 Dica para VS Code:
Para melhor experiência no VS Code, reabra o projeto diretamente no WSL:
1. Pressione `F1` ou `Ctrl+Shift+P`
2. Digite: **"WSL: Reopen Folder in WSL"**
3. Depois use normalmente:
   - `Ctrl+Shift+B` para compilar
   - `F5` para debug
   - Terminal integrado funcionará nativamente no Linux

## Exemplo Rápido

Sequência de operações comuns:
1. `mkdir pasta1`
2. `cd pasta1`
3. `touch arquivo.txt`
4. `echo arquivo.txt conteudo_com_espacos_usa_underscore`
5. `cat arquivo.txt`

## Limitações e Próximos Passos

- Não há listagem (`ls`) de arquivos/diretórios no menu.
- Navegação e operações são somente no diretório atual (sem caminhos com barras).
- Podemos adicionar `ls` e suporte opcional a caminhos relativos se necessário.

## Contribuições

Contribuições são bem-vindas! Abra issues ou PRs com sugestões e correções.