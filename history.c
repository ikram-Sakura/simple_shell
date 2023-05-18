#include "shell.h"

/**
 * get_history - gets the history file
 * @info: parameter struct
 *
 * Return: allocated string containg history file
 */

char *get_history(info_t *info)
{
	char *bufer, *dir;

	dir = _getenv(info, "HOME=");
	if (!dir)
		return (NULL);
	bufer = malloc(sizeof(char) * (_strlen(dir) + _strlen(HIST_FILE) + 2));
	if (!bufer)
		return (NULL);
	bufer[0] = 0;
	_strcpy(bufer, dir);
	_strcat(bufer, "/");
	_strcat(bufer, HIST_FILE);
	return (bufer);
}

/**
 * wr_history - creates a file, or appends to an existing file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int wr_history(info_t *info)
{
	ssize_t fd;
	char *filename = get_history(info);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = info->history; node; node = node->next)
	{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
	}
	_putfd(BUFer_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * rd_history - reads history from file
 * @info: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int rd_history(info_t *info)
{
	ssize_t fd, rdlen, size = 0;
	struct stat st;
	int i, last = 0, lines = 0;
	char *bufer = NULL, *filename = get_history(info);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		size = st.st_size;
	if (size < 2)
		return (0);
	bufer = malloc(sizeof(char) * (size + 1));
	if (!bufer)
		return (0);
	rdlen = read(fd, bufer, size);
	bufer[size] = 0;
	if (rdlen <= 0)
		return (free(bufer), 0);
	close(fd);
	for (i = 0; i < size; i++)
		if (bufer[i] == '\n')
		{
			bufer[i] = 0;
			build_history_list(info, bufer + last, lines++);
			last = i + 1;
		}
	if (last != i)
		build_history_list(info, bufer + last, lines++);
	free(bufer);
	info->histcount = lines;
	while (info->histcount-- >= HIST_MAX)
		rm_node(&(info->history), 0);
	renumber_history(info);
	return (info->histcount);
}

/**
 * renumber_history - renumbers the history linked list after changes
 * @info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int renumber_history(info_t *info)
{
	list_t *node = info->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (info->histcount = i);
}

/**
 * build_history_list - adds entry to a history linked list
 * @info: Structure containing potential arguments. Used to maintain
 * @bufer: bufer
 * @lines: the history lines, histcount
 *
 * Return: Always 0
 */
int build_history_list(info_t *info, char *bufer, int lines)
{
	list_t *node = NULL;

	if (info->history)
		node = info->history;
	add_to_end(&node, bufer, lines);

	if (!info->history)
		info->history = node;
	return (0);
}