# GEMINI - Guia Mestre

Este arquivo serve como índice central para as instruções específicas do Gemini.  
Cada arquivo dedicado **não é mesclado** aqui, para permitir edição independente.

---

## Estrutura

- `scripts/gemini_git.md` → Instruções e fluxos de trabalho para Git, GitHub e GitLab
- `scripts/gemini_github_actions.md` → Instruções para GitHub Actions
- `scripts/gemini_latex.md` → Instruções e padrões para documentos LaTeX
- `scripts/gemini_python.md` → Instruções para Python, PEP8, Sphinx e formatação de código
- `scripts/gemini_shell.md` → Instruções para Shell Script e terminal

---

## Como usar no Gemini

No **Gemini CLI** (ou outra instância do Gemini), você pode pedir para o modelo considerar **somente** uma seção ou arquivo específico, por exemplo:

> "Use apenas as instruções do arquivo `scripts/gemini_python.md`"  
> "Considere as instruções do `scripts/gemini_git.md` para revisar este commit"

---

## Leitura para Instruções Externas

- [gemini_git.md](scripts/gemini_git.md)
- [gemini_github_actions.md](scripts/gemini_github_actions.md)
- [gemini_latex.md](scripts/gemini_latex.md)
- [gemini_python.md](scripts/gemini_python.md)
- [gemini_shell.md](scripts/gemini_shell.md)

---

> **Nota:** Cada arquivo é independente e pode ser atualizado separadamente.  
> O `GEMINI.md` serve apenas como guia/índice mestre para o Gemini.

## File naming policy

- File names must be written in English.
- Use only underscores (`_`) to separate words in file names; do not use hyphens (`-`), spaces, or other separators.

- Keep mandatory ecosystem names such as `AGENTS.md`, `GEMINI.md`, and `README.md` when a tool or platform requires the conventional name.
