/*
 * Test.h
 *
 *  Created on: 03-10-2011
 *      Author: radomir.mazon
 */

#ifndef TEST_H_
#define TEST_H_
#include "avrcore.h"

class Test : public ClockTick {
public:
	void onTick();
};

#endif /* TEST_H_ */
