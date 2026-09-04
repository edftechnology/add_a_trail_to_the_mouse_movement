# Como instalar/configurar/usar o `rastro do mouse` no `Linux Ubuntu`

## Resumo

Guia direto para instalar e configurar um `rastro do mouse` no `Linux Ubuntu` pelo `Terminal Emulator`, usando pacotes instalados via `apt` para compilar e executar um overlay transparente em ambiente `X11`.

## _Abstract_

_A straightforward guide to install and configure a `mouse trail` on `Linux Ubuntu` through the `Terminal Emulator`, using `apt` packages to build and run a transparent overlay on an `X11` desktop._


## Descrição

### `rastro do mouse`

O `rastro do mouse` é um efeito visual que desenha partículas seguindo o ponteiro, semelhante ao recurso de rastro do ponteiro do `Windows`. No `Linux Ubuntu`, especialmente em ambientes `XFCE`/`X11`, esse efeito pode ser obtido com um overlay transparente compilado localmente e configurado para iniciar automaticamente com a sessão.

Este procedimento usa pacotes instalados via `apt` para preparar o ambiente, compilar o projeto `cursor-trail` e configurar uma silhueta preta do ponteiro com transparência, reproduzindo cópias do cursor que desaparecem gradualmente.


## Pré-requisitos

- Permissão para usar `sudo`
- Sessão gráfica `X11`
- Compositor do ambiente gráfico ativo
- Conexão com a internet para instalar pacotes e clonar o repositório
- `apt` funcional no sistema


## 1. Abrir o `Terminal Emulator`

1. Abrir o `Terminal Emulator`. Você pode fazer isso pressionando:

    ```bash
    Ctrl + Alt + T
    ```


2. Certifique-se de que seu sistema esteja limpo e atualizado.

    2.1 Limpar o `cache` do gerenciador de pacotes `apt`. Especificamente, ele remove todos os arquivos de pacotes (`.deb`) baixados pelo `apt` e armazenados em `/var/cache/apt/archives/`. Digite o seguinte comando:
        
    ```bash
    sudo apt clean
    ```

    2.2 Remover pacotes `.deb` antigos ou duplicados do `cache` local. É útil para liberar espaço, pois remove apenas os pacotes que não podem mais ser baixados (ou seja, versões antigas de pacotes que foram atualizados). Digite o seguinte comando:

    ```bash
    sudo apt autoclean
    ```

    2.3 Remover pacotes que foram automaticamente instalados para satisfazer as dependências de outros pacotes e que não são mais necessários. Digite o seguinte comando:

    ```bash
    sudo apt autoremove -y
    ```

    2.4 Buscar as atualizações disponíveis para os pacotes que estão instalados em seu sistema. Digite o seguinte comando e pressione `Enter`:

    ```bash
    sudo apt update
    ```

    2.5 **Corrigir pacotes quebrados**: Isso atualizará a lista de pacotes disponíveis e tentará corrigir pacotes quebrados ou com dependências ausentes:

    ```bash
    sudo apt --fix-broken install
    ```

    2.6 Limpar o `cache` do gerenciador de pacotes `apt` novamente:

    ```bash
    sudo apt clean
    ```

    2.7 Para ver a lista de pacotes a serem atualizados, digite o seguinte comando e pressione `Enter`:

    ```bash
    sudo apt list --upgradable
    ```

    2.8 Realmente atualizar os pacotes instalados para as suas versões mais recentes, com base na última vez que você executou `sudo apt update`. Digite o seguinte comando e pressione `Enter`:

    ```bash
    sudo apt full-upgrade -y
    ```


## 3. Instalar as dependências do `rastro do mouse` via `apt`

O `Linux Ubuntu` não possui, por padrão, uma opção nativa universal para rastro de ponteiro igual ao `Windows`. Para obter esse efeito em `X11`, instale via `apt` as dependências de compilação, janela transparente e diagnóstico gráfico.

1. Habilitar o repositório `universe`, quando necessário:

    ```bash
    sudo add-apt-repository universe
    sudo apt update
    ```

2. Instalar os pacotes necessários:

    ```bash
    sudo apt install -y \
        git \
        cmake \
        build-essential \
        pkg-config \
        libglfw3-dev \
        libx11-dev \
        libxext-dev \
        x11-utils \
        xdotool \
        imagemagick
    ```

3. Confirmar que a sessão atual usa `X11`:

    ```bash
    echo "$XDG_SESSION_TYPE"
    ```

    A saída esperada é:

    ```bash
    x11
    ```


## 4. Baixar, compilar e instalar o `cursor-trail`

1. Criar uma pasta para o projeto e clonar o repositório:

    ```bash
    mkdir -p "$HOME/.local/src"
    cd "$HOME/.local/src"
    git clone https://github.com/nayutalienx/cursor-trail.git
    cd cursor-trail
    ```

2. Compilar o projeto com `CMake`:

    ```bash
    cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
    cmake --build build -j"$(nproc)"
    ```

3. Criar uma textura de ponteiro preta e transparente, semelhante às cópias do cursor no rastro do `Windows`:

    ```bash
    convert -size 32x32 xc:none -fill black \
        -draw 'polygon 3,2 3,26.5 9.3,20.9 13.5,30 17.7,28 13.5,18.9 22,18.9' \
        PNG32:CursorTrail/windows-cursor-trail.png
    ```

