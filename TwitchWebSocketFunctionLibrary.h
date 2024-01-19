// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "TwitchWebSocketFunctionLibrary.generated.h"

/**
 *
 */
class UTwitchWebSocket;
UCLASS()
class MYPROJECT_API UTwitchWebSocketFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "Twitch WebSockets")
		static UTwitchWebSocket* CreateTwitchWebSocket(FString ServerUrl = TEXT("wss://irc-ws.chat.twitch.tv:443"), FString ServerProtocol = TEXT("wss"));

	UFUNCTION(BlueprintCallable, Category = "Twitch WebSockets")
		static UTwitchWebSocket* CreateTwitchWebSocketWithHeaders(FString ServerUrl, TMap<FString, FString> UpgradeHeaders, FString ServerProtocol = TEXT("wss"));

	UFUNCTION(BlueprintCallable, Category = "Twitch WebSockets")
		static void GetMessageType(FString Message, FString& MessageType);

	UFUNCTION(BlueprintCallable, Category = "Twitch WebSockets")
		static void OnNoticeReceived(FString Message);

	UFUNCTION(BlueprintCallable, Category = "Twitch WebSockets")
		static void OnPartRecieved(FString Message);

	UFUNCTION(BlueprintCallable, Category = "Twitch WebSockets")
		static void OnPingRecieved(FString Message);

	UFUNCTION(BlueprintCallable, Category = "Twitch WebSockets")
		static void OnPrivmsgRecieved(FString Message,
			/* Desciption of tags : https://dev.twitch.tv/docs/irc/tags/#privmsg-tags */
			bool& vip,
			bool& sub,
			bool& mod,
			bool& first,
			bool& turbo,
			bool& pinnedChatPaidIsSytemMessage,
			FColor& color,
			FDateTime& tmiSentTs,
			int32& bits,
			int32& pinnedChatPaidAmount,
			int32& tier,
			FString& pinnedChatPaidCurrency,
			FString& pinnedChatPaidExponent,
			FString& pinnedChatPaidLevel,
			FString& displayName,
			FString& badgeInfo,
			FString& clientNonce,
			FString& flags,
			FString& id,
			FString& roomId,
			FString& userId,
			FString& userType,
			FString& replyParentMsgId,
			FString& replyParentUserId,
			FString& replyParentUserLogin,
			FString& replyParentDisplayName,
			FString& replyParentMsgBody,
			FString& replyThreadParentMsgId,
			FString& replyThreadParentUserLogin,
			FString& messageBody
		);

	UFUNCTION(BlueprintCallable, Category = "Twitch WebSockets")
		static void OnClearchatRecieved(FString Message);

	UFUNCTION(BlueprintCallable, Category = "Twitch WebSockets")
		static void OnClearmsgRecieved(FString Message);

	UFUNCTION(BlueprintCallable, Category = "Twitch WebSockets")
		static void OnGlobaluserstateRecieved(FString Message);

	UFUNCTION(BlueprintCallable, Category = "Twitch WebSockets")
		static void OnHosttargetRecieved(FString Message);

	UFUNCTION(BlueprintCallable, Category = "Twitch WebSockets")
		static void OnReconnectRecieved(FString Message);

	UFUNCTION(BlueprintCallable, Category = "Twitch WebSockets")
		static void OnRoomstateRecieved(FString Message);

	UFUNCTION(BlueprintCallable, Category = "Twitch WebSockets")
		static void OnUsernoticeRecieved(FString Message);

	UFUNCTION(BlueprintCallable, Category = "Twitch WebSockets")
		static void OnUserstateRecieved(FString Message);

	UFUNCTION(BlueprintCallable, Category = "Twitch WebSockets")
		static void OnWhisperRecieved(FString Message);

	UFUNCTION(BlueprintCallable, Category = "Twitch WebSockets")
		static void OnJoinRecieved(FString Message);

	UFUNCTION(BlueprintCallable, Category = "Twitch WebSockets")
		static void StringReverseAppendOnCondition(FString StringToAppend, FString String, bool Condition, FString& Out);
};