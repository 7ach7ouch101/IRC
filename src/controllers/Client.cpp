/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-kham <sel-kham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/18 02:41:57 by sel-kham          #+#    #+#             */
/*   Updated: 2023/08/25 22:39:15 by sel-kham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../modules/Client.hpp"

const str_t	Client::forbiddenChars = " ,*?!@.#&";
const str_t	Client::forbiddenToStartWith = "#&$:";
 
/* Constructors & Distructors */
Client::Client(void) { }

Client::Client(const int &fd)
{
	this->clientFd = fd;
	this->nickname = "";
	this->username = "";
	this->level = CLIENT;
	this->vAuth = 0;
}

Client::~Client(void) { }

/* Getters & Setters */
const int	&Client::getClientFd(void) const
{
	return (this->clientFd);
}

const str_t			&Client::getNickname(void) const
{
return (this->nickname);
}

const str_t			&Client::getUsername(void) const
{
return (this->username);
}

const bool	&Client::getIsMod(void) const
{
	return (this->isMod);
}

const unsigned char &Client::getVAuth(void) const
{
	return (this->vAuth);
}

void	Client::setClientFd(const int &fd)
{
	this->clientFd = fd;
}

void	Client::setNickname(const str_t &nickname)
{
	int	i;

	i = -1;
	if (!nickname[0] || Client::forbiddenToStartWith.find(nickname[0]) < Client::forbiddenToStartWith.length())
		throw (std::runtime_error("Invalid nickname."));
	while (nickname[++i])
		if (Client::forbiddenChars.find(nickname[i]) < Client::forbiddenChars.length())
			throw (std::runtime_error("Invalid nickname."));
	this->nickname = nickname;
}

void	Client::setUsername(const str_t &username)
{
	this->username = username;
}

void	Client::setIsMod(const bool &isMod)
{
	this->isMod = isMod;
}
