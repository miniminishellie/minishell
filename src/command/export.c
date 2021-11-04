#include "command.h"
#include "libft.h"

void	convert_env(char *arg, t_list *env_list)
{
	char	**value;
	char	**envp;

	value = ft_split(arg, '=');
	if (value == NULL)
		malloc_error();
	while (env_list)
	{
		envp = ft_split((char *)(env_list->content), '=');
		if (envp == NULL)
			malloc_error();
		if (ft_strncmp(envp[0], value[0], ft_strlen(value[0])) == 0
			&& (ft_strchr(arg, '=') != NULL))
		{
			env_list->content = arg;
			free_str(envp);
			free_str(value);
			return ;
		}
		env_list = env_list->next;
		free_str(envp);
	}
	free_str(value);
}

int	update_envp(char **envp, t_list *export_list, t_list *env_list, char *arg)
{
	char	**value;

	value = ft_split(arg, '=');
	if (value == NULL)
		malloc_error();
	if (ft_strncmp(envp[0], value[0], ft_strlen(value[0])) == 0
		&& (ft_strchr(arg, '=') != NULL))
	{
		export_list->content = arg;
		convert_env(arg, env_list);
		export_list = export_list->next;
		free_str(value);
		free_str(envp);
		return (END);
	}		
	else if (ft_strncmp(envp[0], value[0], ft_strlen(value[0])) == 0
		&& (ft_strchr(arg, '=') == NULL))
	{
		export_list = export_list->next;
		free_str(value);
		free_str(envp);
		return (END);
	}
	free_str(value);
	return (0);
}

int	check_dup(char *arg, t_list *export_list, t_list *env_list)
{
	char	**envp;
	int		result;

	while (export_list)
	{
		envp = ft_split(export_list->content, '=');
		result = update_envp(envp, export_list, env_list, arg);
		if (result == END)
			return (END);
		export_list = export_list->next;
		free_str(envp);
	}
	return (0);
}

void	append_envp(t_list *export_list, t_list *env_list, void *arg)
{
	if (ft_strchr((char *)arg, '=') != NULL)
	{
		if (check_dup((char *)arg, export_list, env_list) == 0)
		{
			add_to_list((char *)arg, export_list);
			add_to_list((char *)arg, env_list);
		}
	}
	else
	{
		if (check_dup((char *)arg, export_list, env_list) == 0)
			add_to_list((char *)arg, export_list);
	}	
}

int	run_export(t_list *args, t_list *export_list, t_list *env_list)
{
	char	*temp;
	int		i;
	int		len;

	i = 0;
	len = ft_lstsize(args);
	if (len == 0)
		print_export_list(export_list);
	else
	{
		while (args)
		{
			append_envp(export_list, env_list, args->content);
			args = args->next;
		}
	}
	printf("----------------------------------------------------\n");
	get_env(env_list);
	print_export_list(export_list);
	return (0);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	t_list	*export_list;
// 	t_list	*env_list;
// 	(void)argc;
// 	(void)argv;
// 	t_list	*args;
// 	t_list	*new;
// 	t_list	*new2;

// 	args = ft_lstnew("a====");
// 	new = ft_lstnew("b");
// 	new2 = ft_lstnew("v=c");
// 	if (args == NULL || new == NULL | new2 == NULL)
// 		return (0);
// 	ft_lstadd_back(&args, new);
// 	ft_lstadd_back(&args, new2);

// 	export_list = init_envp(envp);
// 	env_list = init_envp(envp);
// 	run_export(args, export_list, env_list);
// 	while(1){};
// 	return (0);
// }