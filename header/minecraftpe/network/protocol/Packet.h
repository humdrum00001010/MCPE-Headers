#pramga once

class BinaryStream;
class NetworkIdentifier;
class NetEventCallback&;

// Size : 12
class Packet {
public:
	// void **vtable 0
	int filler[2]; // 4
public:
	virtual ~Packet();
	virtual int getId() const = 0;
	virtual const char *getName() const = 0;
	virtual void write(BinaryStream&) const = 0;
	virtual void read(BinaryStream&) = 0;
	virtual void handle(NetworkIdentifier const&, NetEventCallback&) const = 0;
	virtual void disallowBatching() const;
};