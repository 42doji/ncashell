/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junmin <junmin@student.42gyeongsan.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 13:43:27 by junmin            #+#    #+#             */
/*   Updated: 2024/11/10 16:02:22 by junmin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_shell(t_minishell *mini)
{
	t_token	*token;

	mini->flag2 = 0;
	mini->error = 0;
    if (mini->str)
		add_history(mini->str);
	lexer(mini);
	if (check_unfinished_quote(mini->input) == 1)
		return ;
	replace_env_var(mini);
	tokenizer(mini);
	token = mini->token;
	if (has_syntax_error(token) == 1)
		return ;
	parse_command(mini, token);
	execution(mini);
}
