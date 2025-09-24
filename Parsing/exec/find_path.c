/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: eprottun <eprottun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 15:53:46 by eprottun          #+#    #+#             */
/*   Updated: 2025/09/24 12:25:24 by eprottun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_find_paths(char *envp[], char *env_name)
{
	int	iter;

	iter = 0;
	if (!envp)
		return (-1);
	while (envp[iter])
	{
		if (ft_strncmp(envp[iter], env_name, ft_strlen(env_name)) == 0)
			if (envp[iter][ft_strlen(env_name)] == '=')
				return (iter);
		iter++;
	}
	return (-1);
}

char	*ft_strjointhree(char const *s1, char const *s2, char const *s3)
{
	char	*str_join;
	size_t	i;
	size_t	j;
	size_t	k;

	i = 0;
	j = 0;
	k = 0;
	str_join = malloc(sizeof(char) * (ft_strlen(s1)
				+ ft_strlen(s2) + ft_strlen(s3) + 1));
	if (!str_join)
		return (NULL);
	while (s1[i])
	{
		str_join[i] = s1[i];
		i++;
	}
	while (s2[j])
		str_join[i++] = s2[j++];
	while (s3[k])
		str_join[i++] = s3[k++];
	str_join[i] = '\0';
	return (str_join);
}

int	ft_search_paths(char **paths, char **tmp_path, char *cmd)
{
	size_t	iter;
	int		f_no_x;

	iter = 0;
	f_no_x = 0;
	*tmp_path = NULL;
	while (paths[iter])
	{
		*tmp_path = ft_strjointhree(paths[iter], "/", cmd);
		if (*tmp_path == NULL)
			return (ENOMEM);
		if (access(*tmp_path, X_OK) == 0)
			return (0);
		else if (errno != ENOENT && errno != EACCES)
			return (free(*tmp_path), *tmp_path = NULL, errno);
		else if (errno == EACCES)
			f_no_x = 1;
		free(*tmp_path);
		*tmp_path = NULL;
		iter++;
	}
	if (f_no_x)
		return (EACCES);
	return (ENOENT);
}

char	*ft_getpath(char **envp, char *cmd)
{
	int		path_pos;
	char	**paths;
	char	*tmp_path;
	int		success_bool;

	if (ft_strchr(cmd, '/'))
		return (cmd);
	path_pos = ft_find_paths(envp, "PATH");
	if (!path_pos)
		return (/*own error*/NULL);
	paths = ft_split(&envp[path_pos][5], ':');
	if (paths == NULL)
		return (NULL);
	success_bool = ft_search_paths(paths, &tmp_path, cmd);
	free2d(&paths);
	if (success_bool != 0)
		return (errno = success_bool, NULL);
	return (tmp_path);
}
