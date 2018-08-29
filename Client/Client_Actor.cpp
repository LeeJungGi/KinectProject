// Fill out your copyright notice in the Description page of Project Settings.

#include "RealKinectClient.h"
#include "Client_Actor.h"


// Sets default values
AClient_Actor::AClient_Actor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	KinectBody = nullptr;

}

// Called when the game starts or when spawned
void AClient_Actor::BeginPlay()
{
	Super::BeginPlay();

	TActorIterator<AKinectBody_Actor> BodyIter(GetWorld());

	KinectBody = (*BodyIter);

	if (StartTCPClient(TEXT("TCPClient"), ServerIP, ServerPort)) {

		UE_LOG(LogClass, Warning, TEXT("Connect!!!"));

		Recv = MakeUnique<Receiver>(ConnectSocket, KinectBody);

	}
	else {
		UE_LOG(LogClass, Warning, TEXT("NOT Connect!!!"));
	}
	
}

void AClient_Actor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (ConnectSocket.IsValid()) {
		ConnectSocket.Reset();
	}
}

// Called every frame
void AClient_Actor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AClient_Actor::StartTCPClient(const FString& YourChosenSocketName, const FString& TheIP, const int32 ThePort)
{
	FIPv4Address ip;
	FIPv4Address::Parse(TheIP, ip);

	ConnectSocket = TSharedPtr<FSocket>(
		ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, YourChosenSocketName, false)
		);
		

	if (!ConnectSocket.Get()) {
		return false;
	}

	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value);
	addr->SetPort(ThePort);

	return ConnectSocket->Connect(*addr);
}

