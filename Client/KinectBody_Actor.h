// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "KinectBody_Actor.generated.h"

USTRUCT(BlueprintType)
struct FKinectBodyStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
		bool IsFirstTracked;

	UPROPERTY(BlueprintReadWrite)
		int32	BodyNumber;

	UPROPERTY(BlueprintReadWrite)
		TArray<FVector>	JointsPosition;

	UPROPERTY(BlueprintReadWrite)
		TArray<FRotator>	JointsOrientation;
};

UCLASS()
class REALKINECTCLIENT_API AKinectBody_Actor : public AActor
{
	GENERATED_BODY()
public:
	TArray<ReceiveDataStruct*> RecvDataArray;

	TArray<FKinectBodyStruct> ConvertedKinectBodies;

	uint32 ReceiveDataSize;

	FCriticalSection CS;

public:	
	// Sets default values for this actor's properties
	AKinectBody_Actor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ConvertBodyData();

	UFUNCTION(BlueprintCallable, Category = "Kinect")
		TArray<FKinectBodyStruct> ConvertCTB_GetBody();
	
};

