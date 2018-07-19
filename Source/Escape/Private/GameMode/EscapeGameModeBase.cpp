// Copyright 2018 by January. All Rights Reserved.

#include "EscapeGameModeBase.h"
#include "EscapeGameSession.h"


/** Returns game session class to use */
TSubclassOf<AGameSession> AEscapeGameModeBase::GetGameSessionClass() const
{
	return AEscapeGameSession::StaticClass();
}

