#!/bin/bash

BIN_NAME="Hyprfetch"
INSTALL_DIR="$HOME/.local/bin"

mkdir -p "$INSTALL_DIR"
cp "./$BIN_NAME" "$INSTALL_DIR/$BIN_NAME"
chmod +x "$INSTALL_DIR/$BIN_NAME"

update_path() {
    SHELL_RC="$1"
    if [ -f "$SHELL_RC" ]; then
        if ! grep -q "$INSTALL_DIR" "$SHELL_RC"; then
            echo "export PATH=\"$INSTALL_DIR:\$PATH\"" >> "$SHELL_RC"
        fi
    fi
}

update_path "$HOME/.bashrc"
update_path "$HOME/.zshrc"
update_path "$HOME/.config/fish/config.fish"

echo "Hyprfetch installed! You can now run it using 'Hyprfetch'"
