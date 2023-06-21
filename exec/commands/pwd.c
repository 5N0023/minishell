/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbennani <mbennani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/08 21:21:37 by mlektaib          #+#    #+#             */
/*   Updated: 2023/05/14 21:08:51 by mbennani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "commands.h"

char	*return_pwd(void)
{
	char	*buffer;
	size_t	size;
	char	*re_buffer;

	buffer = NULL;
	size = 0;
	re_buffer = NULL;
	while (1)
	{
		size += 1024;
		re_buffer = (char *)malloc(size);
		if (re_buffer == NULL)
		{
			free(buffer);
			return (NULL);
		}
		if (getcwd(re_buffer, size) != NULL)
		{
			free(buffer);
			buffer = re_buffer;
			break ;
		}
		free(re_buffer);
	}
	return (buffer);
}

int	pwd(t_env **env)
{
	char	*buffer;

	if (get_env(*env, "PWD"))
	{
		ft_putendl_fd(get_env(*env, "PWD")->value, 1);
		return (0);
	}
	else
	{
		buffer = return_pwd();
		if (buffer == NULL)
			return (1);
		ft_putendl_fd(buffer, 1);
		envnew("PWD", buffer, 0);
	}
	return (0);
}
