/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bylee <bylee@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 19:59:10 by bylee             #+#    #+#             */
/*   Updated: 2021/10/24 16:13:25 by bylee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipeline.h"
int	nums_cmd = 2;

void	create_exe_proc(int idx_cmd, int **fd_pipe)
{
	if (idx_cmd == 0)
	{
		// char *str[3];
		// str[0] = "/bin/ls";
		// str[1] = "-l";
		// str[2] = NULL;
		// execve(str[0], str, NULL);
	}
	else if (idx_cmd == nums_cmd - 1)
	{
		// char *str[3];
		// str[0] = "/bin/cat";
		// str[1] = "-e";
		// str[2] = NULL;
		// execve(str[0], str, NULL);
	}	
	else ;
	exit(EXIT_SUCCESS);
}

void	create_cmd_proc(int idx_cmd, int **fd_pipe)
{
	pid_t	pid;

	if (idx_cmd == 0)
		dup2(fd_pipe[idx_cmd][1], STDOUT_FILENO);
	else if (idx_cmd == nums_cmd - 1)
		dup2(fd_pipe[idx_cmd - 1][0], STDIN_FILENO);
	else
	{
		dup2(fd_pipe[idx_cmd][1], STDOUT_FILENO);
		dup2(fd_pipe[idx_cmd - 1][0], STDIN_FILENO);
	}
	pid = fork();
	if (!pid)
		create_exe_proc(idx_cmd, fd_pipe);
	else
	{
		waitpid(pid, NULL, 0);
		exit(EXIT_SUCCESS);
	}
}

int	create_procs(int **fd_pipe)
{
	pid_t	*pid_cmd_proc;
	int		idx_cmd;

	idx_cmd = -1;
	pid_cmd_proc = (pid_t *)malloc(sizeof(pid_t) * nums_cmd);
	if (!pid_cmd_proc)
		return (MALLOC_FAILURE);
	while (++idx_cmd < nums_cmd)
	{
		pid_cmd_proc[idx_cmd] = fork();
		if (pid_cmd_proc[idx_cmd] < 0)
		{
			free_fd_table(nums_cmd, fd_pipe);
			return (PROCESS_ERROR);
		}
		else if (!pid_cmd_proc[idx_cmd])
			create_cmd_proc(idx_cmd, fd_pipe);
	}
	idx_cmd = -1;
	while (++idx_cmd < nums_cmd)
		waitpid(pid_cmd_proc[idx_cmd], NULL, 0);
	return (0);
}

int	build_pipeline(void)
{
	int	result;
	int	**fd_pipe;

	result = 0;
	fd_pipe = malloc_fd_table(nums_cmd);
	if (!fd_pipe)
		return (MALLOC_FAILURE);
	if (fill_fd_table(nums_cmd, fd_pipe))
		return (PIPE_FAILURE);
	if (create_procs(fd_pipe))
		return (PROCESS_ERROR);
	close_fd_table(nums_cmd, fd_pipe);
	free_fd_table(nums_cmd, fd_pipe);
	return (result);
}
