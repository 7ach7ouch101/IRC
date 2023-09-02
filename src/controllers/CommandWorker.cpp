/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandWorker.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-kham <sel-kham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/30 22:34:30 by sel-kham          #+#    #+#             */
/*   Updated: 2023/09/01 03:18:35 by sel-kham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../modules/CommandWorker.hpp"

CommandWorker::CommandWorker(Server *server)
{
	this->server = server;
}

/* Static Class Elements */
std::map<const str_t, CommandWorker::functionallity> CommandWorker::allCommands = std::map<const str_t, CommandWorker::functionallity>();

void	CommandWorker::storeCommands(void)
{
	CommandWorker::allCommands.insert(std::pair<const str_t, CommandWorker::functionallity>("PASS", &CommandWorker::pass));
	CommandWorker::allCommands.insert(std::pair<const str_t, CommandWorker::functionallity>("KICK", &CommandWorker::pass));
	CommandWorker::allCommands.insert(std::pair<const str_t, CommandWorker::functionallity>("NICK", &CommandWorker::nick));
	
}


str_t	(CommandWorker::*CommandWorker::routing(void)) (Client &client) // TODO: Update the function arguments later so it matches the type
{
	str_t	(CommandWorker::*fulfill) (Client &client);  // TODO: Update the function arguments later so it matches the type
	std::map<const str_t, functionallity>::iterator	it;

	it = CommandWorker::allCommands.find(this->command); // TODO: need access to this element
	fulfill = NULL;
	if (it != CommandWorker::allCommands.end())
		fulfill = it->second;
	return (fulfill);
}

str_t	CommandWorker::execute(Client &client)
{
	str_t	(CommandWorker::*fulfill) (Client &client);

	fulfill = this->routing();
	if (!fulfill)
		return (""); // TODO: update the return type to match the error massges to client
	return ((this->*fulfill)(client));
}

CommandWorker::CommandHelper::CommandHelper(void) { }

CommandWorker::CommandHelper::~CommandHelper(void) { }

client_m::iterator	CommandWorker::CommandHelper::findClientByNickName(CommandWorker *worker, const str_t &nick)
{
	client_m::iterator	it;

	for (it = worker->server->clients.begin(); it != worker->server->clients.end(); it++)
	{
		if (it->second.getNickname() == nick)
			break ;
	}
	return (it);
}
