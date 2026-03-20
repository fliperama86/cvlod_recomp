#!/bin/bash
# Find functions with undeclared label errors and add stubs
cd /Users/dudu/Projects/recomp/lod

RECOMP="lib/N64ModernRuntime/N64Recomp/build_tool/N64Recomp"
TOML="recomp.toml"
NEW_STUBS=()

while true; do
    # Run N64Recomp
    $RECOMP $TOML 2>&1 | grep -q "Function count:" || { echo "N64Recomp failed"; exit 1; }

    # Fix truncation
    python3 tools/fix_n64recomp_truncation.py 2>/dev/null

    # Reconfigure cmake
    cd build && cmake .. > /dev/null 2>&1 && cd ..

    # Build and capture errors
    ERRORS=$(cd build && make -j8 2>&1 | grep "error: use of undeclared label")

    if [ -z "$ERRORS" ]; then
        echo "Build succeeded!"
        break
    fi

    # Extract function names from errors
    FUNC_NAMES=()
    while IFS= read -r line; do
        FILE=$(echo "$line" | cut -d: -f1)
        LABEL=$(echo "$line" | grep -o "L_[0-9A-Fa-f]*")
        if [ -n "$FILE" ] && [ -n "$LABEL" ]; then
            FUNC=$(awk -v label="$LABEL" '/RECOMP_FUNC/{fname=$3; sub(/\(.*/, "", fname)} $0 ~ label{print fname; exit}' "$FILE")
            if [ -n "$FUNC" ]; then
                FUNC_NAMES+=("$FUNC")
            fi
        fi
    done <<< "$ERRORS"

    if [ ${#FUNC_NAMES[@]} -eq 0 ]; then
        echo "Could not extract function names from errors:"
        echo "$ERRORS"
        exit 1
    fi

    for FUNC in "${FUNC_NAMES[@]}"; do
        echo "Stubbing: $FUNC"
        NEW_STUBS+=("$FUNC")
        # Add stub to recomp.toml before the closing ]
        sed -i '' "/\"ni_ovl_244_func_0F001014\"/a\\
    \"$FUNC\"," "$TOML"
    done
done

echo ""
echo "Additional stubs needed:"
for s in "${NEW_STUBS[@]}"; do
    echo "    \"$s\","
done
