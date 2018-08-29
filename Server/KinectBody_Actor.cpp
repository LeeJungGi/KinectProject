// Fill out your copyright notice in the Description page of Project Settings.

#include "RealKinectServer.h"
#include "KinectBody_Actor.h"


// Sets default values
AKinectBody_Actor::AKinectBody_Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TotalKinectBodies.SetNumZeroed(TotalTrackedBodyCount);

	for (auto i = 0; i < TotalTrackedBodyCount; i++) {
		TotalKinectBodies[i].JointsPosition.SetNumZeroed(NeedsBodyPartsPosition);
		TotalKinectBodies[i].JointsOrientation.SetNumZeroed(NeedsBodyPartsOrientation);
	}

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
}

