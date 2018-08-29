// Fill out your copyright notice in the Description page of Project Settings.

#include "RealKinectServer.h"
#include "Server_Actor.h"


// Sets default values
AServer_Actor::AServer_Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AServer_Actor::BeginPlay()
{
	Super::BeginPlay();

	TActorIterator<AKinectBody_Actor> BodyIter(GetWorld());

	KinectBodyActor = (*BodyIter);

	if (!StartTCPServer("TCPServer", PortNumber)) {
		UE_LOG(LogClass, Warning, TEXT("TCPServer Failed"));
	}
}

// Called every frame
void AServer_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AServer_Actor::StartTCPServer(const FString & YourChosenSocketName, const uint32 ThePort)
{
	ListenerSocket = TSharedPtr<FSocket>(
		CreateTCPConnectionListener(YourChosenSocketName, ThePort)
		);

	if (!ListenerSocket.IsValid()) {
		return false;
	}
	else {
		TCPConnector = MakeUnique<Connector>(ListenerSocket, &(KinectBodyActor->TotalKinectBodies));
		return true;
	}

}

FSocket* AServer_Actor::CreateTCPConnectionListener(const FString & YourChosenSocketName, const uint32 ThePort, const uint32 ReceiveBufferSize)
{
	FIPv4Endpoint Endpoint(FIPv4Address::Any, ThePort);
	FSocket* ListenSocket = FTcpSocketBuilder(*YourChosenSocketName)
		.AsReusable()
		.BoundToEndpoint(Endpoint)
		.Listening(TotalClientCount);

	int32 NewSize{ 0 };
	ListenSocket->SetReceiveBufferSize(ReceiveBufferSize, NewSize);
	ListenSocket->SetSendBufferSize(ReceiveBufferSize, NewSize);

	return ListenSocket;
}

