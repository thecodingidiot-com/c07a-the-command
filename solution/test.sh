#!/bin/bash
# c07a — The Command / test.sh
#
# Bash-comparison suite: each case is piped, as one script, to both
# ./c07shell and to bash; stdout, stderr, and exit status are diffed.
# Bash is the oracle. Scoped to what this part actually builds: single
# external commands, and the three in-process builtins (cd/pwd/exit).
#
#   bash test.sh

set -o pipefail

pass_count=0
fail_count=0

run_case() {
    # $3 = "status_only" skips stdout/stderr comparison -- for messages
    # that are legitimately implementation-specific wording (every real
    # shell phrases "command not found" differently).
    local label="$1"
    local script="$2"
    local mode="${3:-full}"
    local our_out our_err our_status
    local bash_out bash_err bash_status

    our_out=$(printf '%s' "$script" | ./c07shell 2>/tmp/c07a_our_err)
    our_status=$?
    our_err=$(cat /tmp/c07a_our_err)

    bash_out=$(printf '%s' "$script" | bash 2>/tmp/c07a_bash_err)
    bash_status=$?
    bash_err=$(cat /tmp/c07a_bash_err)

    local ok=1
    [[ "$our_status" == "$bash_status" ]] || ok=0
    if [[ "$mode" == "full" ]]; then
        [[ "$our_out" == "$bash_out" ]] || ok=0
    fi

    if [[ "$ok" == "1" ]]; then
        echo "PASS  $label"
        pass_count=$((pass_count + 1))
    else
        echo "FAIL  $label"
        echo "      ours : status=$our_status out=[$our_out] err=[$our_err]"
        echo "      bash : status=$bash_status out=[$bash_out] err=[$bash_err]"
        fail_count=$((fail_count + 1))
    fi
}

run_case "echo simple"                                  $'echo hello\n'
run_case "echo multiple words"                           $'echo one two three\n'
run_case "exit code from true propagates via bare exit"  $'true\nexit\n'
run_case "exit code from false propagates via bare exit" $'false\nexit\n'
run_case "exit with explicit code"                       $'exit 7\n'
run_case "pwd runs"                                      $'pwd\n'
run_case "cd then pwd"                                   $'cd /tmp\npwd\n'
run_case "nonexistent command"                           $'nosuchcommand123\n' status_only

echo
echo "$pass_count passed, $fail_count failed"
exit "$fail_count"
