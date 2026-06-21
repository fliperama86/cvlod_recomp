#!/usr/bin/env python3
"""Run a CI smoke process and require it to survive for a short interval."""

from __future__ import annotations

import argparse
import os
import subprocess
import time
from pathlib import Path


def print_log_tail(log_path: Path, max_lines: int) -> None:
    if not log_path.exists():
        print(f"[ci-smoke] Log not found: {log_path}")
        return

    try:
        lines = log_path.read_text(errors="replace").splitlines()
    except OSError as exc:
        print(f"[ci-smoke] Failed to read log: {exc}")
        return

    print(f"[ci-smoke] Last {min(len(lines), max_lines)} log lines from {log_path}:")
    for line in lines[-max_lines:]:
        print(line)


def stop_process(proc: subprocess.Popen[object]) -> None:
    if proc.poll() is not None:
        return

    proc.terminate()
    try:
        proc.wait(timeout=5)
    except subprocess.TimeoutExpired:
        proc.kill()
        proc.wait(timeout=5)


def resolve_command(command: list[str], cwd: Path) -> list[str]:
    resolved = list(command)
    executable = Path(resolved[0])
    if not executable.is_absolute():
        candidate = (cwd / executable).resolve()
        if candidate.exists():
            resolved[0] = str(candidate)
    return resolved


def print_cwd_listing(cwd: Path) -> None:
    print(f"[ci-smoke] Directory listing for {cwd}:")
    try:
        for child in sorted(cwd.iterdir()):
            kind = "dir" if child.is_dir() else "file"
            size = child.stat().st_size if child.is_file() else 0
            print(f"[ci-smoke]   {kind:4} {size:10d} {child.name}")
    except OSError as exc:
        print(f"[ci-smoke] Failed to list directory: {exc}")


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--cwd", type=Path, required=True)
    parser.add_argument("--seconds", type=float, default=5.0)
    parser.add_argument("--log", type=Path, required=True)
    parser.add_argument("--tail-lines", type=int, default=180)
    parser.add_argument("command", nargs=argparse.REMAINDER)
    args = parser.parse_args()

    command = list(args.command)
    if command and command[0] == "--":
        command = command[1:]
    if not command:
        raise SystemExit("missing command")

    cwd = args.cwd.resolve()
    if not cwd.is_dir():
        raise SystemExit(f"working directory does not exist: {cwd}")

    log_path = args.log.resolve()
    log_path.parent.mkdir(parents=True, exist_ok=True)

    env = os.environ.copy()
    env.setdefault("SDL_AUDIODRIVER", "dummy")
    env.setdefault("LOD_ZELDA_WAIT_FOR_START", "1")
    command = resolve_command(command, cwd)

    print(f"[ci-smoke] Starting: {' '.join(command)}")
    print(f"[ci-smoke] Working directory: {cwd}")
    print(f"[ci-smoke] Required survival time: {args.seconds:.1f}s")

    with log_path.open("wb") as log_file:
        try:
            proc = subprocess.Popen(
                command,
                cwd=str(cwd),
                stdout=log_file,
                stderr=subprocess.STDOUT,
                env=env,
            )
        except FileNotFoundError as exc:
            print_cwd_listing(cwd)
            print(f"[ci-smoke] Failed to launch command: {exc}")
            return 127

        deadline = time.monotonic() + args.seconds
        while time.monotonic() < deadline:
            status = proc.poll()
            if status is not None:
                log_file.flush()
                print_log_tail(log_path, args.tail_lines)
                print(f"[ci-smoke] Process exited early with status {status}")
                return status if status != 0 else 1
            time.sleep(0.1)

        stop_process(proc)
        log_file.flush()

    print_log_tail(log_path, args.tail_lines)
    print("[ci-smoke] OK, process survived smoke interval")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
