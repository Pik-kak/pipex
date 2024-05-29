/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kkauhane <kkauhane@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 12:55:35 by kkauhane          #+#    #+#             */
/*   Updated: 2024/04/12 14:20:19 by kkauhane         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	execute_command(char *cmd, char **envp)
{
	char	**cmds;
	char	*path;
	int		i;

	i = 0;
	cmds = parse(cmd);
	if (access(cmds[0], X_OK) == 0)
		path = cmds[0];
	else
		path = find_path(cmds[0], envp);
	if (!path)
	{
		while (cmds[i])
		{
			free(cmds[i]);
			i++;
		}
		free(cmds);
		print_error(cmd, 2);
		exit (127);
	}
	if (execve(path, cmds, envp) == -1)
		exit (1);
}

static void	call_child1(char *argv[], char **envp, int *pipe_ends)
{
	int	infile;

	if (access(argv[1], R_OK) == -1)
	{
		print_error(argv[1], 1);
		exit(1);
	}
	infile = open(argv[1], O_RDONLY, 0666);
	if (infile == -1)
	{
		print_error(argv[1], 1);
		exit (1);
	}
	dup2(infile, STDIN_FILENO);
	close(infile);
	dup2(pipe_ends[1], STDOUT_FILENO);
	close(pipe_ends[0]);
	close(pipe_ends[1]);
	execute_command(argv[2], envp);
}

static void	call_child2(char *argv[], char **envp, int *pipe_ends)
{
	int	outfile;

	outfile = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (outfile == -1)
	{
		print_error(argv[4], 1);
		exit(1);
	}
	dup2(outfile, STDOUT_FILENO);
	dup2(pipe_ends[0], STDIN_FILENO);
	close(outfile);
	close(pipe_ends[0]);
	close(pipe_ends[1]);
	execute_command(argv[3], envp);
}

void	pipex(char *argv[], char **envp, int *pipe_ends)
{
	pid_t	child1;
	pid_t	child2;
	int		status;

	child1 = fork();
	if (child1 == -1)
		exit(1);
	if (child1 == 0)
		call_child1(argv, envp, pipe_ends);
	child2 = fork();
	if (child2 == -1)
		exit(1);
	if (child2 == 0)
		call_child2(argv, envp, pipe_ends);
	close(pipe_ends[0]);
	close(pipe_ends[1]);
	waitpid(child1, &status, 0);
	waitpid(child2, &status, 0);
	if (WIFEXITED(status))
		exit(WEXITSTATUS(status));
	else
		exit(1);
}

int	main(int argc, char *argv[], char **envp)
{
	int		pipe_ends[2];
	char	*def_envp[2];	

	def_envp[0] = "PATH=/bin:/usr/bin";
	def_envp[1] = NULL;
	if (envp == NULL || envp[0] == NULL)
		envp = def_envp;
	if (argc != 5 || pipe(pipe_ends) == -1)
		exit (1);
	pipex(argv, envp, pipe_ends);
}
