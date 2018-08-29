// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kinect/KinectBody_Actor.h"

/**
 * 
 */
class REALKINECTCLIENT_API Receiver : public FRunnable
{
public:
	static Receiver*	Runnable;

	TUniquePtr<FRunnableThread> ReceiverThread;

	FThreadSafeCounter	StopTaskCounter;

	TSharedPtr<FSocket>	ConnectSocket;

	FIPv4Endpoint RemoteAddressForConnection;

	AKinectBody_Actor* KinectBodyActor;

	ReceiveDataStruct RecvData[TotalClientCount];

public:
	Receiver(TSharedPtr<FSocket> Socket, AKinectBody_Actor* BodyActor);
	~Receiver();

	virtual bool	Init();
	virtual uint32	Run();
	virtual void	Stop();

	void	EnsureCompletion();

	static void Shutdown();

	static bool IsThreadFinished(FSocket* ConnectionSocket);
};
