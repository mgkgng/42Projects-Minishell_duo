/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gphilipp <gphilipp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/15 11:40:03 by gkgpteam          #+#    #+#             */
/*   Updated: 2022/02/23 15:35:55 by gphilipp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_env(t_app *app)
{
	t_list		**plist;
	t_keyval	*keyval;

	plist = &app->env;
	while (*plist)
	{	
		keyval = (t_keyval *)(*plist)->data;
		ft_putstr_fd(keyval->key, 1);
		ft_putstr_fd("=", 1);
		ft_putstr_fd(keyval->val, 1);
		ft_putstr_fd("\n", 1);
		plist = &((*plist)->next);
	}
	return (0);
}
