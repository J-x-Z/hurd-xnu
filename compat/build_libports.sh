#!/bin/bash
# Build script for libports on macOS/XNU
# Run from the xnu-hurd directory

cd "$(dirname "$0")"

COMPAT_DIR="."
LIBPORTS_SRC="../hurd/libports"
CFLAGS="-I. -D__extern_inline='static inline' -DPORTS_DEFINE_EI"

echo "=== Building libports for XNU ==="

# Core files that we know compile
CORE_FILES="
    init.c
    create-class.c
    create-bucket.c
    port-ref.c
    port-deref.c
    lookup-port.c
"

# MIG generated files
MIG_FILES="
    notifyServer.c
    interruptServer.c
"

SUCCESS=0
FAILED=0

echo "--- Compiling core libports files ---"
for f in $CORE_FILES; do
    src="$LIBPORTS_SRC/$f"
    obj="${f%.c}.o"
    if gcc $CFLAGS -c -o "$obj" "$src" 2>/dev/null; then
        echo "[OK] $f"
        ((SUCCESS++))
    else
        echo "[FAIL] $f"
        ((FAILED++))
    fi
done

echo "--- Compiling MIG generated files ---"
for f in $MIG_FILES; do
    obj="${f%.c}.o"
    if gcc $CFLAGS -c -o "$obj" "$f" 2>/dev/null; then
        echo "[OK] $f"
        ((SUCCESS++))
    else
        echo "[FAIL] $f"
        ((FAILED++))
    fi
done

echo ""
echo "=== Summary: $SUCCESS succeeded, $FAILED failed ==="
ls -la *.o 2>/dev/null | wc -l | xargs echo "Total .o files:"
