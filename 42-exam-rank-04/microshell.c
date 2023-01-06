#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define SIDOUT 0
#define SIDIN 1

typedef struct s_cmds
{
	char **cmd;
	int pipe[2];
	int pipe_before;
	int pipe_after;
	struct s_cmds *begin;
	struct s_cmds *next;
} t_cmds;

void putstr_fd(char *str, int fd)
{
	for (int i = 0; str[i]; i++)
		write(fd, &str[i], 1);
}

void fatal_error()
{
	putstr_fd("error: fatal", 2);
	exit(1);
}

int cd_cmd(char *cmd[])
{
	if (!cmd[1] || cmd[2])
	{
		putstr_fd("error: cd: bad arguments\n", 2);
		return 1;
	}
	if (chdir(cmd[1]) < 0)
	{
		putstr_fd("error: cd: cannot change directory to ", 2);
		putstr_fd(cmd[1], 2);
		putstr_fd("\n", 2);
		return 1;
	}
	return 0;
}

int count_arg(char *args[])
{
	int i = 0;

	while (args[i] && strcmp(args[i], "|") && strcmp(args[i], ";"))
		i++;
	return i;
}

t_cmds *alloc_elem()
{
	t_cmds *elem;

	elem = malloc(sizeof(t_cmds));
	if (!elem)
		fatal_error();
	elem->begin = NULL;
	elem->next = NULL;
	elem->cmd = NULL;
	elem->pipe_after = 0;
	elem->pipe_before = 0;
	return elem;
}

t_cmds *pars_arg(char *argv[])
{
	t_cmds *cmds;
	t_cmds *elem;
	t_cmds *tmp;
	int i = 1;
	int size = 0;

	elem = alloc_elem();
	cmds = elem;
	while (argv[i])
	{
		size = count_arg(&argv[i]);
		elem->cmd = malloc((size + 1) * sizeof(char *));
		if (!elem->cmd)
			fatal_error();
		for (int j = 0; j < size; j++)
			elem->cmd[j] = argv[i++];
		elem->cmd[size] = NULL;
		if (argv[i])
		{
			if (!strcmp(argv[i], "|"))
				elem->pipe_after = 1;
			tmp = elem;
			elem = alloc_elem();
			if (tmp->pipe_after)
				elem->pipe_before = 1;
			tmp->next = elem;
			elem->begin = tmp;
			i++;
		}
	}
	return cmds;
}

int exec_cmd(t_cmds *cmd, char *env[])
{
	pid_t pid;
	int status = 0;

	if (!strcmp(cmd->cmd[0], "cd"))
		return cd_cmd(cmd->cmd);
	pid = fork();
	if (pid < 0)
		fatal_error();
	else if (pid == 0)
	{
		if (cmd->pipe_after)
			dup2(cmd->pipe[SIDIN], 1);
		if (cmd->pipe_before)
			dup2(cmd->begin->pipe[SIDOUT], 0);
		status = execve(cmd->cmd[0], cmd->cmd, env);
		if (status < 0)
		{
			putstr_fd("error: cannot execute ", 2);
			putstr_fd(cmd->cmd[0], 2);
			putstr_fd("\n", 2);
			status = 127;
		}
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		if (cmd->pipe_after)
		{
			close(cmd->pipe[SIDIN]);
			// printf("close STDIN\n");
		}
		if (cmd->pipe_before)
		{
			close(cmd->begin->pipe[SIDOUT]);
			// printf("close STDOUT\n");
		}
	}
	return status;
}

int loop(t_cmds *cmds, char *env[])
{
	int status = 0;

	while (cmds)
	{
		if (cmds->pipe_after)
			pipe(cmds->pipe);
		if (cmds->cmd && cmds->cmd[0])
			status = exec_cmd(cmds, env);
		cmds = cmds->next;
	}
	return status;
}

int main(int argc, char *argv[], char *env[])
{
	int status;
	t_cmds *cmds;
	t_cmds *tmp;

	if (argc == 1)
		return 0;
	cmds = pars_arg(argv);
	status = loop(cmds, env);
	while (cmds)
	{
		// printf(":");
		// for (int i = 0; cmds->cmd[i]; i++)
		// 	printf("%s\n", cmds->cmd[i]);
		// printf("%d\n", cmds->pipe_after);
		// printf("%d\n", cmds->pipe_before);
		free(cmds->cmd);
		tmp = cmds;
		cmds = cmds->next;
		free(tmp);
	}
	return status;
}
