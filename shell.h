#ifndef SHELL_H
#define SHELL_H

/* Done by youssef && ikram */


#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>

/* MACROS FOR READ AND WRITE BUFFERS */

#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* MACROS FOR COMMAND FOR CHAINING */

#define CMD_NORM 0
#define CMD_OR 1
#define CMD_AND 2
#define CMD_CHAIN 3

/* MACROS FOR CONVERT NUMBERS */

#define CONVERT_LOWERCASE 1
#define CONVERT_UNSIGNED 2

/* MACROS FOR USING GETLINE */

#define USE_GETLINE 0
#define USE_STRTOK 0

#define HIST_FILE ".simple_shell_history"
#define HIST_MAX 4096

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, \
	NULL, 0, 0, 0}


extern char **environ;

/**
 * struct liststr - linked list
 * @num: field number
 * @str: string
 * @next: pointer to the next node
 */

typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 * struct passinfo - contains arguments to pass into a function,
 * @arg: string generate from getline contains arguments
 * @argv: an array of string generate from arg
 * @path: string path for the current command
 * @argc: argument count
 * @line_count: error count
 * @err_num: error code for exit
 * @linecount_flag: count line of inputs
 * @fname: filename
 * @env: linked list for copu of env
 * @environ: custom modified copy of env
 * @history: history node
 * @alias: alias node
 * @env_changed: if env was changed
 * @status: return status of the list exec'd command
 * @cmd_buf: address point to cmd_buf
 * @cmd_buf_type: cmd_type ||,, &&, ;
 * @readfd: fd from which read line input
 * @histcount: the history line number count
 */

typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;
	char **cmd_buf;   /*pointer to cmd for memory management*/
	int cmd_buf_type; /* for || , &&, ;*/
	int readfd;
	int histcount;
} info_t;

/**
 *struct builtin - contains builtin string
 *@type: builtin command flag
 *@func: function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;

/* shell-tools.c contains all func needed for simple-shell project */


void _find_cmd(info_t *);
void _fork_cmd(info_t *);

int _hsh(info_t *, char **);
int _find_builtin(info_t *);
/* shell-tools1.c continus of tools */

/* shell-parser.c contains command parser */
int _is_cmd(info_t *, char *);
char *_dup_chars(char *, int, int);
char *_find_path(info_t *, char *, char *);

/* looper.c */

int _loophsh(char **);

/* error-handler.c */
void _eputs(char *);
int _eputchar(char);
int _putfd(char, int);
int _putsfd(char *, int);

/* string-handler.c */
int _strcmp(char *, char *);
char *_starts_with(const char *, const char *);
int _strlen(char *);
char *_strcat(char *, char *);

/* string-handler1.c */
char *_strdup(const char *);
int _putchar(char);
void _puts(char *);
char *_strcpy(char *, char *);

/* string-handler2.c */
char *_strncat(char *, char *, int);
char *_strchr(char *, char);
char *_strncpy(char *, char *, int);

/* shell-tokenizer.c */
char **_strtow2(char *, char);
char **_strtow(char *, char *);

/* memory-handler.c */
void _ffree(char **);
int _bfree(void **);
void *_realloc(void *, unsigned int, unsigned int);
char *_memset(char *, char, unsigned int);

/* shell-atoi.c */
int _interactive(info_t *);
int _atoi(char *);
int _is_delim(char, char *);
int _isalpha(int);

/* error-handler1.c */
void _remove_comment(char *);
void _print_error(info_t *, char *);
int _erratoi(char *);
int _print_d(int, int);
char *_convert_number(long int, int, int);

/* shell-builtin.c */
int _mycd(info_t *);
int _myexit(info_t *);


/* shell-builtin1.c */
int _set_alias(info_t *, char *);
int _unset_alias(info_t *, char *);
int _myalias(info_t *);
int _print_alias(list_t *);
int _myhistory(info_t *);

/* shell-getline.c */
void _sigintHandler(int);
int _getline(info_t *, char **, size_t *);
ssize_t _get_input(info_t *);

/* shell-getinfo.c */
void _set_info(info_t *, char **);
void _free_info(info_t *, int);
void _clear_info(info_t *);
/* shell-environ.c */
int _myenv(info_t *);
int _populate_env_list(info_t *);
int _mysetenv(info_t *);
char *_getenv(info_t *, const char *);
int _myunsetenv(info_t *);

/* shell-getenv.c */
int _setenv(info_t *, char *, char *);
int _unsetenv(info_t *, char *);
char **_get_environ(info_t *);

/* shell-history.c */
char *_get_history_file(info_t *);
int read_history(info_t *);
int _renumber_history(info_t *);
int _build_history_list(info_t *, char *, int);
int _write_history(info_t *);

/* shell-linked.c */

list_t *_add_node_end(list_t **, const char *, int);
size_t _print_list_str(const list_t *);
list_t *_add_node(list_t **, const char *, int);
void _free_list(list_t **);
int _delete_node_at_index(list_t **, unsigned int);

/* shell-linked1.c */
size_t _list_len(const list_t *);
ssize_t _get_node_index(list_t *, list_t *);
size_t _print_list(const list_t *);
list_t *_node_starts_with(list_t *, char *, char);
char **_list_to_string(list_t *);

/* shell-variables.c */

int _replace_string(char **, char *);
void _check_chain(info_t *, char *, size_t *, size_t, size_t);
int _is_chain(info_t *, char *, size_t *);
int _replace_vars(info_t *);
int _replace_alias(info_t *);

#endif
