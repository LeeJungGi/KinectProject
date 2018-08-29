// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "KinectBody_Actor.generated.h"

USTRUCT(BlueprintType)
struct KinectBodyStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
		bool	IsTracked;

	UPROPERTY(BlueprintReadWrite)
		int32	BodyNumber;

	UPROPERTY(BlueprintReadWrite)
		TArray<FVector>	JointsPosition;

	UPROPERTY(BlueprintReadWrite)
		TArray<FRotator>	JointsOrientation;

};

UCLASS()
class REALKINECTSERVER_API AKinectBody_Actor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AKinectBody_Actor();

	UPROPERTY(BlueprintReadWrite, Category = "BPToCPP")
		TArray<KinectBodyStruct>	TotalKinectBodies;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
