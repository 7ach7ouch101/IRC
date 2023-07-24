/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-kham <sel-kham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 04:04:05 by sel-kham          #+#    #+#             */
/*   Updated: 2023/07/18 03:35:28 by sel-kham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef __CLIENT_HPP__
# define __CLIENT_HPP__

# include "macros.hpp"

class Client
{
private:
	enum	grade {OPERATOR, USER};
	str_t	nickname;
	str_t	hostname;
	int		grade;
	/* data */
public:
	Client(/* args */);
	~Client();
	static const str_t forbiddenChars;
	static const str_t forbiddenToStartWith;
};

#endif