4. Configurar o projeto para usar a textura do ponteiro:

    ```bash
    sed -i 's|texture=cursortrail.png|texture=windows-cursor-trail.png|' config.ini
    ```


## 5. Executar o `rastro do mouse`

1. Entrar no diretório dos recursos e iniciar o overlay:

    ```bash
    cd "$HOME/.local/src/cursor-trail/CursorTrail"
    ../build/CursorTrail --config ../config.ini
    ```

2. Para encerrar o processo iniciado no terminal, pressione:

    ```bash
    Ctrl + C
    ```

3. Se o rastro aparecer como pontos ou roxo, confirme se a configuração aponta para a textura do ponteiro:

    ```bash
    grep '^texture=' "$HOME/.local/src/cursor-trail/config.ini"
    ```

    A saída esperada é:

    ```bash
    texture=windows-cursor-trail.png
    ```


## 6. Configurar inicialização automática no `Linux Ubuntu`

1. Criar um comando local para iniciar, parar e reiniciar o rastro:

    ```bash
    mkdir -p "$HOME/.local/bin" "$HOME/.config/autostart"
    nano "$HOME/.local/bin/cursor-trail"
    ```

2. Inserir o conteúdo abaixo no arquivo:

    ```bash
    #!/usr/bin/env bash
    set -euo pipefail

    APP_DIR="$HOME/.local/src/cursor-trail"
    BIN="$APP_DIR/build/CursorTrail"
    RUN_DIR="$APP_DIR/CursorTrail"
    CONFIG="$APP_DIR/config.ini"
    PID_FILE="/tmp/cursor-trail.pid"

    case "${1:-start}" in
      start)
        "$0" stop
        setsid "$0" run >/tmp/cursor-trail.log 2>&1 &
        echo $! > "$PID_FILE"
        ;;
      run)
        cd "$RUN_DIR"
        "$BIN" --config "$CONFIG"
        ;;
      stop)
        if [ -f "$PID_FILE" ]; then
          kill "$(cat "$PID_FILE")" 2>/dev/null || true
          rm "$PID_FILE"
        fi
        pkill -f "$BIN --config $CONFIG" 2>/dev/null || true
        ;;
      restart)
        "$0" stop
        "$0" start
        ;;
      status)
        if [ -f "$PID_FILE" ] && kill -0 "$(cat "$PID_FILE")" 2>/dev/null; then
          echo "CursorTrail is running"
        else
          echo "CursorTrail is stopped"
          exit 1
        fi
        ;;
      *)
        echo "Usage: cursor-trail [start|stop|restart|status]" >&2
        exit 2
        ;;
    esac
    ```

3. Dar permissão de execução ao comando:

    ```bash
    chmod +x "$HOME/.local/bin/cursor-trail"
    ```

4. Criar o arquivo de inicialização automática:

    ```bash
    nano "$HOME/.config/autostart/cursor-trail.desktop"
    ```

5. Inserir o conteúdo abaixo no arquivo:

    ```bash
    [Desktop Entry]
    Type=Application
    Name=Cursor Trail
    Comment=Mouse cursor trail overlay
    Exec=sh -lc '$HOME/.local/bin/cursor-trail start'
    Terminal=false
    X-GNOME-Autostart-enabled=true
    Hidden=false
    ```

6. Iniciar o rastro imediatamente:

    ```bash
    "$HOME/.local/bin/cursor-trail" restart
    "$HOME/.local/bin/cursor-trail" status
    ```


## Compatibilidade

- Este procedimento é indicado para `Linux Ubuntu` em sessão `X11`.
- Em `Wayland`, o acesso global à posição do ponteiro e overlays transparentes são mais restritos por segurança.
- No `XFCE`, mantenha o compositor ativo para que a transparência do overlay funcione corretamente.
- O pacote `gromit-mpx` existe nos repositórios do `Ubuntu`, mas é uma ferramenta de anotação na tela, não um rastro automático do ponteiro igual ao recurso do `Windows`.


## Licença

Este repositório inclui o arquivo `LICENSE.txt`.

## Contato e suporte

Para dúvidas ou problemas, consulte o repositório oficial do `cursor-trail`, a documentação do `GLFW` e a documentação da sua versão do `Linux Ubuntu`.


## Referências

[1] OPENAI. **Instalar o `rastro do mouse` no `linux ubuntu` pelo `terminal emulator`**. Disponível em: <https://chatgpt.com/g/g-p-6980caf949648191ad6acfcdbe590f9e/project>. ChatGPT. Acessado em: 03/09/2026.

[2] NAYUTALIENX. **Cursor-trail**. Disponível em: <https://github.com/nayutalienx/cursor-trail>. Acessado em: 03/09/2026.

[3] GLFW. **Window guide**. Disponível em: <https://www.glfw.org/docs/3.3/window_guide.html>. Acessado em: 03/09/2026.

[4] UBUNTU. **Details of package build-essential in jammy**. Disponível em: <https://packages.ubuntu.com/jammy/build-essential>. Acessado em: 03/09/2026.

[5] UBUNTU. **Details of source package gromit-mpx in jammy**. Disponível em: <https://packages.ubuntu.com/source/jammy/gromit-mpx>. Acessado em: 03/09/2026.

