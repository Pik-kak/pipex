/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkauhane <kkauhane@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 12:57:07 by kkauhane          #+#    #+#             */
/*   Updated: 2024/04/19 12:24:59 by kkauhane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <unistd.h>
# include <sys/types.h>
# include <fcntl.h>
# include "libft/libft.h"
# include <sys/wait.h>

char	*find_path(char *cmd, char **envp);
char	**parse(char *cmd);
void	print_error(char *argv, int i);

#endif