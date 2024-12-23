/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmin <junmin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 18:32:43 by junmin            #+#    #+#             */
/*   Updated: 2024/11/10 16:29:47 by junmin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exit_the_shell(t_minishell *mini, int count)
{
	if (count == 1)
	{
		printf("exit\n");
		free_all(mini);
		free_path_and_env(mini);
		exit(g_exit_status);
	}
}

int	get_count_argument(char **input)
{
	int	i;

	i = 0;
	while (input[i])
		i++;
	return (i);
}

static void	check_exit_args(t_minishell *mini, char **input, int count)
{
	if (get_count_argument(input) > 2)
	{
		print_error(NULL, "exit\nexit: too many arguments\n", 1);
		if (count == 1)
		{
			free_all(mini);
			free_path_and_env(mini);
			exit(1);
		}
	}
	else
	{
		g_exit_status = ft_atoi(input[1]) % 256;
		if (count == 1)
		{
			free_all(mini);
			free_path_and_env(mini);
			exit(g_exit_status);
		}
	}
}

static int	get_number_signal(char *arg)
{
	long long	num;

	if (!is_valid_digit_count(arg))
		return (0);
	num = 0;
	if (*arg == '+' || *arg == '-')
		arg++;
	while (*arg)
	{
		if (num > (LLONG_MAX - (*arg - '0')) / 10)
			return (0);
		num = num * 10 + (*arg++ - '0');
	}
	return (1);
}

void	command_exit(t_minishell *mini, char **input)
{
	int	count;

	count = mini->n_tokens2;
	if (get_count_argument(input) == 1)
		exit_the_shell(mini, count);
	else
	{
		if (get_number_signal(input[1]))
		{
			check_exit_args(mini, input, count);
		}
		else
		{
			print_error(NULL, "exit\nexit: numeric argument required\n", 2);
			if (count == 1)
			{
				free_all(mini);
				free_path_and_env(mini);
				exit(2);
			}
		}
	}
}
