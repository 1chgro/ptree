# ptree

`ptree` is a simple terminal utility that prints a visual representation of the current (or given) directory structure, similar to the `tree` command, but with file sizes in human-readable format.

## 📦 Features

- Recursively prints all subdirectories and files
- Displays file sizes in B or KB
- Clean and readable tree-style output
- Works from any directory
- Supports both Bash and Zsh shells

## 🛠️ Installation

Run the setup script to compile and install `ptree` locally:

```bash
chmod +x setup.sh
./setup.sh
```
This will:

    Compile ptree.c

    Move the binary to ~/.local/bin

    Add it to your shell PATH if needed (~/.bashrc or ~/.zshrc)

Once installed, you can use ptree from anywhere.
## 📂 Usage
Show the tree for the current directory:
```sh
ptree
```
Show the tree for a specific path:
```sh
ptree /path/to/project
```
### Example Output:
```sh
myproject/
├── src/
│   ├── main.c (4.2 KB)
│   ├── utils.c (3.1 KB)
│   └── utils.h
├── README.md (1.2 KB)
└── Makefile
```
## 🧼 Uninstall

To remove the installed binary manually:
```sh
rm ~/.local/bin/ptree
```
Also remove the PATH entry in your .zshrc or .bashrc if desired.

Made with ❤️ by M4DF00l
