// Fill out your copyright notice in the Description page of Project Settings.


#include "TwitchWebSocketFunctionLibrary.h"
#include "IWebSocket.h"
#include "TwitchWebsocket.h"
#include "WebSocketsModule.h"
#include "Containers/UnrealString.h"

UTwitchWebSocket* UTwitchWebSocketFunctionLibrary::CreateTwitchWebSocket(FString ServerUrl, FString ServerProtocol)
{
	return CreateTwitchWebSocketWithHeaders(ServerUrl, {}, ServerProtocol);
}

UTwitchWebSocket* UTwitchWebSocketFunctionLibrary::CreateTwitchWebSocketWithHeaders(FString ServerUrl, TMap<FString, FString> UpgradeHeaders, FString ServerProtocol /* = TEXT("ws") */)
{
	const TSharedPtr<IWebSocket> ActualSocket = FModuleManager::LoadModuleChecked<FWebSocketsModule>(TEXT("WebSockets")).CreateWebSocket(ServerUrl, ServerProtocol, UpgradeHeaders);
	UTwitchWebSocket* const WrapperSocket = NewObject<UTwitchWebSocket>();
	WrapperSocket->InitWebSocket(ActualSocket);
	return WrapperSocket;
}

void UTwitchWebSocketFunctionLibrary::GetMessageType(FString Message, FString& MessageType) {
	TArray< FString > OutArray;

	if (Message.Len() > 0) {
		Message.ParseIntoArray(OutArray, TEXT(" "));
		if (OutArray.Num() > 3) {
			if (OutArray[2] == "CLEARCHAT") {
				MessageType = "CLEARCHAT";
			}
			else if (OutArray[2] == "CLEARMSG") {
				MessageType = "CLEARMSG";
			}
			else if (OutArray[1] == "CAP" && OutArray[3] == "ACK") {
				MessageType = "CAPACK";
			}
			else if (OutArray[1] == "CAP" && OutArray[3] == "NAK") {
				MessageType = "CAPNAK";
			}
			else if (OutArray[2] == "PRIVMSG") {
				MessageType = "PRIVMSG";
			}
			else if ((OutArray[3] == ":Welcome,") && (OutArray[0] == ":tmi.twitch.tv")) {
				MessageType = "GLOBALUSERSTATE";
			}
			else if (OutArray[1] == "NOTICE" && OutArray[3] == ":Login") {
				MessageType = "FAILEDAUTH";
			}
			else if (OutArray[2] == "NOTICE") {
				MessageType = "NOTICE";
			}
			else if (OutArray[2] == "ROOMSTATE") {
				MessageType = "ROOMSTATE";
			}
			else if (OutArray[2] == "USERNOTICE") {
				MessageType = "USERNOTICEMSG";
			}
			else if (OutArray[2] == "USERSTATE") {
				MessageType = "USERSTATE";
			}
			else if (OutArray[2] == "WHISPER") {
				MessageType = "WHISPER";
			} 
			else if (OutArray[1] == "JOIN") {
				MessageType = "JOIN";
			}
			else if (OutArray[1] == "353") {
				MessageType = "353";
			}
			else if (OutArray[1] == "366") {
				MessageType = "366";
			}
			else {
				MessageType = "UNDEFINED L3";
			}
		} else if (OutArray.Num() > 2){
			if (OutArray[2] == "USERNOTICE") {
				MessageType = "USERNOTICE";
			}
			else if (OutArray[1] == "JOIN") {
				MessageType = "JOIN";
			}
			else {
				MessageType = "UNDEFINED L2";
			}
		} else if (OutArray.Num() > 1) {
			if (OutArray[1] == "HOSTTARGET") {
				MessageType = "HOSTTARGET";
			} else if (OutArray[1] == "RECONNECT") {
				MessageType = "RECONNECT";
			}
			else if (OutArray[0] == "PING") {
				MessageType = "PING";
			}
			else {
				MessageType = "UNDEFINED L1";
			}
		}
		else {
			MessageType = "UNDEFINED LNAUTH";
		}
	}
	else {
		MessageType = "UNDEFINED L0";
	}
	
}

void UTwitchWebSocketFunctionLibrary::OnNoticeReceived(FString Message) {

}

void UTwitchWebSocketFunctionLibrary::OnPartRecieved(FString Message) {

}

void UTwitchWebSocketFunctionLibrary::OnPingRecieved(FString Message) {

}

