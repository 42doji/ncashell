/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmin <junmin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 13:43:56 by junmin            #+#    #+#             */
/*   Updated: 2024/11/10 14:49:48 by junmin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int init_new_fd(t_fd **fd)
{
	*fd = (t_fd *)malloc(sizeof(t_fd));
	if (!(*fd))
		return (0);
	(*fd)->in = -1;
	(*fd)->out = -1;
	(*fd)->next = NULL;
	return (1);
}

static int setup_fd_io(t_fd *fd)
{
	fd->in = dup(STDIN_FILENO);
	if (fd->in == -1)
		return (0);
	fd->out = dup(STDOUT_FILENO);
	if (fd->out == -1)
	{
		close(fd->in);
		return (0);
	}
	return (1);
}

static int handle_fd_next(t_fd **fd, t_file *file, t_command **temp, int i)
{
	if (file->next || temp[i + 1])
	{
		(*fd)->next = (t_fd *)malloc(sizeof(t_fd));
		if (!(*fd)->next)
			return (0);
		*fd = (*fd)->next;
		(*fd)->in = -1;
		(*fd)->out = -1;
		(*fd)->next = NULL;
	}
	return (1);
}

static void create_out_dup_list(t_minishell *mini)
{
	t_fd *fd;
	int i;

	if (!init_new_fd(&fd))
		return;
	mini->fd = fd;
	i = -1;
	while (mini->parsed[++i])
	{
		t_file *file = mini->parsed[i]->file;
		while (file)
		{
			if (!setup_fd_io(fd) || !handle_fd_next(&fd, file, mini->parsed, i))
			{
				free_fd_list(mini);
				return;
			}
			file = file->next;
		}
	}

}

static void	process_redirection(t_token **token, t_command **cur)
{
	t_file	*file;
	t_file	*tmp_list;

	tmp_list = (*cur)->file;
	file = (t_file *)malloc(sizeof(t_file));
	file->next = NULL;
	file->type = (*token)->type;
	(*token) = (*token)->next;
	file->name = ft_strdup((*token)->value);
	if (!tmp_list)
		(*cur)->file = file;
	else
	{
		while (tmp_list->next)
			tmp_list = tmp_list->next;
		tmp_list->next = file;
	}
	(*token) = (*token)->next;
}

static t_command	*create_next_command(t_token **tkns, t_command **olds)
{
	t_command	*new_cmd;

	new_cmd = create_command((*olds)->in_file, (*olds)->out_file);
	(*olds)->next = new_cmd;
	new_cmd->prev = (*olds);
	(*tkns) = (*tkns)->next;
	return (new_cmd);
}

static void	add_argument(t_token **token, t_command **cur)
{
	int		i;
	char	**new_arguments;

	i = 0;
	if (!((*cur)->cmd))
	{
		(*cur)->cmd = ft_calloc(ft_strlen((*token)->value) + 2, \
							sizeof(char));
		ft_strcpy((*cur)->cmd, (*token)->value);
	}
	while ((*cur)->args[i])
		i++;
	new_arguments = ft_calloc((i + 2), sizeof(char *));
	new_arguments[i + 1] = NULL;
	new_arguments[i] = ft_strdup((*token)->value);
	while (i--)
		new_arguments[i] = ft_strdup((*cur)->args[i]);
	free_array((*cur)->args);
	(*cur)->args = new_arguments;
	(*token) = (*token)->next;
}

void	parse_command(t_minishell *mini, t_token *token)
{
	t_command	**list;
	t_command	*cur;

	list = ft_calloc(cal_input_token(mini) + 1, sizeof(t_command *));
	while (token)
	{
		cur = create_command(0, 1);
		add_command_to_list(cur, list);
		while (token)
		{
			if (token->type >= 1 && token->type <= 4 && token->next)
				process_redirection(&token, &cur);
			else if (token->type == STRING)
				add_argument(&token, &cur);
			else if (token->type == PIPE && token->next)
			{
				cur = create_next_command(&token, &cur);
				add_command_to_list(cur, list);
			}
			else
				token = token->next;
		}
	}
	mini->parsed = list;
	create_out_dup_list(mini);
}
