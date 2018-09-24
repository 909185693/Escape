#pragma once

enum ELogicCode : unsigned short int
{
#ifndef DEFINE_LOGIC_CODE
#define DEFINE_LOGIC_CODE(Name, Value, Desc) \
Name = Value,
#endif //DEFINE_LOGIC_CODE
#include "LogicCode.h"
#undef DEFINE_LOGIC_CODE
};

enum EErrorCode : unsigned short int
{
#ifndef DEFINE_ERROR_CODE
#define DEFINE_ERROR_CODE(Name, Value, Desc) \
Name = Value,
#endif //DEFINE_ERROR_CODE
#include "ErrorCode.h"
#undef DEFINE_ERROR_CODE
};


struct FDataHeader
{
	ELogicCode	Code;
	EErrorCode	Error;
	int			Size;
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

	void Init(ELogicCode InCode, EErrorCode InError, int InSize)
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
	int		ID;
	char	Nickname[20];
};

struct FClientTravel
{
	char	URL[256];
};

struct FInvitation
{
	int	 UserID;
};

enum EMatchState
{
	EnteringMap,
	WaitingToStart,
	InProgress,
	WaitingPostMatch,
	LeavingMap,
	Aborted
};

struct FDedicatedServer
{
	int						Port;
	char					IP[15];
	char					Guid[128];
	unsigned short int		MinPlayers;
	unsigned short int		MaxPlayers;
};