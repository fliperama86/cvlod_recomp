#!/usr/bin/env python3
"""
Validate recomp.toml configuration.
Ensures every 'ignored' function has a native implementation.
"""
import re
import sys
import tomllib
from pathlib import Path


def main():
    root = Path(__file__).parent.parent

    # Parse recomp.toml
    with open(root / "recomp.toml", "rb") as f:
        config = tomllib.load(f)

    ignored = set(config.get("patches", {}).get("ignored", []))
    print(f"Found {len(ignored)} ignored functions")

    # Find implementations in src/*.cpp. Match definitions only, not extern
    # declarations in generated files such as src/funcs.cpp.
    implemented = set()
    definition_re = re.compile(
        r'^\s*(?:extern\s+"C"\s+)?void\s+(\w+)\s*\([^;{}]*\)\s*\{',
        re.MULTILINE,
    )
    for cpp_file in (root / "src").glob("*.cpp"):
        content = cpp_file.read_text()
        # Match: extern "C" void func_XXXXXXXX(...) { or L_XXXXXXXX(...) {
        for match in definition_re.finditer(content):
            implemented.add(match.group(1))

    # Find registrations in main.cpp (if using set_libultra_wrapper)
    main_cpp = (root / "src/main.cpp").read_text()
    registered = set()
    for match in re.finditer(r'set_libultra_wrapper\(0x([0-9A-Fa-f]+)', main_cpp):
        addr = int(match.group(1), 16)
        registered.add(f"func_{addr:08X}")
        registered.add(f".L{addr:08X}")  # Alternative naming

    # Check for problems
    problems = []
    for func in sorted(ignored):
        # Handle different naming conventions
        # .L8008C4E0 -> L_8008C4E0
        lookup_names = [func]
        if func.startswith(".L"):
            lookup_names.append("L_" + func[2:])
        elif func.startswith("func_"):
            lookup_names.append(func)

        found = False
        for name in lookup_names:
            if name in implemented:
                found = True
                break

        if not found:
            problems.append(f"MISSING: {func} - ignored but no implementation found")

    if problems:
        print("\n=== CONFIGURATION ERRORS ===")
        for p in problems:
            print(f"  {p}")
        print(f"\nTotal: {len(problems)} functions need fixing")
        print("\nFix options:")
        print("  1. Remove from 'ignored' list in recomp.toml (let N64Recomp generate code)")
        print("  2. Add native implementation in src/*.cpp")
        return 1
    else:
        print("\nAll ignored functions have implementations")
        print("\nImplemented functions found:")
        for func in sorted(ignored):
            lookup_name = "L_" + func[2:] if func.startswith(".L") else func
            for name in [func, lookup_name]:
                if name in implemented:
                    print(f"  {func} -> {name}")
                    break
        return 0


if __name__ == "__main__":
    sys.exit(main())
