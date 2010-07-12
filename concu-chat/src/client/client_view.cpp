/*
 * client_view.cpp
 *
 *  Created on: Jun 26, 2010
 *      Author: demian
 */

#include "client_view.h"
#include "client_state.h"
#include "utils.h"

#include <iostream>
#include <sstream>

namespace
{

template<typename T> void print(const T& t)
{
	std::cout << t << std::flush;
}

template<typename T> void println(const T& t)
{
	std::cout << t << std::endl;
}

void println()
{
	println("");
}

void printLineSeparator()
{
	println("========================================"
		"========================================");
}

} // end namespace

const string ClientView::EXIT_COMMAND = "-salir";
const string ClientView::PEER_TABLE_COMMAND = "-usuarios";
const string ClientView::START_CHAT_COMMAND = "-hola";
const string ClientView::END_CHAT_COMMAND = "-adios";
const string ClientView::LOWERCASE_YES = "si";
const string ClientView::LOWERCASE_NO = "no";

ClientView::ClientView()
{
}

void ClientView::showWelcomeMessage()
{
	printLineSeparator();
	println("Bienvenido a Concu-Chat 2010!");
	println(
			"Escribí " + EXIT_COMMAND
					+ " o un caracter e fin de archivo (ctrl + D) para salir cuando quieras.");
	printLineSeparator();
}

void ClientView::askUserName()
{
	print("Tu nombre?: ");
}

void ClientView::showCouldNotContactServer()
{
	println("No se pudo contactar al servicio de localización =(");
}

void ClientView::showCouldNotContactPeer(const string& peerName)
{
	println("No se pudo contactar a " + peerName + " =(");
}

void ClientView::showInvalidName(const string& userName)
{
	println("El nombre \"" + userName + "\" no es válido");
}

void ClientView::showAlreadyUsedName(const string& userName)
{
	println("El nombre \"" + userName
			+ "\" ya está siendo usado. Intentá con otro nombre =P");
}

void ClientView::showPeerTable(const PeerTable& peerTable)
{
	print(peerTable);
}

void ClientView::showIdleStateCommands()
{
	println("Comandos:");
	println(PEER_TABLE_COMMAND + " para mostrar los usuarios conectados");
	println(START_CHAT_COMMAND + " <nombre-usuario> para chatear con alguien");
}

void ClientView::showCannotChatWithYourself()
{
	println("No podés chatear con vos mismo! (lo siento...)");
}

void ClientView::showInvalidPeerName(const string& peerName)
{
	println("El usuario " + peerName
			+ " no existe. Intentá actualizar la tabla de usuarios");
}

void ClientView::showInvalidCommand(const string& command)
{
	println("Comando invlálido: " + command);
}

void ClientView::showWaitingPeerResponse(const string& peerName)
{
	println("Esperando la respuesta de " + peerName);
}

void ClientView::askUserStartChatWith(const string& peerName)
{
	println(peerName + " quiere chatear con vos");
	print("Querés? (" + LOWERCASE_YES + "/" + LOWERCASE_NO + "): ");
}

void ClientView::showPeerCanceledChat(const string& peerName)
{
	println(peerName + " está ocupado. Lo siento mucho =(");
}

void ClientView::showPeerAcceptedChat(const string& peerName)
{
	println(peerName + " aceptó!");
}

void ClientView::showStartChatSession(const string& peerName)
{
	printLineSeparator();
	println("Sesión de chat con " + peerName + " iniciada");
	println("Escribí " + END_CHAT_COMMAND + " para terminar la sesión");
	printLineSeparator();
}

void ClientView::showEndChatSession(const string& peerName)
{
	println("Sesión de chat con " + peerName + " terminada");
	printLineSeparator();
}

void ClientView::showPeerLeftChat(const string& peerName)
{
	println(peerName + " se ha ido =(");
}

void ClientView::showChatMessage(const string& peerName,
		const string& chatMessage)
{
	println(peerName + " dice: " + chatMessage);
}

bool ClientView::isYesString(const string& str)
{
	return toLowerCase(str) == LOWERCASE_YES;
}

bool ClientView::isNoString(const string& str)
{
	return toLowerCase(str) == LOWERCASE_NO;
}

