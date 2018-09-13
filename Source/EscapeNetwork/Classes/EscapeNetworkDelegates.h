// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

class FEscapeNetworkNotify
{
public:
	/**
	 * Notification that an incoming connection is pending, giving the interface a chance to reject the request
	 *
	 * @return EAcceptConnection indicating willingness to accept the connection at this time
	 */
	//virtual EAcceptConnection::Type NotifyAcceptingConnection() PURE_VIRTUAL(FEscapeNetworkNotify::NotifyAcceptedConnection, return EAcceptConnection::Ignore;);

	/**
	 * Notification that a new connection has been created/established as a result of a remote request, previously approved by NotifyAcceptingConnection
	 *
	 * @param Connection newly created connection
	 */
	//virtual void NotifyAcceptedConnection(class UNetConnection* Connection) PURE_VIRTUAL(FEscapeNetworkNotify::NotifyAcceptedConnection, );

	/**
	 * Notification that a new channel is being created/opened as a result of a remote request (Actor creation, etc)
	 *
	 * @param Channel newly created channel
	 *
	 * @return true if the channel should be opened, false if it should be rejected (destroying the channel)
	 */
	//virtual bool NotifyAcceptingChannel(class UChannel* Channel) PURE_VIRTUAL(FEscapeNetworkNotify::NotifyAcceptingChannel, return false;);

	/**
	 * Handler for messages sent through a remote connection's control channel
	 * not required to handle the message, but if it reads any data from Bunch, it MUST read the ENTIRE data stream for that message (i.e. use FNetControlMessage<TYPE>::Receive())
	 */
	//virtual void NotifyControlMessage(UNetConnection* Connection, uint8 MessageType, class FInBunch& Bunch) PURE_VIRTUAL(FEscapeNetworkNotify::NotifyReceivedText, );
};