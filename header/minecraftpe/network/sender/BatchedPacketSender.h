#pragma once

class Packet;                      
class NetworkIdentifier;

class BatchedPacketSender {
public:
	virtual void ~BatchedPacketSender();
	virtual void send(Packet const&);
	virtual void send(NetworkIdentifier const&, Packet const&);
	virtual void sendBroadcast(NetworkIdentifier const&, Packet const&);
	virtual void sendBroadcast(Packet const&);
	virtual void flush(NetworkIdentifier const&);
};