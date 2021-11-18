#include "minishell.h"

extern t_minishell	g_shell;

void	execute_line(t_AST_Node *AST)
{
	if (AST != NULL)
	{
		if (g_shell.cmd_cnt > 1)
			build_pipeline(AST, g_shell.cmd_cnt);
		else
			redir_and_exe(0, AST);
	}
}

void	loop_minishell(struct termios *org, struct termios *new)
{
	t_AST_Node	*AST;

	while (1)
	{
		if (set_term_mode(new) == TERMIOS_ERROR)
			error(TERMIOS_ERROR);
		g_shell.getting_cmd = 1;
		g_shell.line = readline("minishell> ");
		if (g_shell.line == NULL)
		{
			write(STDOUT_FILENO, "\033[1A", 4);
			write(STDOUT_FILENO, "\nminishell> exit\n", 17);
			break ;
		}
		g_shell.getting_cmd = 0;
		if (set_term_mode(org) == TERMIOS_ERROR)
			error(TERMIOS_ERROR);
		add_history(g_shell.line);
		AST = interpreter(g_shell.line);
		execute_line(AST);
		destruct_AST(AST);
		free(g_shell.line);
	}
}

void	handler(int signum)
{
	if (signum == SIGQUIT)
		return ;
	else if (signum == SIGINT && g_shell.getting_cmd == 1)
	{
		write(STDOUT_FILENO, "\n", 1);
		if (rl_on_new_line() == -1)
			error(RL_ERROR);
		rl_replace_line("", 1);
		rl_redisplay();
	}
	else if (signum == SIGINT && g_shell.getting_cmd == 2)
		return ;
}

int	main(int argc, char *argv[], char *envp[])
{
	struct termios	org_term;
	struct termios	new_term;

	(void)argc;
	(void)argv;
	signal(SIGINT, handler);
	signal(SIGQUIT, handler);
	if (init_envp(envp) == MALLOC_ERROR)
		error(MALLOC_ERROR);
	if (init_term(&org_term, &new_term) == TERMIOS_ERROR)
		error(TERMIOS_ERROR);
	loop_minishell(&org_term, &new_term);
	ft_lstclear(&(g_shell.export_list), free);
	ft_lstclear(&(g_shell.env_list), free);
	return (0);
}
