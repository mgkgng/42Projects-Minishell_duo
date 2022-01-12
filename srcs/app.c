/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gphilipp <gphilipp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/11 10:50:45 by gkgpteam          #+#    #+#             */
/*   Updated: 2022/01/12 20:50:41 by gphilipp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <stdlib.h>

t_app	*handle_app(t_app *abc)
{
	static t_app	*app = NULL;

	if (abc)
		app = abc;
	return (app);
}

t_app	*get_app(void)
{
	return (handle_app(NULL));
}

void	init_app(t_app *app)
{
	app->workdir = ft_pwd();
	(void)app;
}
