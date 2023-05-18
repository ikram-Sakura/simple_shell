#ifndef _SHELL_H_
#define _SHELL_H_

#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <limits.h>
#include <string.h>


/* for num_converter() */
#define CONVERT_UNSIGNED	2
#define CONVERT_LOWERCASE	1

/* for command chaining */
#define CMD_OR		1
#define CMD_NORM	0
#define CMD_CHAIN	3
#define CMD_AND		2


/* 1 if using system getline() */
#define USE_STRTOK 0
#define USE_GETLINE 0

/* for read/write buffers */
#define BUF_FLUSH -1
#define WRITE_BUF_SIZE 1024
#define READ_BUF_SIZE 1024

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @str: a string
 * @next: points to the next node
 * @num: the number field
 */
typedef struct liststr
{
	char *str;
	int num;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@histcount: the history line number count
 *@argv: an array of strings generated from arg
 *@argc: the argument count
 *@path: a string path for the current command
 *@err_num: the error code for exit()s
 *@env: linked list local copy of environ
 *@readfd: the fd from which to read line input
 *@history: the history node
 *@linecount_flag: if on count this line of input
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@fname: the program filename
 *@line_count: the error count
 *@environ: custom modified copy of environ from LL env
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@alias: the alias node
 *@cmd_buf_type: CMD_type ||, &&, ;
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	char *fname;
	char **environ;
	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	unsigned int line_count;
	int argc;
	int err_num;
	int linecount_flag;
	int env_changed;
	int status;
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
	list_t *env;
	list_t *history;
	list_t *alias;

} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	int (*func)(info_t *);
	char *type;
} builtin_table;


/* shloop.c */
void fork_cm(info_t *);
int builtinfinder(info_t *);
void find_cm(info_t *);
int msl(info_t *, char **);

/* parser.c */
char *find_path(info_t *, char *, char *);
char *dup_chars(char *, int, int);
int is_exe(info_t *, char *);

/* loophsh.c */
int loophsh(char **);

/* errors.c */
int _putsfd(char *str, int fd);
int _putfd(char c, int fd);
int _putchar(char);
void _puts(char *);

/* _utils.c */
char *_strcat(char *, char *);
int _strcmp(char *, char *);
char *starts_with(const char *, const char *);
int _strlen(char *);

/* utils.c*/
int _putchar(char);
char *_strdup(const char *);
char *_strcpy(char *, char *);
void _puts(char *);

/* exits.c */
char *_strchr(char *, char);
char *_strncat(char *, char *, int);
char *_strncpy(char *, char *, int);

/* tokenizer.c */
char **_split_w(char *, char);
char **split_w(char *, char *);

/* realloc.c */
void *_realloc(void *, unsigned int, unsigned int);
void _free(char **);
char *_memset(char *, char, unsigned int);

/* mem.c */
int bfree(void **);

/* __util.c */
int _atoi(char *);
int _isalpha(int);
int is_delimeter(char, char *);
int interactive(info_t *);

/*_errors.c */
void rm_comments(char *);
int print_d(int, int);
void print_error(info_t *, char *);
char *num_converter(long int, int, int);
int ft_error(char *);

/* builtins.c */
int helper(info_t *);
int ft_exit(info_t *);
int ft_cd(info_t *);

/* _builtins.c */
int alias_clone(info_t *);
int ft_history(info_t *);

/*getline.c */
void sig_Handler(int);
ssize_t get_input(info_t *);
int _getline(info_t *, char **, size_t *);

/* getinfo.c */
void free_info(info_t *, int);
void set_info(info_t *, char **);
void cl_info(info_t *);

/* _env.c */
int _setenv(info_t *);
int _env(info_t *);
int populate_env(info_t *);
int _unsetenv(info_t *);
char *_getenv(info_t *, const char *);

/* env.c */
int _setenv(info_t *, char *, char *);
int _unsetenv(info_t *, char *);
char **get_environ(info_t *);

/* history.c */
int wr_history(info_t *info);
int build_history_list(info_t *info, char *buf, int linecount);
int renumber_history(info_t *info);
int rd_history(info_t *info);
char *get_history(info_t *info);

/* l_lists.c */
void free_list(list_t **);
list_t *add_to_end(list_t **, const char *, int);
int rm_node(list_t **, unsigned int);
size_t print_list_str(const list_t *);
list_t *add_node(list_t **, const char *, int);

/* l__lists.c */
ssize_t get_node_index(list_t *, list_t *);
size_t print_list(const list_t *);
size_t list_len(const list_t *);
list_t *node_starter(list_t *, char *, char);
char **list_to_strings(list_t *);

/* var.c */
int ov_vars(info_t *);
int ov_alias(info_t *);
int replace_str(char **, char *);
void checkchain(info_t *, char *, size_t *, size_t, size_t);
int is_chaindel(info_t *, char *, size_t *);

#endif
