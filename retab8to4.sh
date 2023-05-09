#!/bin/sh

for FILE in "$@"; do
    echo Retabbing "$FILE"
    expand -8 "$FILE" | unexpand -t 4 > "$FILE".tmp &&
    mv "$FILE".tmp "$FILE"
done
