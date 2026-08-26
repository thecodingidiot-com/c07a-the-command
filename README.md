# c07a-the-command

Companion repository for **c07a — The Command** at
[thecodingidiot.com](https://thecodingidiot.com). First of four parts
(c07a–d) building a Unix shell in C — this part is the read-eval-print
loop skeleton: a prompt, one external command run via `fork()` +
`execvp()` + `waitpid()`, `$?` exit status, and the three builtins
(`cd`, `pwd`, `exit`) that have to run without forking.

---

## Follow my journey

Working through c07a alongside the implementation pages? Build
`c07shell` step by step, then run the tester.

Clone this repository and copy `test.sh` into your working directory:

```bash
git clone https://github.com/thecodingidiot-com/c07a-the-command.git
cp c07a-the-command/test.sh ~/c07a-practice/
cd ~/c07a-practice
make re
bash test.sh
```

All tests must pass before the chapter is complete.

---

## Follow your journey

Building `c07shell` independently? Here is the full project brief.

**What you build:** a prompt loop reading one line at a time,
whitespace-only tokenizing (no quote handling yet — that's c07b), a
single external command run via `fork()`/`execvp()`/`waitpid()` with
`$?` tracked, and three builtins that run in the shell's own process
because forking them would either lose their effect (`cd`) or be
pointless (`pwd`, `exit`).

Source is split across three files:

| File | Contents |
| --- | --- |
| `main.c` | the prompt/read loop, naive tokenizing, dispatch |
| `builtins.c` | `cd`, `pwd`, `exit` |
| `exec.c` | `fork()`/`execvp()`/`waitpid()` for one external command |

Build and test your own version first. Use `solution/` to compare once
you are done, not before.

**Deliberately unfixed in this part:** quoted arguments split wrong
(`echo "hi there"` becomes three words), and Ctrl+C kills the whole
shell (no `sigaction()` call exists yet). Both are named on the
closing page, not hidden — c07b fixes quoting, c07d fixes signals.

---

## Building the solution

The `solution/` Makefile expects `libtci.a`, `libtciutil.a`,
`libtci.h`, and `libtciutil.h` to be present under `solution/libtci/`.
They're already committed there — no external dependencies beyond a
C99 compiler.

```bash
cd c07a-the-command/solution
make re
```

## What the tester checks

Bash-comparison: every case is piped, as one script, to both
`./c07shell` and to real `bash`, then stdout/stderr/exit status are
diffed. Bash is the oracle throughout the whole c07 arc, not just this
part.

- `echo` with and without arguments.
- `$?` propagating correctly from `true`/`false` and from an explicit
  `exit N`.
- `cd` actually moving the shell (checked via a following `pwd`).
- A nonexistent command's exit status (wording is status-only —
  every real shell phrases "command not found" differently).

---

## License

MIT License. See [LICENSE](LICENSE).
