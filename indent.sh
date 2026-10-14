find . \( -path ./build -o -path ./.git \) -prune -o \
    -type f \( -name '*.c' -o -name '*.h' -o -name '*.s' -o -name '*.ld' \) -print |
while read -r f; do
    unexpand -t4 --first-only "$f" > "$f.tmp" && mv "$f.tmp" "$f"
done

echo "[x] Leftovers are:"
grep -rnP '^\t* {2,}\S' --include='*.c' --include='*.h' --include='*.s' --exclude-dir=build .