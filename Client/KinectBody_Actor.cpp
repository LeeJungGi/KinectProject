// Fill out your copyright notice in the Description page of Project Settings.

#include "RealKinectClient.h"
#include "KinectBody_Actor.h"


// Sets default values
AKinectBody_Actor::AKinectBody_Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConvertedKinectBodies.SetNumZeroed(TotalClientCount);

	for (auto i = 0; i < TotalClientCount; i++) {
		ConvertedKinectBodies[i].JointsPosition.SetNumZeroed(NeedsBodyPartsPosition);
		ConvertedKinectBodies[i].JointsOrientation.SetNumZeroed(NeedsBodyPartsOrientation);
	}

	ReceiveDataSize = sizeof(ReceiveDataStruct) * TotalClientCount;
}

// Called when the game starts or when spawned
void AKinectBody_Actor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AKinectBody_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CS.Lock();
	ConvertBodyData();
	CS.Unlock();
}

TArray<FKinectBodyStruct> AKinectBody_Actor::ConvertCTB_GetBody()
{
	return ConvertedKinectBodies;
}

void AKinectBody_Actor::ConvertBodyData()
{
	if (RecvDataArray.Num() > 0) {

		ReceiveDataStruct* data = RecvDataArray.Pop();

		if (data != nullptr) {

			for (auto i = 0; i < TotalClientCount; i++) {

				ConvertedKinectBodies[i].BodyNumber = data[i].BodyNumber;
				ConvertedKinectBodies[i].IsFirstTracked = data[i].IsFirstTracked;

				for (auto j = 0; j < NeedsBodyPartsPosition; j++) {
					ConvertedKinectBodies[i].JointsPosition[j].X = data[i].JointsPosition[j].X;
					ConvertedKinectBodies[i].JointsPosition[j].Y = data[i].JointsPosition[j].Y;
					ConvertedKinectBodies[i].JointsPosition[j].Z = data[i].JointsPosition[j].Z;
				}

				for (auto j = 0; j < NeedsBodyPartsOrientation; j++) {
					ConvertedKinectBodies[i].JointsOrientation[j].Pitch = data[i].JointsOrientation[j].X;
					ConvertedKinectBodies[i].JointsOrientation[j].Yaw = data[i].JointsOrientation[j].Y;
					ConvertedKinectBodies[i].JointsOrientation[j].Roll = data[i].JointsOrientation[j].Z;
				}
			}
		}
	}
}

