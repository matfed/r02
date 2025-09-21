/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mafedory <mafedory@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/21 13:37:38 by mafedory          #+#    #+#             */
/*   Updated: 2025/09/21 14:50:02 by mafedory         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h> //testowanie
#include <unistd.h>  //write, read
#include <stdlib.h>  // nalloc, free
#include <sys/stat.h>  //open
#include <fcntl.h>   //open
#include "rush_02.h"

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (*str)
	{
		str++;
		i++;
	}
	return (i);
}

int is_whitespace(char c)
{
	if ((c >= 9 && c <= 13) || c == 32)
		return (1);
	else
		return (0);
}

int is_ws_n(char c) //is whitesace without
{
	if (c == 9 || c == 32 || c == 11 || c == 12 || c == 13)
		return (1);
	else
		return (0);
}

int ft_is_digit(char c)
{
	if(c >= '0' && c <= '9')
		return (1);
	else
		return (0);   
}

int is_whitespace_only(char *str)
{
	while(*str)
	{
		if (!is_whitespace(*str))
			return (0);
		str++;
	}
	return (1);
}

void trim_spaces(char *str)
{
	int i;

	i = 0;
	while(str[i])
		i++;
	i--;
	while(i >= 0 && is_whitespace(str[i]))
	{
		str[i] = '\0';
		i--;
	}
}

void	ft_putstr(char *str)
{
	while (*str)
	{
		write(1, str, 1);
		str++;
	}
}

t_sized_dict new_t_sized_dict(t_dict *dictionary, int size)
{
	t_sized_dict d;

	d.dictionary = dictionary;
	d.size = size;
	return(d);
}

t_sized_dict free_ret_empty(t_dict *d)
{
	free(d);
	return(new_t_sized_dict(NULL, 0));
}

t_reading_state process_start(t_reading_state s, char c)
{
	if (is_whitespace(c))
		return s;
	else if (ft_is_digit(c))  // nie jest whitespace
	{
		s.str_len_so_far = 0;
		s.input_type = READ_DIGIT;
		s.dictionary[s.dict_size_so_far].nb[s.str_len_so_far] = c;
		s.str_len_so_far++;
		return s;
	}
	else {
		s.error = 1;
		return s;
	} 
}

t_reading_state process_read_digit(t_reading_state s, char c)
{
	if (ft_is_digit(c)) {
		s.dictionary[s.dict_size_so_far].nb[s.str_len_so_far++] = c;
	}
	else
	{
		s.dictionary[s.dict_size_so_far].nb[s.str_len_so_far] = '\0';
		if (is_whitespace(c))
			s.input_type = WAIT_4COLON;
		else if (c == ':')
			s.input_type = WAIT_4STR;
		else
			s.error = 1;
	}
	return s;
}
	
t_reading_state process_wait_4colon(t_reading_state s, char c)
{
	if (is_whitespace(c))
		return s;
	else if (c == ':') 
	{
		s.input_type = WAIT_4STR;
		return s;
	}
	else 
	{
		s.error = 1;
		return s;
	}
}
	
t_reading_state process_wait_4str(t_reading_state s, char c)
{
	if (is_ws_n(c))
		return s;
	else if (!is_ws_n(c))
	{
		s.input_type = READ_STR;
		s.str_len_so_far = 0;
		if (c != '\n') {
			s.dictionary[s.dict_size_so_far].str_nb[s.str_len_so_far++] = c;
			return s;
		} 
		else 
		{
			s.error = 1;
			return s;
		}
	}
	else
	{
		s.error = 1;
		return s;
	}
}
	
t_reading_state process_read_str(t_reading_state s, char c)
{
	if (c != '\n') 
	{
		s.dictionary[s.dict_size_so_far].str_nb[s.str_len_so_far++] = c;
	}
	else 
	{
		s.dictionary[s.dict_size_so_far].str_nb[s.str_len_so_far] = '\0';
		if (is_whitespace_only(s.dictionary[s.dict_size_so_far].str_nb))
		{
			s.error = 1;
			return s;
		}
		trim_spaces(s.dictionary[s.dict_size_so_far].str_nb);
		s.dict_size_so_far++;
		s.input_type = START;
	}
	return s;
}

t_sized_dict ft_read_dict(int fd, int max_dict_size)
{
	char c;
	t_reading_state s;

	s.str_len_so_far = 0;
	s.dict_size_so_far = 0;
	s.input_type = START;
	s.dictionary = (t_dict *)malloc(sizeof(t_dict)* max_dict_size);
	s.error = 0;
	while(read(fd, &c, 1))   // jak cos jest to da 1, jak nic to da 0
	{
		if (s.input_type == START)
			s = process_start(s, c);
		else if (s.input_type == READ_DIGIT)
			s = process_read_digit(s, c);
		else if (s.input_type == WAIT_4COLON)
			s = process_wait_4colon(s, c);
		else if (s.input_type == WAIT_4STR)
			s = process_wait_4str(s, c);
		else if (s.input_type == READ_STR)
			s = process_read_str(s, c);
		if (s.error)
			return free_ret_empty(s.dictionary);
	}
	return (new_t_sized_dict(s.dictionary, s.dict_size_so_far));
}

int ft_str_equal(char *s2, char *s1)
{
	while(*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	return (*s1 == *s2);
}

char *find_input(t_sized_dict dict, char *nb)
{
	int i = 0;
	while(i < dict.size)
	{
		if(ft_str_equal(dict.dictionary[i].nb, nb))
		{
			return(dict.dictionary[i].str_nb);
		}
		i++;
	}
	return(NULL);
}

void translate_2digit(t_sized_dict dict, char *nb)
{
	char *s;
	char temp[3];

	temp[0] = nb[0];
	temp[1] = '0';
	temp[2] = '\0';
	if ((s = find_input(dict, temp)))
	{
		ft_putstr(s);
		ft_putstr(" ");
	}
	else 
		ft_putstr("Errorrrrrrrr!\n");
	if((s = find_input(dict, &nb[1])))
		ft_putstr(s);
	else 
		ft_putstr("Errorrrrrrrr!\n");
}

void translate_3digit(t_sized_dict dict, char *nb)
{
	char *s;
	char temp[2];
	
	temp[0] = nb[0];
	temp[1] ='\0';
	if ((s = find_input(dict, temp)))
	{
		ft_putstr(s);
		ft_putstr(" ");
	}
	if ((s = find_input(dict, "100")))
	{
		ft_putstr(s);
		ft_putstr(" ");
	}
	else 
		ft_putstr("Errorrrrrrrr!\n");
	translate_helper(dict, &nb[1]);
}

void translate_helper(t_sized_dict dict, char *nb)
{
	char *s;
	int len;

	len = ft_strlen(nb);
	if ((s = find_input(dict, nb)))
	{
		ft_putstr(s);
		return ;
	}
	else if(len == 2)
	{
		translate_2digit(dict, nb);
	} 
	else if(len == 3)
	{
		translate_3digit(dict, nb);
	}
}

void translate_thousands(t_sized_dict dict, int num_zeros)
{
    int i;
    char temp[1000];
    char *s;

    i = 1;
    temp[0] = '1';
    while(i <= num_zeros)
    {
        temp[i] = '0';
        i++;
    }
    temp[i] = '\0';
    if ((s = find_input(dict, temp)) && num_zeros > 0)
    {
            ft_putstr(" ");
            ft_putstr(s);
    }
}

void ft_str_cpy(char *src, char *dest, int len)
{
	int i;

	i = 0;
	while(i < len)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

void translate(t_sized_dict dict, char *nb)
{
	char *s;
	char temp[4];
	int len;
	int num_zeros;

	if((s = find_input(dict, nb)))
	{       
		ft_putstr(s);
		return;
	}
	while(*nb == '0')
		nb++;
	if(*nb == '\0')
		return ;
	len = ft_strlen(nb);
	num_zeros = ((len - 1) / 3 ) * 3;
	ft_str_cpy(nb, temp, len - num_zeros);
	translate_helper(dict, temp);
	translate_thousands(dict, num_zeros);
	ft_putstr(" ");
	translate(dict, nb + (len - num_zeros));
}

int	main(int ac, char **av) //dodac walidacje
{
	int	fd;
	t_sized_dict dict;

	fd = -1;
	if (ac == 3)
	 	fd = open(av[1], O_RDONLY);
	if(ac == 2)
		fd = open("numbers.dict", O_RDONLY);
	if(fd == -1)
		return (ft_pstr_err("Dict Error\n"));   // wydrukowac jakis error sprawdz jakie errors obslugiwac
	dict = ft_read_dict(fd, 1000);
	close(fd);
	if(dict.dictionary == NULL)
		return (ft_pstr_err("Dict Error\n"));
	if(valid_input(av[ac - 1]))
		return (ft_pstr_err("Error\n"));
	translate(dict, av[ac - 1]);
	ft_putstr("\n");
	free(dict.dictionary);
	return (0);
}