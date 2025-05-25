#!/bin/bash

set -e

SRC="ptree.c"
BIN="ptree"
INSTALL_DIR="$HOME/.local/bin"

echo "[+] Compiling $SRC..."
cc -Wall -Wextra -Werror "$SRC" -o "$BIN"

echo "[+] Creating install directory at $INSTALL_DIR if it doesn't exist..."
mkdir -p "$INSTALL_DIR"

echo "[+] Moving $BIN to $INSTALL_DIR..."
mv "$BIN" "$INSTALL_DIR/"

echo "[+] Ensuring $INSTALL_DIR is in PATH..."

# Append export to ~/.zshrc if not already present
if [ -f "$HOME/.zshrc" ] && ! grep -q "$INSTALL_DIR" "$HOME/.zshrc"; then
    echo "export PATH=\"\$HOME/.local/bin:\$PATH\"" >> "$HOME/.zshrc"
    echo "    → Updated ~/.zshrc"
fi

# Append export to ~/.bashrc if not already present
if [ -f "$HOME/.bashrc" ] && ! grep -q "$INSTALL_DIR" "$HOME/.bashrc"; then
    echo "export PATH=\"\$HOME/.local/bin:\$PATH\"" >> "$HOME/.bashrc"
    echo "    → Updated ~/.bashrc"
fi

echo "[✓] Installation complete. Restart your terminal or run:"
echo "    'source ~/.zshrc' for zsh"
echo "    'source ~/.bashrc' for bash"
echo "Then run:"
echo "    ptree"
