/*
 * MIT License
 *
 * Copyright (c) 2016 Christopher Friedt
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "BaseTest.h"

#include <unistd.h>

BaseTest::BaseTest()
:
	should_interrupt( false )
{
}

BaseTest::~BaseTest()
{
}

void BaseTest::SetUp() {
	SetUpVirt();
	should_interrupt = true;
	interrupt_th = thread( interrupt, this );
}

void BaseTest::TearDown() {
	TearDownVirt();
	should_interrupt = false;
	try {
		interrupt_th.join();
	} catch( ... ) {
	}
}

void BaseTest::interrupt( BaseTest *siht ) {
	usleep( siht->interrupt_delay_ms() * 1000 );
	if ( siht->should_interrupt ) {
		siht->interrupt_cb();
	}
}
