// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Kinect/KinectBody_Actor.h"
#include "Network/Receiver.h"
#include "Client_Actor.generated.h"

UCLASS()
class REALKINECTCLIENT_API AClient_Actor : public AActor
{
	GENERATED_BODY()
		

public:	
	// Sets default values for this actor's properties
	AClient_Actor();

	TSharedPtr<FSocket>	ConnectSocket;

	TUniquePtr<Receiver> Recv;

	AKinectBody_Actor* KinectBody;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool StartTCPClient(
		const FString& YourChosenSocketName,
		const FString& TheIP,
		const int32 ThePort
	);
	
};
