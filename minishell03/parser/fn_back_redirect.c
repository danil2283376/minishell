/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fn_back_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: melisha <melisha@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/03/02 08:48:50 by melisha           #+#    #+#             */
/*   Updated: 2021/03/03 13:48:18 by melisha          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libminishell.h"

int		fn_back_redirect(t_obj *obj)
{
	int		i;
	char	*arg;

	obj->flag.beg += 1;
	obj->flag.back_redirect = 1;
	i = obj->flag.beg;
	obj->redirect.count_red = 0;
	while (obj->pars.line[i] == '<' && obj->pars.line[i] && obj->pars.line[i] != ';')
	{
		obj->redirect.count_red += 1;
		i++;
		obj->flag.beg++;
	}
	if (obj->redirect.count_red > 2)
	{
		obj->flag.valid_back_red = 0;
		return (-1);
	}
	else if (obj->pars.line[i] == '\0')
	{
		obj->flag.valid_redir = -1;
		return (-1);
	}
	i = fn_space(obj->pars.line, obj->flag.beg);
	obj->flag.beg = fn_space(obj->pars.line, obj->flag.beg);
	if (obj->pars.line[obj->flag.beg ] == '<')
	{
		obj->redirect.count_red = 2;
		while(obj->pars.line[obj->flag.beg] == '<')
		{
			obj->flag.beg++;
			obj->redirect.count_red++;
		}
		obj->flag.valid_back_red = 0;
		return (-1);
	}
	else if (obj->pars.line[obj->flag.beg] == '>')
	{
		obj->redirect.count_red = 2;
		while(obj->pars.line[obj->flag.beg] == '>')
		{
			obj->flag.beg++;
			obj->redirect.count_red+=1;
		}
		obj->flag.valid_redir = 0;
		return (-1);
	}
	while (obj->pars.line[i] != ' ' && obj->pars.line[i] != '\0' && obj->pars.line[i] != '<' && obj->pars.line[i] != ';')
	{
		if (obj->pars.line[i] == '\"' || obj->pars.line[i] == '\'')
		{
			obj->flag.quote = obj->pars.line[i];
			i++;
			while (obj->pars.line[i] != obj->flag.quote && obj->pars.line[i])
				i++;
		}
		if (obj->pars.line[i] != '\0')
			i++;
		else
		{
			obj->flag.multiple_com = 0;
			return (-1);
		}
	}
	if (!(obj->pars.redirect = ft_substr(obj->pars.line, obj->flag.beg, i - obj->flag.beg)))
		fn_error("not memory allocate");
	obj->pars.redirect = fn_circumcision(obj->pars.redirect, obj);
	printf("back red = %s*\n", obj->pars.redirect);
	obj->redirect.fd_back_redirect = open(obj->pars.redirect, O_RDONLY, 0666);
	if (obj->redirect.fd_back_redirect == -1)
	{
		obj->flag.valid_back_red = -1;
		return (-1);
	}
	if (obj->redirect.count_red == 1)
	{
		write(1, ">", 1);
		while (get_next_line(&arg, 0) > 0)
		{
			write(1, ">", 1);
			if (!obj->pars.arg_for_back_redirect)
				obj->pars.arg_for_back_redirect = ft_strdup(arg);
			else
				obj->pars.arg_for_back_redirect = ft_strjoin(obj->pars.arg_for_back_redirect, arg);
			obj->pars.arg_for_back_redirect = ft_strjoin(obj->pars.arg_for_back_redirect, "\n");
		}
	}
	else if (obj->redirect.count_red == 0)
	{
		while ((get_next_line(&arg, obj->redirect.fd_back_redirect)) > 0)
		{
			if (!obj->pars.arg_for_back_redirect)
				obj->pars.arg_for_back_redirect = ft_strdup(arg);
			else
				obj->pars.arg_for_back_redirect = ft_strjoin(obj->pars.arg_for_back_redirect, arg);
			obj->pars.arg_for_back_redirect = ft_strjoin(obj->pars.arg_for_back_redirect, "\n");
		}
	}
	obj->flag.beg = fn_space(obj->pars.line, i);
	return (obj->flag.beg);
}