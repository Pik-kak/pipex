/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkauhane <kkauhane@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 16:42:56 by kkauhane          #+#    #+#             */
/*   Updated: 2024/04/09 17:27:19 by kkauhane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static char	*find_cmd1(char *cmd, char ***arg_out, int i, char c)
{
	char		*temp;

	temp = (char *)malloc(ft_strlen(cmd) + 1);
	if (!temp)
		return (NULL);
	c = *cmd;
	cmd++;
	while (*cmd != c && *cmd != '\0')
	{
		temp[i] = *cmd;
		i++;
		cmd++;
	}
	if (*cmd == c)
		cmd++;
	temp[i] = '\0';
	**arg_out = temp;
	return (cmd);
}

static char	*find_cmd2(char *cmd, char ***arg_out, int i)
{
	char		*temp;

	temp = (char *)malloc(ft_strlen(cmd) + 1);
	if (!temp)
		return (NULL);
	while (*cmd != ' ' && *cmd != '\0')
	{
		if (*cmd == '\\' && (*(cmd + 1) == '\'' || *(cmd + 1) == '"'))
			cmd++;
		temp[i] = *cmd;
		i++;
		cmd++;
	}
	temp[i] = '\0';
	**arg_out = temp;
	return (cmd);
}

static char	*find_cmd(char *cmd, char **cmd_part, int i, char c)
{
	if (*cmd == '\'' || *cmd == '"')
		return (find_cmd1(cmd, &cmd_part, i, c));
	else
		return (find_cmd2(cmd, &cmd_part, i));
}

char	**parse(char *cmd)
{
	char		**cmds;
	char		*cmd_part;
	int			i;
	char		*temp;

	cmds = malloc(4 * sizeof(char *));
	if (!cmds)
		return (NULL);
	i = 0;
	while (*cmd != '\0' && i < 3)
	{
		while (*cmd == ' ')
			cmd++;
		temp = find_cmd(cmd, &cmd_part, 0, 0);
		if (cmd_part)
		{
			cmds[i] = cmd_part;
			i++;
		}
		else
			free(cmd_part);
		cmd = temp;
	}
	cmds[i] = NULL;
	return (cmds);
}
