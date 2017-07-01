/*
 * sindbuf.h
 *
 *  Created on: 2015. 5. 10.
 *      Author: panda
 */

#ifndef SINDBUFF_H_
#define SINDBUFF_H_

#include "secind.h"
#include "recfile/fixfld.h"
using namespace std;

// class TextIndexBuffer supports reading and writing index records from
//	class TextIndex
// each record is consistent in its maximum size
class SecondaryIndexBuffer: public FixedFieldBuffer
{public:
	SecondaryIndexBuffer(int keySize, int maxKeys = 100, 
		int extraFields = 0, int extraSize=0);
	// extraSize is included to allow derived classes to extend
	// the buffer with extra fields. 
	// Required because the buffer size is exact.
	int Pack (const SecondaryIndex &);
	int Unpack (SecondaryIndex &);
	void Print (ostream &) const;
protected:
	int MaxKeys;
	int KeySize;
	char * Dummy; // space for dummy in pack and unpack
};

#endif /* SINDBUFF_H_ */
