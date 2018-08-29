// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kinect/KinectBody_Actor.h"

/**
 * 
 */
class REALKINECTSERVER_API Sender : public FRunnable
{
public:
	static Sender*	Runnable;

	TUniquePtr<FRunnableThread>	SendDataThread;

	FThreadSafeCounter	StopTaskCounter;

	TSharedPtr<FSocket> ConnectionSocket;

	TArray<KinectBodyStruct>*	KinectBodyData;

	TArray<SendDataStruct*>	SendBodyDataArray;

	int32 SendDataSize;

	int32 ClientNum;

	SendDataStruct ConvertBodyData[TotalClientCount];

public:
	Sender(TSharedPtr<FSocket> Socket, TArray<KinectBodyStruct>* Data, int32 ThreadNum);
	~Sender();

	virtual bool	Init();
	virtual uint32	Run();
	virtual void	Stop();

	void	EnsureCompletion();

	static void Shutdown();

	static bool IsThreadFinished(FSocket* ConnectionSocket);

	void MakeSendDataArray();
};
