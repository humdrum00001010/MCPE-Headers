#pramga once

#include <vector>
#include <string>

#include "Packet.h"

enum class TextPacketType {
	RAW
};

class TextPacket : public Packet {
public:
	TextPacketType type;
	std::string sender;
	std::string text;
	std::vector<std::string> params;
public:	
	TextPacket() {}
public:
	virtual ~TextPacket();
	virtual int getId() const;
	virtual const char *getName() const;
	virtual void write(BinaryStream&) const;
	virtual void read(BinaryStream&);
	virtual void handle(NetworkIdentifier const&, NetEventCallback&) const;
	virtual void disallowBatching() const;
};