// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

enum ELogicCode
{

};

enum EErrorCode
{
	INVALID_DATA,
	NETWORK_ERROR
};

struct FDataHeader
{
	ELogicCode	Code;
	EErrorCode	Error;
	int32		Size;

	bool IsValid() const
	{
		return Valid[0] == 'E' &&
			Valid[1] == 'S' &&
			Valid[2] == 'C' &&
			Valid[3] == 'A' &&
			Valid[4] == 'P' &&
			Valid[5] == 'E';
	};

private:
	char	Valid[6] = {'E', 'S', 'C', 'A', 'P', 'E'};
};