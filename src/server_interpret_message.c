int	message_driver(int sender_fd, char *message)
{
	int	message_type = return_message_type(message);
	char	**commands;
	char	**message_token;
	if (message_type  == 1)
	{
		//user wants public key of other user
		commands = create_command("usercode usercode", ' ');
		message_token = return_splitted_message(message + 2, commands);
		//find target's public key & fd 
		ClinetList	*target = return_node(message_token[1]);
		//send 14[target's user code][target's public key]
		char	*built_message;
		int	built_message_length = build_message(&built_message, create_command("msg_type usercode key", ' '));
		send(target->data, built_message, built_message_length, 0);

	}
	else if (message_type  == 2)
	{
		//user wants to send public message
	}
	else if (message_type  == 3)
	{
		//user wants to send secret message
		send(fd, message, message_len, 0);

	}
	else
	{
		printf("[SERVER MESSAGE DRIVER] %d is an invalid message type from client\n", message_type);
		return 0;
	}
	free_listoflist(message_token);
	free_listoflist(commands);
	return 1;
}

