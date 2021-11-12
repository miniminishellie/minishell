/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jihoolee <jihoolee@student.42SEOUL.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/13 00:19:00 by jihoolee          #+#    #+#             */
/*   Updated: 2021/11/13 02:14:07 by jihoolee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_error	redirect(int redir_to_fd, t_list **fd_table, t_redirect *redir)
{
	int		from_proc_fd;
	t_list	*fd_node;

	from_proc_fd = search_proc_fd(*fd_table, redir->before_fd);
	if (from_proc_fd == INVALID_FD)
	{
		fd_node = ft_lstnew(fd_new(redir->before_fd, redir_to_fd));
		if (fd_node == NULL)
			error(MALLOC_ERROR);
		ft_lstadd_back(fd_table, fd_node);
	}
	else
	{
		if (dup2(redir_to_fd, from_proc_fd) == INVALID_FD)
			return (INVALID_FD);
	}
	return (SUCCESS);
}
