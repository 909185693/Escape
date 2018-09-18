// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#define ESCAPELOGICSERVER_PACKAGE 0
#define ESCAPEDEDICATEDSERVER_PACKAGE 0
#define ESCAPECLIENT_PACKAGE 0


enum ELogicCode : uint16
{
#ifndef DEFINE_LOGIC_CODE
#define DEFINE_LOGIC_CODE(Name, Value, Desc) \
Name = Value,
#endif //DEFINE_LOGIC_CODE
#include "EscapeLogicCode.h"
#undef DEFINE_LOGIC_CODE
};

enum EErrorCode : uint16
{
#ifndef DEFINE_ERROR_CODE
#define DEFINE_ERROR_CODE(Name, Value, Desc) \
Name = Value,
#endif //DEFINE_ERROR_CODE
#include "EscapeErrorCode.h"
#undef DEFINE_ERROR_CODE
};


struct FDataHeader
{
	ELogicCode	Code;
	EErrorCode	Error;
	int32		Size;
	char		Valid[6];

	bool IsValid() const
	{
		return Valid[0] == 'E' &&
			Valid[1] == 'S' &&
			Valid[2] == 'C' &&
			Valid[3] == 'A' &&
			Valid[4] == 'P' &&
			Valid[5] == 'E';
	};

	void Init(ELogicCode InCode, EErrorCode InError, int32 InSize)
	{
		Valid[0] = 'E';
		Valid[1] = 'S';
		Valid[2] = 'C';
		Valid[3] = 'A';
		Valid[4] = 'P';
		Valid[5] = 'E';

		Code = InCode;
		Error = InError;
		Size = InSize;
	}
};

struct FUserLogin
{
	char Username[20];
	char Password[20];
};

struct FUser
{
	int32	ID;
	char	Nickname[20];
};

struct FClientTravel
{
	char	URL[256];
};

struct FInvitation
{
	int32 UserID;
};

struct FDedicatedServer
{
	FDedicatedServer()
	{
		FMemory::Memzero(this, sizeof(FDedicatedServer));
	}

	int32	Port;
	char	IP[15];
	char	Guid[128];
	int32	MinPlayers;
	int32	MaxPlayers;
};

struct FGameInfo
{
	int32 NumPlayers;
};