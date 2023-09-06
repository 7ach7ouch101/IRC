/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-kham <sel-kham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/03 22:14:01 by mmeziani          #+#    #+#             */
/*   Updated: 2023/09/06 03:56:22 by sel-kham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../modules/CommandWorker.hpp"

static std::vector<Channel>	composeChannelList(const std::vector<str_t> &channels, const std::vector<str_t> &keys)
{
	std::vector<Channel>	channelList = std::vector<Channel>();

	for (size_t i = 0; i < channels.size(); ++i)
	{
		if (i >= keys.size())
			channelList.push_back(Channel(channels[i]));
		else
			channelList.push_back(Channel(channels[i], keys[i]));
	}
	return (channelList);
}

static std::vector<Channel>	joinParser(str_t params)
{
	std::vector<str_t>	channels;
	std::vector<str_t>	keys;
	size_t				i;
	std::vector<Channel>	channelsList;

	i = params.find_first_of(" ");
	if (i == str_t::npos)
		channels = Helpers::split(params, ',');
	else
	{
		str_t	tmp1;

		tmp1 = params.substr(0, i);
		channels = Helpers::split(tmp1, ',');
		tmp1 = params.substr(i + 1, params.size());
		tmp1 = Helpers::trim(tmp1, ' ');
		if (!tmp1.empty())
		{
			if (tmp1[0] == ':')
				keys.push_back(tmp1.substr(1, tmp1.size()));
			else
				keys = Helpers::split(tmp1, ',');
		}
	}
	channelsList = composeChannelList(channels, keys);
	return (channelsList);
}

str_t	CommandWorker::join(Client &client)
{
	str_t				params;
	size_t				i;
	channel_m			&allChannels = this->server->channels;
	std::vector<Channel>	channelsList;
	str_t				response;

	if (client.getVAuth() != FULL_AUTH)
		return (ERR_NOTREGISTERED(this->server->getHost(), client.getNickname()));
	i = this->request.find_first_of(" ");
	if (i == str_t::npos)
		return (ERR_NEEDMOREPARAMS(this->server->getHost(), client.getNickname()));
	params = this->request.substr(i, this->request.size() - 1);
	params = Helpers::trim(params, " \n\r");
	if (params.empty() || params.size() < 2)
		return (ERR_NEEDMOREPARAMS(this->server->getHost(), client.getNickname()));
	channelsList = joinParser(params);
	response = "";
	for (std::vector<Channel>::iterator it = channelsList.begin(); it != channelsList.end(); it++)
	{
		channel_m::iterator	ch_it;
		client_m::iterator	c_it;
		str_t				name;

		name = it->getName();
		if (name[0] != '#' && name[0] != '&')
		{
			std::cout << name << std::endl;
			response += ERR_NOSUCHCHANNEL(this->server->getHost(), client.getNickname(), name);
			continue ;
		}
		ch_it = allChannels.find(it->getName());
		if (ch_it == allChannels.end())
		{
			name = it->getName();
			it->addMod(client);
			it->addClient(client);
			allChannels.insert(std::pair<const str_t, Channel>(name, *it));
			response += RPL_JOIN(client.getNickname(), client.getUsername(), client.getHost(), name);
			response += RPL_MODE(client.getNickname(), name, "+nk");
		}
		else
		{
			str_t	ch_key = ch_it->second.getKey();
			if (ch_key.empty())
			{
				ch_it->second.addClient(client);
				response += RPL_JOIN(client.getNickname(), client.getUsername(), client.getHost(), ch_it->second.getName());
				response += RPL_MODE(client.getNickname(), ch_it->second.getName(), "+n");
				response += RPL_TOPIC(this->server->getHost(), client.getNickname(), it->getName(), ch_it->second.getTopic());
				ch_it->second.broadcast(str_t(RPL_JOIN_WATCH(client.getNickname(), client.getUsername(), client.getHost(), ch_it->second.getName())));
			}
			else
			{
				if (ch_key == it->getKey())
				{
					ch_it->second.addClient(client);
					response += RPL_JOIN(client.getNickname(), client.getUsername(), client.getHost(), ch_it->second.getName());
					response += RPL_MODE(client.getNickname(), ch_it->second.getName(), "+n");
					response += RPL_TOPIC(this->server->getHost(), client.getNickname(), it->getName(), ch_it->second.getTopic());
					ch_it->second.broadcast(str_t(RPL_JOIN_WATCH(client.getNickname(), client.getUsername(), client.getHost(), ch_it->second.getName())));
				}
				else
				{
					response += ERR_BADCHANNELKEY(this->server->getHost(), client.getNickname(), ch_it->second.getName());
				}
			}
		}
	}
    return (response);
}
