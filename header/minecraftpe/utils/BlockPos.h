#ifndef BLOCKPOS_H_
#define BLOCKPOS_H_

class Vec3;
class ChunkPos;

class BlockPos {
public:
	int x, y, z;
public:
	static BlockPos ZERO;
	static BlockPos MAX;
	static BlockPos MIN;
	static BlockPos ONE;
public:
	BlockPos(Vec3 const &);
	BlockPos(ChunkPos const &, int);
	BlockPos(float, float, float);
public:
	void relative(signed char, int) const;
	void neighbor(signed char) const;
	const char *toString() const;
	void center() const;
};

#endif