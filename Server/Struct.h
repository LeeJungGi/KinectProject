#pragma once

struct INT3
{
	int32 X{ 0 };
	int32 Y{ 0 };
	int32 Z{ 0 };

	void operator=(const FVector& data)
	{
		X = data.X;
		Y = data.Y;
		Z = data.Z;
	}

	void operator=(const FRotator& data)
	{
		X = data.Pitch;
		Y = data.Yaw;
		Z = data.Roll;
	}
};

#pragma pack(push, 1)
struct SendDataStruct
{
	bool IsFirstTracked{ false };

	int32 BodyNumber{ NotTrackedBodyNumber };

	INT3 JointsPosition[NeedsBodyPartsPosition];

	INT3 JointsOrientation[NeedsBodyPartsOrientation];
};
#pragma pack(pop)