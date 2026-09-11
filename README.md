# Minishell

*This project has been created as part of the 42 curriculum by bastalze, nildruon.*

---

# Table of content
1. [Description](#description)
2. [Instructions](#instructions)
    - [Install and run](#install-and-run)
    - [Usage Guide](#usage-guide)
        - [Basic Commands](#basic-commands)
        - [Command History](#command-history)
        - [Signals and EOF](#signals-and-eof)
        - [Built-in Commands](#built-in-commands)
        - [Environment variables](#infos-about-environment-variables)
        - [Redirections](#redirections)
        - [Heredoc](#heredoc)
        - [Pipes](#pipes)
        - [Syntax Errors](#syntax-errors)
        - [Limitations](#limitations)
        - [Surpressed environment](#surpressed-environment)
3. [Resources](#resources)
    - [Online](#online)
    - [Help from peers](#help-from-peers)
    - [AI usage](#ai-usage)

---

# Description

Minishell is a minimalist Unix shell implementation written in C. The project aims to recreate a simplified version of Bash, providing a command-line interface where users can run commands, execute programs, navigate the filesystem and utilize built-in shell functionalities.

The minishell features: 
- A custom prompt displaying `"minishell> "`
- Command history which can be navigated with arrow keys
- Command execution with absolute or relative paths or via the `PATH` environment variable
- Built-in commands: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`
- Input/Output redirections and heredoc (`<`, `>`, `>>`, `<<`)
- Pipes (`|`) connecting multiple commands
- Environment variable expansion (`$VAR`)
- Signal handling (`Ctrl+C`, `Ctrl+D`, `Ctrl+\`)

This project provided us with a deeper understanding of process creation, file descriptors, signal handling and the inner workings of a command-line interpreter including input parsing.

---

# Instructions

## Install and run

1. Clone repository
2. To compile code run:
   ```bash
   make
   ```
   and then to run the program type:
   ```bash
   ./minishell
   ```
   in the root of the minishell repository.

The program takes no arguments.

To exit minishell press either `Ctrl+D` (EOF) or use the `exit` command.

## Usage Guide

### Basic Commands
- Type any command and press Enter to execute it
- Commands can be absolute paths (`/bin/ls´) or relative paths (`./myprogram`)
- Commands without a path are searched for in directories listed in the `$PATH` environment variable

### Command History
- Press ↑ and ↓ arrow keys to navigate through previously executed commands
- Press Enter to re-execute a selected command

### Signals and EOF

- **`Ctrl+\`**: Sends SIGQUIT. In interactive mode or heredoc mode it gets ignored. Interrupts the current running command and produces a core dump. With exit status of 131 (128 + sigal number of 3).
- **`Ctrl+C`**: Sends SIGINT. Interrupts the current running command, hereodc mode or half written command and displays a new prompt on a new line. With exit status of 130 (128 + sigal number of 2).
- **`Ctrl+D`**: Sends EOF (not a signal). In interactive and heredoc mode: When pressed on a non empty line nothing happens. On an empty line it delimits the heredoc and it exists the minishell in interactive mode.

### Built-in Commands

#### `echo`
- **Description:** Prints the text given as arguments into the terminal or into the specified fd if redirected
- **Usage:** `echo [-n] [text...]`
  - The `-n` option suppresses the trailing newline
  - Can input one or more strings as arguments

#### `cd`
- **Description:** Change working directory. Can be an abolute or relative PATH
- **Usage:** `cd [directory]`
  - Options: 
    - Without arguments, changes to `$HOME`
	- `~` : Also changes to `$HOME`, but it doenst handle like bash (bash uses getpwuid to get the HOME). (Therefore it works the exact same way as cd without arguments in our Minishell)
    - `.` : goes into the current directory
    - `..`: goes out of the current dir
    - `-` : goes to the previous working directory

#### `pwd`
- **Description:** Print the current working directory
- **Usage:** `pwd` [no options]

#### `export`
- **Description:** Set one or more environment variables (for the rules setting a var see: [About Env Vars](#infos-about-environment-variables))
- **Usage:** `export [VAR[=value]...] [VAR[=value]...] ...`

#### `unset`
- **Description:** Removes environment variables as params from the environment
- **Usage:** `unset [VAR] [VAR] ...`

#### `env`
- **Description:** Display all environment variables
- **Usage:** `env` [no options]

#### `exit`
- **Description:** Exit the shell with an optional status code
- **Usage:** `exit [code]`

<a id="environment-variables"></a>
### Infos about Environment variables
- Can be set using export as written above
- Variables are expanded with `$VAR` syntax
  - Example: `echo $HOME` displays the home directory path
- Rules for variable name:
  - First character: Can be an alphabetic character (A-Z or a-z) or an underscore (`_`)
  - Subsequent characters: Can be alphanumeric (A-Z, a-z, 0-9) or an underscore (`_`)
- Exit status of the last command is available via `$?`
- When a variable expands to multiple words the word gets split into multiple words if it's not in a heredoc or between double quotes

### Redirections
- `< file` - Redirect input from a file
- `> file` - Redirect output to a file (overwrites)
- `>> file` - Redirect output to a file (appends)
- `<< delimiter` - [Heredoc](#heredoc): read input until a line containing delimiter is encountered
- If the filename expands to more than one word, minishell reports an error.

### Heredoc
- `<< delimiter` - if no part of delimiter is quoated all lines of the here-document are expanded for parameter expansion

### Pipes
- **Description:** Connect the output of a command to the input next command
- **Usage:**
  - One pipe: `cmd1 | cmd2`
  - Multiple pipes: `cmd1 | cmd2 | cmd3`

### Syntax Errors
The shell detects basic syntax errors before sending it to execution such as:
- Unclosed quotes
- Consecutive operators (`||`, `<<<`, etc.)
- Operators at the start or end of a command (no multiline input)

### Limitations
- This shell is designed for interactive use with single-line commands. Pasting multi-line commands as well as piping content into the shell may result in undefined behavior.
- The minishell uses four dedicated memory arenas (64 KB each) to organize memory allocation by purpose:
  - Token structs with token type and pointer to word
  - Strings for word in token
  - Pointers to strings for variable expansions and field splitting
  - Strings for variable expansions and field splitting
- Heredoc delimiter is max 100 bytes which accommodates any realistic delimiter length
- Environment variable name is max 1 KB, anything longer would be impractical
- The temporary file used for heredoc input must not be deleted while the heredoc is being written. Deleting it during input collection will result in undefined behavior.

### Surpressed environment
When you start the minishell with the command `env -i ./minishell` a default environment is set in place. It contains:
- `PWD`
- `PATH=/usr/local/bin:/usr/bin:/bin`
- `SHLVL=1`

If the environment doesn't get surpressed, `SHLVL` gets updated with every new call of a shell within a shell.

---

# Resources

## Online
- [Bash Reference Manual](https://www.gnu.org/software/bash/manual/bash.html)
- [POSIX: The Open Group Base Specifications Issue 7, Chapter 2. Shell Command Language](https://pubs.opengroup.org/onlinepubs/9699919799/)
- [Wikipedia](https://en.wikipedia.org/wiki/Main_Page)
- [Geeks for Geeks](https://www.geeksforgeeks.org/c/c-programming-language/)
- [Stack Overflow](https://stackoverflow.com/questions)
- [GNU Readline Library](https://tiswww.case.edu/php/chet/readline/readline.html)
- [GNU History Library](https://tiswww.case.edu/php/chet/readline/history.html)
- [Minishell Tester](https://github.com/LeaYeh/42_minishell_tester)

## Help from peers
- We were discussing concepts with Kian, David, Stefan A., Stefan L., Michal C., Dorian and others
- Got help debugging from Kian, Myron, Veja, David Stefan A.
- Stefan A. also helped with implementing the arenas in an already existing data structure for tokenization and variable expansion

## AI usage
- llms used: 
  - deepseek.com
  - gemini.com 
- To discuss concepts and implementation ideas
- Clear up confusions with minor issues in code like if conditions
- Help create the structure of this README