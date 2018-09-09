// Fill out your copyright notice in the Description page of Project Settings.

#include "RealKinectServer.h"
#include "Connector.h"

Connector*	Connector::Runnable = nullptr;

Connector::Connector(TSharedPtr<FSocket> Socket, TArray<KinectBodyStruct>* Data)
{
	KinectBodyData = Data;

	ListenerSocket = Socket;

	FString ThreadName(FString::Printf(TEXT("TCPConnect %i"), rand()));

	ConnectorThread = TUniquePtr<FRunnableThread>(
		FRunnableThread::Create(this, *ThreadName, 0, TPri_BelowNormal)
		);
}

Connector::~Connector()
{
	if (ListenerSocket.IsValid()) {
		ListenerSocket.Reset();
	}
}

bool Connector::Init()
{
	return true;
}

uint32 Connector::Run()
{
	FPlatformProcess::Sleep(0.03);

	while (StopTaskCounter.GetValue() == 0 && !IsThreadFinished()) {
		TSharedRef<FInternetAddr> RemoteAddress = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
		bool Pending;

		for (auto i = 0; i < SendThreads.Num(); i++) {
			if (SendThreads[i]->ConnectionSocket->GetConnectionState() == SCS_NotConnected) {
				SendThreads.Pop();
			}
		}

		if (ListenerSocket->HasPendingConnection(Pending) && Pending) {

			TSharedPtr<FSocket> ConnectionSocket = MakeShareable<FSocket>(
				ListenerSocket->Accept(*RemoteAddress, TEXT("TCPSocketConnection"))
				);

			if (ConnectionSocket.IsValid()) {

				UE_LOG(LogClass, Warning, TEXT("Connection!!!!!!!!!"));

				TUniquePtr<Sender> temp = MakeUnique<Sender>(
					ConnectionSocket, KinectBodyData, ThreadCount++)
					);

				SendThreads.Push(temp);
			}
		}

		FPlatformProcess::Sleep(0.01);
	}

	return 0;
}

void Connector::Stop()
{
	StopTaskCounter.Increment();
}

void Connector::EnsureCompletion()
{
	Stop();
	ConnectorThread->WaitForCompletion();
}

void Connector::Shutdown()
{
	if (Runnable) {
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = nullptr;
	}
}

bool Connector::IsThreadFinished()
{
	return false;
}


