#pragma once

struct INT3
{
	int32 X{ 0 };
	int32 Y{ 0 };
	int32 Z{ 0 };
};

#pragma pack(push, 1)
struct ReceiveDataStruct
{
	bool IsFirstTracked{ false };

	int32 BodyNumber{ NotTrackedBodyNumber };

	INT3 JointsPosition[NeedsBodyPartsPosition];

	INT3 JointsOrientation[NeedsBodyPartsOrientation];
};
#pragma pack(pop)
