# 🏎️ MyShell

MyShell is a custom, lightweight shell environment built in C, specifically designed for **Linux** systems. It combines the core functionality of a Unix terminal with custom built-in utilities and a modern, user-friendly interface.

![preview](preview.png)

---

## 🚀 Key Features

* **Linux Native:** Optimized for Linux environments using standard system calls.
* **Command Execution:** Runs standard system binaries (e.g., `ls`, `grep`, `htop`) using `fork()` and `execvp()`.
* **Case-Insensitive Input:** Automatically handles commands in lowercase for a smoother experience.
* **Interactive Prompt:** Displays the current user and working directory. Includes a toggle to hide/show the path.
* **Command History:** Full support for arrow-key navigation and persistent history thanks to the [linenoise](https://github.com/antirez/linenoise) library.

---

## 🛠️ Built-in Commands

| Command | Action | Description |
| :--- | :--- | :--- |
| `cd <path>` | **Change Directory** | Navigates to the specified folder. |
| `up` | **Auto Update** | Chains `sudo apt update` and `upgrade -y`. |
| `hide` | **Minimalist Mode** | Hides the current path from the prompt. |
| `unhide` | **Standard Mode** | Restores the path visibility in the prompt. |
| `help` | **Manual** | Lists all available commands from the help file. |
| `version` | **Info** | Displays version, author, and GitHub info. |
| `ex` | **Exit** | Safely closes the shell and saves history. |

---

## 📥 Installation & Setup

### Prerequisites
You will need a C compiler (like `gcc`) and standard Linux headers.

### Compilation
Run the following command in your terminal:
```bash
gcc main.c lib/commands.c lib/support.c lib/linenoise.c -I./lib -o myshell 
```
```bash
./myshell
```

---

## 🚧 Development Status
This project is currently under development. I am constantly adding new features and refining the core engine. <br><br>

📌 To-Do List: Check our current progress and upcoming tasks on the [Notion Page](https://www.notion.so/myShell-294e266683b5806d9a95cfc18b091e47?source=copy_link). <br><br>

The project uses [linenoise](https://github.com/antirez/linenoise) to handle command history and line editing.
