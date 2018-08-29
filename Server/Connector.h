// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kinect/KinectBody_Actor.h"
#include "Network/Sender.h"

/**
 * 
 */
class REALKINECTSERVER_API Connector : public FRunnable
{
public:
	static Connector*	Runnable;

	TUniquePtr<FRunnableThread> ConnectorThread;

	TSharedPtr<FSocket>	ListenerSocket;

	uint32 ThreadCount{ 0 };

	TArray<KinectBodyStruct>*	KinectBodyData;

	TArray<TUniquePtr<Sender>>	SendThreads;

	FThreadSafeCounter	StopTaskCounter;

public:
	Connector(TSharedPtr<FSocket> Socket, TArray<KinectBodyStruct>* Data);
	~Connector();

	virtual bool	Init();
	virtual uint32	Run();
	virtual void	Stop();

	void	EnsureCompletion();

	static void Shutdown();

	static bool IsThreadFinished();
};
