#!/bin/bash

EXEC=./cub3D

GREEN="\033[0;32m"
RED="\033[0;31m"
NC="\033[0m"

echo "============================"
echo "      CUB3D PARSING TEST"
echo "============================"
echo

echo "🔹 Testing VALID maps"
echo "----------------------------"
for file in maps/valid/*.cub; do
    echo -n "▶ $file : "
    $EXEC "$file" > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}OK${NC}"
    else
        echo -e "${RED}FAIL${NC}"
    fi
done

echo
echo "🔹 Testing INVALID maps"
echo "----------------------------"
for file in maps/invalid/*.cub; do
    echo -n "▶ $file : "
    $EXEC "$file" > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo -e "${GREEN}ERROR (expected)${NC}"
    else
        echo -e "${RED}UNEXPECTED OK${NC}"
    fi
done

echo
echo "✅ Parsing tests finished"