void UTwitchWebSocketFunctionLibrary::OnPrivmsgRecieved(FString Message,
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
	/*emotes*/
	/*badges*/
) {

	TArray< FString > OutArray;
	TArray< FString > TagsArray;
	TArray< FString > KeyArray;
	mod = false;
	sub = false;
	vip = false;
	turbo = false;
	first = false;
	pinnedChatPaidIsSytemMessage = false;

	Message.ParseIntoArray(OutArray, TEXT(" "));
	OutArray[0].ParseIntoArray(TagsArray, TEXT(";"));
	for (FString TagString : TagsArray) {
		KeyArray = {};
		TagString.ParseIntoArray(KeyArray, TEXT("="));
		if (KeyArray.Num() < 2) {
			KeyArray.Add(TEXT(""));
		}
		if (KeyArray[0] == "@badge-info") {
			badgeInfo = KeyArray[1];
		}
		else if (KeyArray[0] == "badges") {

		}
		else if (KeyArray[0] == "bits") {
			bits = FCString::Atoi(*KeyArray[1]);
		}
		else if (KeyArray[0] == "client-nonce") {
			clientNonce = KeyArray[1];
		}
		else if (KeyArray[0] == "color") {
			if (KeyArray[1].Equals(TEXT(""))) {
				KeyArray[1] = "#FFFFFF";
			}
			color = FColor::FromHex(KeyArray[1]);
		}
		else if (KeyArray[0] == "display-name") {
			displayName = KeyArray[1];
		}
		else if (KeyArray[0] == "emotes") {

		}
		else if (TagString.Contains(TEXT("first-msg=1"))) {
			first = true;
		}
		else if (KeyArray[0] == "flags") {
			flags = KeyArray[1];
		}
		else if (KeyArray[0] == "id") {
			id = KeyArray[1];
		}
		else if (TagString.Contains(TEXT("mod=1"))) {
			mod = true;
		}
		else if (KeyArray[0] == "room-id") {
			roomId = KeyArray[1];
		}
		else if (TagString.Contains(TEXT("subscriber=1"))) {
			sub = true;
		}
		else if (TagString.Contains(TEXT("turbo=1"))) {
			turbo = true;
		}
		else if (KeyArray[0] == "user-id") {
			userId = KeyArray[1];
		}
		else if (KeyArray[0] == "user-type") {
			userType = KeyArray[1];
		}
		else if (KeyArray[0] == "reply-parent-msg-id") {
			replyParentMsgId = KeyArray[1];
		}
		else if (KeyArray[0] == "reply-parent-user-id") {
			replyParentUserId = KeyArray[1];
		}
		else if (KeyArray[0] == "reply-parent-user-login") {
			replyParentUserLogin = KeyArray[1];
		}
		else if (KeyArray[0] == "reply-parent-display-name") {
			replyParentDisplayName = KeyArray[1];
		}
		else if (KeyArray[0] == "reply-parent-msg-body") {
			replyParentMsgBody = KeyArray[1];
		}
		else if (KeyArray[0] == "reply-thread-parent-msg-id") {
			replyThreadParentMsgId = KeyArray[1];
		}
		else if (KeyArray[0] == "reply-thread-parent-user-login") {
			replyThreadParentUserLogin = KeyArray[1];
		}
		else if (TagString.Contains(TEXT("vip=1"))) {
			vip = true;
		}
		else if (KeyArray[0] == "pinned-chat-paid-amount") {
			pinnedChatPaidAmount = FCString::Atoi(*KeyArray[1]);
		}
		else if (KeyArray[0] == "pinned-chat-paid-currency") {
			pinnedChatPaidCurrency = KeyArray[1];
		}
		else if (KeyArray[0] == "pinned-chat-paid-exponent") {
			pinnedChatPaidExponent = KeyArray[1];
		}
		else if (KeyArray[0] == "pinned-chat-paid-level") {
			pinnedChatPaidLevel = KeyArray[1];
		}
		else if (TagString.Contains(TEXT("pinned-chat-paid-is-system-message=1"))) {
			pinnedChatPaidIsSytemMessage = true;
		}
		else if (KeyArray[0] == "tmi-sent-ts") {
			int64 UnixTimestampSeconds;
			if (!FCString::IsNumeric(*KeyArray[1]))
			{
				tmiSentTs = FDateTime::Now(); // Return current time as a default in case of an error
			}
			else
			{
				UnixTimestampSeconds = FCString::Atoi64(*KeyArray[1]);
				tmiSentTs = FDateTime::FromUnixTimestamp(UnixTimestampSeconds);
			}
		}
		else {

		}
	};
	/*badge-info
	badges 
	bits
	client-nonce 
	color
	display-name
	emotes
	first-msg
	flags
	id
	mod
	room-id
	subscriber
	tmi-sent-ts
	turbo
	user-id
	user-type
	reply-parent-msg-id
	reply-parent-user-id
	reply-parent-user-login
	reply-parent-display-name
	reply-parent-msg-body
	reply-thread-parent-msg-id
	reply-thread-parent-user-login
	vip*/
	 
}

void UTwitchWebSocketFunctionLibrary::OnClearchatRecieved(FString Message) {

}

void UTwitchWebSocketFunctionLibrary::OnClearmsgRecieved(FString Message) {

}

void UTwitchWebSocketFunctionLibrary::OnGlobaluserstateRecieved(FString Message) {

}

void UTwitchWebSocketFunctionLibrary::OnHosttargetRecieved(FString Message) {

}

void UTwitchWebSocketFunctionLibrary::OnReconnectRecieved(FString Message) {

}

void UTwitchWebSocketFunctionLibrary::OnRoomstateRecieved(FString Message) {

}

void UTwitchWebSocketFunctionLibrary::OnUsernoticeRecieved(FString Message) {

}

void UTwitchWebSocketFunctionLibrary::OnUserstateRecieved(FString Message) {

}

void UTwitchWebSocketFunctionLibrary::OnWhisperRecieved(FString Message) {

}

void UTwitchWebSocketFunctionLibrary::OnJoinRecieved(FString Message) {

}

void UTwitchWebSocketFunctionLibrary::StringReverseAppendOnCondition(FString StringToAppend, FString String, bool Condition, FString& Out) {
	Out = "";
	if (Condition) {
		Out.Append(StringToAppend);
		Out.Append(String);
	}
	else {
		Out.Append(String);
	}
}





