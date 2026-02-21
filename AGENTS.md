## Guidelines

- Don’t change any code or run any non-read-only-commands without informing me and discussing it first.
- Be extremely methodical, performing one step at a time and ensuring it works properly before continuing.
- Do not commit code unless explicitly told otherwise.

## Session Memory (Required)

- Maintain `SCRATCHBOOK.md` as persistent cross-session memory.
- Update `SCRATCHBOOK.md` on nearly every user interaction with concise entries.
- Always record:
  - User preferences and workflow constraints.
  - What was attempted, what worked, and what failed/regressed.
  - Current hypotheses, safe/unsafe patterns, and next recommended steps.
- Keep entries factual, timestamped, and short (append-only log style).
- Prefer introducing risky or experimental behavior behind compile-time feature flags (`#ifdef`/macros).
- Default policy for experiments: flag-gated, off by default, easy rollback path, avoid direct regression risk.
