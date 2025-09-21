/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafedory <mafedory@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 13:46:55 by mafedory          #+#    #+#             */
/*   Updated: 2025/09/21 14:19:16 by mafedory         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rush_02.h"

int	ft_pstr_err(char *str)
{
	ft_putstr(str);
	return (-1);
}

int	valid_input(char *str) // zero valid, 1 invalid
{
	while(is_whitespace(*str))
		str++;
	while(ft_is_digit(*str))
		str++;
	while(is_whitespace(*str))
		str++;
	if(*str == '\0')
		return(0);
	else
		return(1);
}

//validate the number, nie na minusie i nie pusty, i tylko digits