// Fill out your copyright notice in the Description page of Project Settings.

#include "HNetMenuPlayerState.h"
#include "HNetMenuController.h"
#include "HNetMenuGameMode.h"
#include "UnrealNetwork.h"

void AHNetMenuPlayerState::OnRequestMatching_Implementation() {
	auto Gamemode = StaticCast<AHNetMenuGameMode*>(GetWorld()->GetAuthGameMode());
	Gamemode->OnRequestMatching(StaticCast<AHNetMenuController*>(GetNetOwningPlayer()->PlayerController));
}

bool AHNetMenuPlayerState::OnRequestMatching_Validate() {
	return true;
}

void AHNetMenuPlayerState::OnMove_Implementation(const FString& Move)
{
	auto Gamemode = StaticCast<AHNetMenuGameMode*>(GetWorld()->GetAuthGameMode());
	Gamemode->OnMove(Move,IsHost,RoomID);
}

bool AHNetMenuPlayerState::OnMove_Validate(const FString& Move) {
	return true;
}

void AHNetMenuPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHNetMenuPlayerState, MeGoingFirst);
	DOREPLIFETIME(AHNetMenuPlayerState, RoomID);
	DOREPLIFETIME(AHNetMenuPlayerState, StartBoard);
	DOREPLIFETIME(AHNetMenuPlayerState, IsHost);
	DOREPLIFETIME(AHNetMenuPlayerState, UpdateFlag);
}


AHNetMenuPlayerState::AHNetMenuPlayerState() {
	bReplicates = true;
}

void AHNetMenuPlayerState::ClearUpdateFlag_Implementation(){
	UpdateFlag = false;
}

bool AHNetMenuPlayerState::ClearUpdateFlag_Validate() {
	return true;
}

void AHNetMenuPlayerState::ClearUpAll_Implementation(){
	auto Gamemode = StaticCast<AHNetMenuGameMode*>(GetWorld()->GetAuthGameMode());
	Gamemode->RemoveRoom(RoomID);
	UpdateFlag = false;
	MeGoingFirst = false;
	RoomID = -1;
	IsHost = false;
	StartBoard.Reset();
}

bool AHNetMenuPlayerState::ClearUpAll_Validate() {
	return true;
}
