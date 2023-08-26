/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-kham <sel-kham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 01:16:22 by sel-kham          #+#    #+#             */
/*   Updated: 2023/08/26 02:59:46 by sel-kham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../modules/Command.hpp"

/* Constructors & Distructors */

Command::Command(str_t &messgae) : message(messgae) { }

Command::Command(const char *messgae)
{
	this->message = str_t(messgae);
}

Command::~Command() { }

/* Getters & Setters */
const str_t	&Command::getMessage(void) const
{
	return (this->message);
}

const str_t	&Command::getCommand(void) const
{
	return (this->command);
}

const str_v	&Command::getParameters(void) const
{
	return (this->parameters);
}

/* Class funcionallities */
std::map<const str_t, Command::execmd> Command::allCommands = std::map<const str_t, Command::execmd>();

void	Command::storeCommands(void)
{
	Command::allCommands.insert(std::make_pair<const str_t, Command::execmd>("PASS", &Command::pass));
	Command::allCommands.insert(std::make_pair<const str_t, Command::execmd>("NICK", &Command::nick));
}

size_t	Command::extractCommand(void)
{
	size_t	i;
	str_t	cmd;

	i = -1;
	this->message = Helpers::ltrim(this->message, WHITESPACES);
	i = this->message.find_first_of(" ");
	if (i != str_t::npos)
	{
		this->command = this->message.substr(0, i);
		return (i);
	}
	this->command = this->message;
	return (str_t::npos);
}

void	(Command::*Command::commandRouting(void)) (Client &)
{
	void	(Command::*functionallity) (Client &);
	std::map<const str_t, execmd>::iterator	it;

	this->extractCommand();
	functionallity = NULL;
	it = Command::allCommands.find(this->command);
	if (it != Command::allCommands.end())
		functionallity = it->second;
	return (functionallity);
}

void	Command::executeCommand(Client &client)
{
	void	(Command::*functionallity) (Client &);

	functionallity = this->commandRouting();
	if (!functionallity)
		return ;
	(this->*functionallity)(client);
}

void	Command::pass(Client &client)
{
	unsigned char	vAuth;
	str_t		password;
	size_t	index;

	vAuth = client.getVAuth();
	if (vAuth & CORRECT_PASSWORD)
	{
		std::cerr << "Already authenticated" << std::endl;
		return ; // TODO: Handle password command if authenticated
	}
	index = this->message.find_first_of(" ");
	if (index == str_t::npos)
		return ; //TODO: Handle Invalid PASS command
	password = this->message.substr(index, this->message.size() -1);
	password = Helpers::trim(password, "\n \r");
	if (password + "\r\n" == Server::password|| password == Server::password)
	{
		vAuth |= CORRECT_PASSWORD;
		client.setVAuth(vAuth);
	}
	else
		return ; //TODO: Handle Invalid PASS command
}
void	Command::nick(Client &client)
{
	client.getClientFd();
	this->getCommand();
}
