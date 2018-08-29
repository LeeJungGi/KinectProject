// Fill out your copyright notice in the Description page of Project Settings.

#include "RealKinectClient.h"
#include "Receiver.h"

Receiver*	Receiver::Runnable = nullptr;

Receiver::Receiver(TSharedPtr<FSocket> Socket, AKinectBody_Actor* BodyActor)
{
	ConnectSocket = Socket;

	KinectBodyActor = BodyActor;

	StopTaskCounter.Set(0);

	FString ThreadName(FString::Printf(TEXT("TCPReceiver %i"), rand()));

	ReceiverThread = TUniquePtr<FRunnableThread>(
		FRunnableThread::Create(this, *ThreadName, 0, TPri_BelowNormal)
		);
}

Receiver::~Receiver()
{
	if (ConnectSocket.IsValid()) {
		ConnectSocket.Reset();
	}
}

bool Receiver::Init()
{
	return true;
}

uint32 Receiver::Run()
{
	FPlatformProcess::Sleep(0.03);

	while (StopTaskCounter.GetValue() == 0 && !IsThreadFinished(ConnectSocket.Get()))
	{
		TArray<uint8> ReceivedData;

		uint32 PendingDataSize{ 0 };

		int32 ReceiveSize{ 0 };

		while (ConnectSocket->HasPendingData(PendingDataSize))
		{
			if (PendingDataSize >= static_cast<uint32>(KinectBodyActor->ReceiveDataSize)) {

				ReceivedData.SetNumZeroed(KinectBodyActor->ReceiveDataSize);

				if (ConnectSocket->Recv(ReceivedData.GetData(), ReceivedData.Num(), ReceiveSize)) {

					if (ReceiveSize == ReceivedData.Num()) {
						memcpy(RecvData, ReceivedData.GetData(), ReceiveSize);

						KinectBodyActor->CS.Lock();
						KinectBodyActor->RecvDataArray.Push(RecvData);
						KinectBodyActor->CS.Unlock();
					}
				}
				break;
			}
		}

		FPlatformProcess::Sleep(0.01);
	}

	Shutdown();

	return 0;
}

void Receiver::Stop()
{
	StopTaskCounter.Increment();
}

void Receiver::EnsureCompletion()
{
	Stop();
	ReceiverThread->WaitForCompletion();
}

void Receiver::Shutdown()
{
	if (Runnable)
	{
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = nullptr;
	}
}

bool Receiver::IsThreadFinished(FSocket* ConnectSocket)
{
	return false;
}
