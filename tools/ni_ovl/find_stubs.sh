#!/bin/bash
# Iteratively find all functions that need to be stubbed
RECOMP="lib/N64ModernRuntime/N64Recomp/build_tool/N64Recomp"
TOML="recomp.toml"
STUBS=()

while true; do
    OUTPUT=$($RECOMP $TOML 2>&1)
    FAILED=$(echo "$OUTPUT" | grep "^Error recompiling" | sed 's/Error recompiling //')
    if [ -z "$FAILED" ]; then
        echo "N64Recomp completed successfully!"
        break
    fi
    echo "Stubbing: $FAILED"
    STUBS+=("$FAILED")
    # Add stub to recomp.toml - insert before the closing ]
    sed -i '' "/# NI overlay functions with jump table/a\\
    \"$FAILED\"," "$TOML"
done

echo ""
echo "All stubs needed:"
for s in "${STUBS[@]}"; do
    echo "  \"$s\","
done
