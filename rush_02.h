/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rush_02.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafedory <mafedory@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 13:38:17 by mafedory          #+#    #+#             */
/*   Updated: 2025/09/21 14:53:26 by mafedory         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RUSH_02_H
# define RUSH_02_H

typedef enum	e_state
{
	START,
	READ_DIGIT,
	WAIT_4COLON,
	WAIT_4STR,
	READ_STR
} t_state;

typedef struct	s_dict
{
	char	nb[1000];
	char	str_nb[1000];
} t_dict;

typedef struct	s_reading_state
{
	t_state	input_type;
	t_dict	*dictionary;
	int		dict_size_so_far;
	int		str_len_so_far;
	int		error;
} t_reading_state;

typedef struct	s_sized_dict
{
	t_dict	*dictionary;
	int		size;
} t_sized_dict;

void			translate(t_sized_dict dict, char *nb, int is_first);
void			ft_str_cpy(char *src, char *dest, int len);
void			translate_thousands(t_sized_dict dict, int num_zeros);
void			translate_helper(t_sized_dict dict, char *nb, int is_first);
void			translate_3digit(t_sized_dict dict, char *nb, int is_first);
void			translate_2digit(t_sized_dict dict, char *nb, int is_first);
char			*find_input(t_sized_dict dict, char *nb);
int				ft_str_equal(char *s2, char *s1);
t_sized_dict	ft_read_dict(int fd, int max_dict_size);
t_reading_state	process_read_str(t_reading_state s, char c);
t_reading_state	process_wait_4str(t_reading_state s, char c);
t_reading_state	process_wait_4colon(t_reading_state s, char c);
t_reading_state	process_read_digit(t_reading_state s, char c);
t_reading_state	process_start(t_reading_state s, char c);
t_sized_dict	free_ret_empty(t_dict *d);
t_sized_dict	new_t_sized_dict(t_dict *dictionary, int size);
void			ft_putstr(char *str);
void			ft_putstr_prefixed(char* str, int is_first);
void			trim_spaces(char *str);
int				is_whitespace_only(char *str);
int				ft_is_digit(char c);
int				is_whitespace(char c);
int				ft_strlen(char *str);
int				ft_pstr_err(char *str);
int				valid_input(char *str);
int				is_ws_n(char c);

#endif