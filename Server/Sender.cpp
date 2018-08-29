// Fill out your copyright notice in the Description page of Project Settings.

#include "RealKinectServer.h"
#include "Sender.h"

Sender*Sender::Runnable = nullptr;

Sender::Sender(TSharedPtr<FSocket> Socket, TArray<KinectBodyStruct>* Data, int32 ThreadNum)
{
	ConnectionSocket = Socket;

	KinectBodyData = Data;

	FString ThreadName(FString::Printf(TEXT("TCPNetworking %i"), rand()));

	SendDataThread = TUniquePtr<FRunnableThread>(
		FRunnableThread::Create(this, *ThreadName, 0, TPri_BelowNormal)
		);

	ClientNum = ThreadNum;

	SendDataSize = sizeof(SendDataStruct) * TotalClientCount;

}

Sender::~Sender()
{
	if (ConnectionSocket.IsValid()) {
		ConnectionSocket.Reset();
	}
}

bool Sender::Init()
{
	return true;
}

void Sender::MakeSendDataArray()
{
	const uint32 PlayerNum{ 0 };
	const uint32 OtherNum{ 1 };

	switch (ClientNum)
	{
	case PlayerNum:
		ConvertBodyData[PlayerNum].IsFirstTracked = true;
		ConvertBodyData[OtherNum].IsFirstTracked = false;
		break;
	case OtherNum:
		ConvertBodyData[PlayerNum].IsFirstTracked = false;
		ConvertBodyData[OtherNum].IsFirstTracked = true;
		break;
	default:
		break;
	}

	if (ConvertBodyData[PlayerNum].BodyNumber == NotTrackedBodyNumber) {

		for (auto i = 0; i < TotalTrackedBodyCount; i++) {

			if ((*KinectBodyData)[i].IsTracked) {
				ConvertBodyData[PlayerNum].BodyNumber = (*KinectBodyData)[i].BodyNumber;

				for (auto j = 0; j < NeedsBodyPartsPosition; j++) {
					ConvertBodyData[PlayerNum].JointsPosition[j] = (*KinectBodyData)[i].JointsPosition[j];
				}

				for (auto j = 0; j < NeedsBodyPartsOrientation; j++) {
					ConvertBodyData[PlayerNum].JointsOrientation[j] = (*KinectBodyData)[i].JointsOrientation[j];
				}
			}
		}
	}
	else {
		int32 n = ConvertBodyData[PlayerNum].BodyNumber;

		if ((*KinectBodyData)[n].IsTracked) {

			ConvertBodyData[PlayerNum].BodyNumber = (*KinectBodyData)[n].BodyNumber;

			for (auto j = 0; j < NeedsBodyPartsPosition; j++) {
				ConvertBodyData[PlayerNum].JointsPosition[j] = (*KinectBodyData)[n].JointsPosition[j];
			}

			for (auto j = 0; j < NeedsBodyPartsOrientation; j++) {
				ConvertBodyData[PlayerNum].JointsOrientation[j] = (*KinectBodyData)[n].JointsOrientation[j];
			}
		}
		else {
			ConvertBodyData[PlayerNum].BodyNumber = NotTrackedBodyNumber;
		}
	}

	if (ConvertBodyData[OtherNum].BodyNumber == NotTrackedBodyNumber) {

		for (auto i = 0; i < TotalTrackedBodyCount; i++) {

			if ((*KinectBodyData)[i].IsTracked && ConvertBodyData[PlayerNum].BodyNumber != i) {

				ConvertBodyData[OtherNum].BodyNumber = (*KinectBodyData)[i].BodyNumber;

				for (auto j = 0; j < NeedsBodyPartsPosition; j++) {
					ConvertBodyData[OtherNum].JointsPosition[j] = (*KinectBodyData)[i].JointsPosition[j];
				}

				for (auto j = 0; j < NeedsBodyPartsOrientation; j++) {
					ConvertBodyData[OtherNum].JointsOrientation[j] = (*KinectBodyData)[i].JointsOrientation[j];
				}
			}
		}
	}
	else {
		int32 n = ConvertBodyData[OtherNum].BodyNumber;

		if ((*KinectBodyData)[n].IsTracked) {

			ConvertBodyData[OtherNum].BodyNumber = (*KinectBodyData)[n].BodyNumber;

			for (auto j = 0; j < NeedsBodyPartsPosition; j++) {
				ConvertBodyData[OtherNum].JointsPosition[j] = (*KinectBodyData)[n].JointsPosition[j];
			}

			for (auto j = 0; j < NeedsBodyPartsOrientation; j++) {
				ConvertBodyData[OtherNum].JointsOrientation[j] = (*KinectBodyData)[n].JointsOrientation[j];
			}
		}
		else {
			ConvertBodyData[OtherNum].BodyNumber = NotTrackedBodyNumber;
		}
	}

	SendBodyDataArray.Push(ConvertBodyData);
}

uint32 Sender::Run()
{
	FPlatformProcess::Sleep(0.03);

	while (StopTaskCounter.GetValue() == 0 && !IsThreadFinished(ConnectionSocket.Get()))
	{
		MakeSendDataArray();
		
		int32 BytesSent{ 0 };

		while (SendBodyDataArray.Num() > 0) 
		{
			SendDataStruct* data = SendBodyDataArray.Pop();

			TArray<BYTE> SendData;
			SendData.SetNumZeroed(SendDataSize);

			memcpy(SendData.GetData(), data, SendData.Num());

			bool successful = true;
			successful = ConnectionSocket->Send(SendData.GetData(), SendData.Num(), BytesSent);

			if (successful) {

			}
			else {
				return 0;
			}
		}

		FPlatformProcess::Sleep(0.01);
	}

	//종료

	return 0;
}

void Sender::Stop()
{
	StopTaskCounter.Increment();
}

void Sender::EnsureCompletion()
{
	Stop();
	SendDataThread->WaitForCompletion();
}

void Sender::Shutdown()
{
	if (Runnable)
	{
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = nullptr;
	}
}

bool Sender::IsThreadFinished(FSocket* ConnectionSocket)
{
	//실행 종료 조건
	return false;
}
