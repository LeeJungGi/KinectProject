// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Kinect/KinectBody_Actor.h"
#include "Network/Connector.h"
#include "Server_Actor.generated.h"

UCLASS()
class REALKINECTSERVER_API AServer_Actor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	AServer_Actor();

	TUniquePtr<Connector> TCPConnector;

	AKinectBody_Actor* KinectBodyActor;

	TSharedPtr<FSocket> ListenerSocket;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool StartTCPServer(
		const FString& YourChosenSocketName,
		const uint32 ThePort
	);

	FSocket* CreateTCPConnectionListener(
		const FString& YourChosenSocketName,
		const uint32 ThePort,
		const uint32 ReceiveBufferSize = 2 * 1024 * 1024
	);
	
};
