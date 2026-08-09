# 42_Minishell

## Names in data structure for the parsed data
cmd_data - struct of type t_cmd_data, holds the pointers to the "head" and the "tail" of the parsing data structure
cmd_data.head - points to head of linked list made of nodes of the type t_slingle_lined_node from libft (which contains a pointer to content and a pointer to the next node)
cmd - (content of linked list) the struct of type t_command that holds all information for one command inkluding the argv (array whos first string is the command and the next ones are the flags) and a pointer to the head of the linked list of redirections (redir)
redir - points to the head of redirection list of type t_single_linked_node
curr_redir - content that redir node points to (of type t_redir_list) that contains filename, fd (which will be filled by Nilsin the execution part) and redirection type (in, out, out + append, heredoc)

cmd_data(head(node), tail(node) - linked list) t_cmd_data (stack)
node(content, next - node of linked list) t_single_linked_node (heap)
cmd(argv, redir(node)) t_command (heap)
node(content, next - node of linked list) t_single_linked_node (heap)
curr_redir(filename, fd, redir_type) t_redir_list (heap)
filename() char * (heap)

## Quote removal and variable expansion
Posix (Shell variables IFS) "...the value of IFS is <space> <tab> <newline>..." - when to do field splitting
Posix (Field splitting) "...the shell shall scan the results of expansions and substitutions that did not occur in double-quotes for field splitting and multiple fields can result."
Posix (Name) "In the shell command language, a word consisting solely of underscores, digits, and alphabetics from the portable character set. The first character of a name is not a digit." - valid variable name
Posix (Parameter expansion) "If the parameter is not enclosed in braces, and is a name, the expansion shall use the longest valid name (see XBD Name), whether or not the variable represented by that name exists."
Decision: Every "word" has to go through the quote_rm_var_expan() function. No special cases for strings that don't contain quotes or $ to keep the code more simple.
### WORD
If single quote found - enter single quote mode (everything up to the next single quote gets added to the word)
if double quote found - enter double quote mode (var expansion gets added to the same word)
if $VAR found - expand and create mutiple words (field splitting only outside of quotes)
### REDIRECTION FILENAME
Same as above
Bash manual (Redirections) "If it expands to more than one word, Bash reports an error."
### HEREDOC
Posix (Here-Document) "If no part of word is quoted, all lines of the here-document shall be expanded for parameter expansion[...]"

## Syntax error in parsing
"syntax error near unexpected token"
- redirection token or pipe token is the last
- pipe token is the first
- two redirection tokens come in a row
- two pipe tokens come in a row

!!!!!!!Still to do!!!!!!!!!!!!!!!!!!!!!
- "$?" has to be implemented correctly (has a comment in it so it's easier to find)
- testing whether the whole quote removal and field splitting/var expansion works
- testing if the entire parsing works
- function to free the entire struct
- where are testfiles and quote removal and delete heredoc

## Signals

Ctrl+C - SIGINT
Ctrl+\ - SIGQUIT

### ctrl backslash
signal SIGQUIT ignored with signal() in main.c

### ctrl d
as in bash: when pressed on empty line in interactive mode it exits, in heredoc it delimits and prints a warning because it wasn't delimited correctly but command runs

### ctrl c
sig_atomic_t: An integer type which can be accessed as an atomic entity even in the presence of asynchronous interrupts made by signals.
An int might require two separate CPU instructions that can be interrupted by a signal.
The volatile keyword is used to inform the compiler that the value of a variable may change at any time, so it should read the value from memory every time it is used.

Variable: rl_hook_func_t * rl_signal_event_hook

    If non-zero, this is the address of a function to call if a read system call is interrupted by a signal when Readline is reading terminal input. 
