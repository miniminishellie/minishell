/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihoolee <jihoolee@student.42SEOUL.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/18 20:07:34 by jihoolee          #+#    #+#             */
/*   Updated: 2021/11/17 21:40:11 by jihoolee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_heredoc(int idx_cmd, char *EOF_str)
{
	int		temp_fd;
	char	*filename;
	char	*line;

	filename = format_filename(idx_cmd);
	temp_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (temp_fd == INVALID_FD)
		error(FILE_OPEN_ERROR);
	while (1)
	{
		line = readline("> ");
		if (line == NULL \
			|| ft_strncmp(line, EOF_str, ft_strlen(EOF_str) + 1) == 0)
			break ;
		write(temp_fd, line, ft_strlen(line));
		write(temp_fd, "\n", 1);
		free(line);
	}
	if (line)
		free(line);
	close(temp_fd);
	temp_fd = open(filename, O_RDONLY);
	free(filename);
	return (temp_fd);
}

int	open_file(int idx_cmd, t_redirect *redir)
{
	int	fd;

	if (redir->type == TYPE_REDIR_HEREDOC)
		fd = open_heredoc(idx_cmd, redir->after_fd);
	else if (redir->type == TYPE_REDIR_STDIN)
		fd = open(redir->after_fd, O_RDONLY);
	else if (redir->type == TYPE_REDIR_STDOUT)
		fd = open(redir->after_fd, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd = open(redir->after_fd, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		if (redir->type == TYPE_REDIR_HEREDOC)
			error(HEREDOC_ERROR);
		else
		{
			write(STDERR_FILENO, "bash: ", 6);
			write(STDERR_FILENO, redir->after_fd, ft_strlen(redir->after_fd));
			write(STDERR_FILENO, ": No such file or directory.\n", 29);
		}
	}
	return (fd);
}

int	ft_itoa_fd(char *fd_char)
{
	int	fd;

	fd = 0;
	while (ft_isdigit(*fd_char))
	{
		fd = fd * 10 + (*fd_char - '0');
		fd_char++;
	}
	if (*fd_char != '\0')
		return (INVALID_FD);
	return (fd);
}

int	get_proc_fd(t_list *fd_table, char *fd_char)
{
	int	shell_fd;
	int	proc_fd;

	shell_fd = ft_itoa_fd(fd_char);
	if (shell_fd == INVALID_FD)
	{
		write(STDERR_FILENO, "bash: ", 6);
		write(STDERR_FILENO, fd_char, ft_strlen(fd_char));
		write(STDERR_FILENO, ": ambiguous redirect\n", 21);
		return (INVALID_FD);
	}
	proc_fd = search_proc_fd(fd_table, shell_fd);
	if (proc_fd == INVALID_FD)
	{
		write(STDERR_FILENO, "bash: ", 6);
		ft_putnbr_fd(shell_fd, STDERR_FILENO);
		write(STDERR_FILENO, ": Bad file descriptor\n", 22);
		return (INVALID_FD);
	}
	return (proc_fd);
}

t_error	handle_redir(int idx_cmd, t_list **fd_table, t_redirect *redir)
{
	int	redir_to_fd;

	if (redir->after_fd[0] == '&')
		redir_to_fd = get_proc_fd(*fd_table, redir->after_fd + 1);
	else
		redir_to_fd = open_file(idx_cmd, redir);
	if (redir_to_fd < 0)
		return (FILE_OPEN_ERROR);
	return (redirect(redir_to_fd, fd_table, redir));
}
