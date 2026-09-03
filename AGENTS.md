# AGENTS - Guia Mestre

Este arquivo serve como índice central para as instruções específicas de cada agente.  
Cada agente possui seu próprio arquivo dedicado, que **não é mesclado** aqui, para permitir edição independente.

---

## Regras Mandatórias

- **LaTeX**: **NUNCA** use a estrutura `\ifdefined\mainfile`. Todos os arquivos `.tex` devem seguir o template padrão com `\documentclass`, `\input{preamble.tex}`, `\input{variables.tex}`, `\begin{document}` e `\end{document}`.
- **Identificadores**: Nomes de funções, variáveis e identificadores devem ser **SEMPRE** em inglês (EUA).

---

## Estrutura

- `docs/agents_git.md` → Instruções e fluxos de trabalho para Git, GitHub e GitLab  
- `docs/agents_latex.md` → Instruções e padrões para documentos LaTeX  
- `docs/agents_python.md` → Instruções para Python, PEP8, Sphinx e formatação de código

---

## Como usar no ChatGPT Codex

No **ChatGPT Codex** (ou outra instância), você pode pedir para o modelo considerar **somente** uma seção ou arquivo específico, por exemplo:

> "Use apenas as instruções do arquivo `docs/AGENTS_python.md`"  
> "Considere as instruções do `docs/AGENTS_git.md` para revisar este commit"

---

## Leitura para Instruções Externas

- [agents_git.md](subs/submodules/agents_git.md)  
- [agents_github_actions.md](subs/submodules/agents_github_actions.md)  
- [agents_latex.md](subs/submodules/agents_latex.md)  
- [agents_python.md](subs/submodules/agents_python.md)
- [agents_shell.md](subs/submodules/agents_shell.md)

---

> **Nota:** Cada arquivo é independente e pode ser atualizado separadamente.  
> O `AGENTS.md` serve apenas como guia/índice mestre.

## File naming policy

- File names must be written in English.
- Use only underscores (`_`) to separate words in file names; do not use hyphens (`-`), spaces, or other separators.

- Keep mandatory ecosystem names such as `AGENTS.md`, `GEMINI.md`, and `README.md` when a tool or platform requires the conventional name.
