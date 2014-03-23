//!
//! @file 			PidTest.cpp
//! @author 		Geoffrey Hunter <gbmhunter@gmail.com> (www.cladlab.com)
//! @edited 		n/a
//! @created		2013/05/27
//! @last-modified 	2014/03/24
//! @brief 			Unit test file for Pid library.
//! @details
//!					See README.rst in repo root dir for more info.

#include "../api/CP3id.hpp"

// UnitTest++ library
#include "UnitTest++/src/UnitTest++.h"

using namespace PidNs;


TEST(ZpZiZdCorrectTest)
{
	Pid<double> pidTest;

	double kp = 1.0;
	double ki = 5.0;
	double kd = 6.0;
	double timeStepMs = 1000.0;

	// Set-up controller with P, I and D values
	pidTest.Init(
		kp,									//!< Kp
		ki,									//!< Ki
		kd,									//!< Kd
		Pid<double>::PID_DIRECT,		//!< Control type
		Pid<double>::VELOCITY_PID,		//!< Control type
		timeStepMs,							//!< Update rate (ms)
		0.0,									//!< Min output
		100.0,								//!< Max output
		0.0									//!< Initial set-point
	);
	
	// Now check that Kp, Ki, and Kd were calculated correctly
	
	CHECK_CLOSE(kp, pidTest.GetKp(), 0.0001);
	CHECK_CLOSE(ki, pidTest.GetKi(), 0.0001);
	CHECK_CLOSE(kd, pidTest.GetKd(), 0.0001);

	// Check Zp, Zi, and Zd (time-scaled Ki, Kd, Kp) were calculated correctly
	CHECK_CLOSE(kp, pidTest.GetZp(), 0.0001);
	CHECK_CLOSE(ki/(timeStepMs/1000.0), pidTest.GetZi(), 0.0001);
	CHECK_CLOSE(kd*(timeStepMs/1000.0), pidTest.GetZd(), 0.0001);
}

TEST(ZeroInZeroOutTest)
{
	Pid<double> pidTest;

	// Set-up P only controller
	pidTest.Init(
		1.0,									//!< Kp
		0.0,									//!< Ki
		0.0,									//!< Kd
		Pid<double>::PID_DIRECT,		//!< Control type
		Pid<double>::VELOCITY_PID,		//!< Control type
		10.0,									//!< Update rate (ms)
		0.0,									//!< Min output
		100.0,								//!< Max output
		0.0									//!< Initial set-point
	);
	
	pidTest.Run(0.0);

	CHECK_CLOSE(0, pidTest.output, 0.0001);
}

TEST(POnlyPositiveTest)
{
	Pid<double> pidTest;

	// Set-up P only controller, non accumulating
	pidTest.Init(
		1.0,									//!< Kp
		0.0,									//!< Ki
		0.0,									//!< Kd
		Pid<double>::PID_DIRECT,		//!< Control type
		Pid<double>::DONT_ACCUMULATE_OUTPUT,		//!< Control type
		1000.0,								//!< Update rate (ms)
		-100.0,									//!< Min output
		100.0,								//!< Max output
		0.0									//!< Initial set-point
	);
	
	// Perform one execution
	pidTest.Run(1.0);

	// Output should be -1
	CHECK_CLOSE(-1.0, pidTest.output, 0.0001);
}

TEST(POnlyNegativeTest)
{
	Pid<double> pidTest;

	// Set-up P only controller, non accumulating
	pidTest.Init(
		1.0,									//!< Kp
		0.0,									//!< Ki
		0.0,									//!< Kd
		Pid<double>::PID_DIRECT,		//!< Control type
		Pid<double>::DONT_ACCUMULATE_OUTPUT,		//!< Control type
		1000.0,								//!< Update rate (ms)
		-100.0,									//!< Min output
		100.0,								//!< Max output
		0.0									//!< Initial set-point
	);
	
	// Perform one execution
	pidTest.Run(-1.0);

	// Output should be 1
	CHECK_CLOSE(1.0, pidTest.output, 0.0001);
}

TEST(IOnlyPositiveTest)
{
	Pid<double> pidTest;

	// Set-up P only controller, non accumulating
	pidTest.Init(
		0.0,									//!< Kp
		10.0,									//!< Ki
		0.0,									//!< Kd
		Pid<double>::PID_DIRECT,		//!< Control type
		Pid<double>::DONT_ACCUMULATE_OUTPUT,		//!< Control type
		1000.0,								//!< Update rate (ms)
		-100.0,									//!< Min output
		100.0,								//!< Max output
		0.0									//!< Initial set-point
	);
	
	// Perform one execution
	pidTest.Run(1.0);

	// Output should be -10
	CHECK_CLOSE(-10.0, pidTest.output, 0.0001);

	// Perform one execution
	pidTest.Run(1.0);

	// Output should be -20
	CHECK_CLOSE(-20.0, pidTest.output, 0.0001);
}

TEST(IOnlyNegativeTest)
{
	Pid<double> pidTest;

	// Set-up P only controller, non accumulating
	pidTest.Init(
		0.0,									//!< Kp
		10.0,									//!< Ki
		0.0,									//!< Kd
		Pid<double>::PID_DIRECT,		//!< Control type
		Pid<double>::DONT_ACCUMULATE_OUTPUT,		//!< Control type
		1000.0,								//!< Update rate (ms)
		-100.0,									//!< Min output
		100.0,								//!< Max output
		0.0									//!< Initial set-point
	);
	
	// Perform one execution
	pidTest.Run(-1.0);

	// Output should be 10
	CHECK_CLOSE(10.0, pidTest.output, 0.0001);

	// Perform one execution
	pidTest.Run(-1.0);

	// Output should be 20
	CHECK_CLOSE(20.0, pidTest.output, 0.0001);
}

TEST(DOnlyPositiveTest)
{
	Pid<double> pidTest;

	// Set-up P only controller, non accumulating
	pidTest.Init(
		0.0,									//!< Kp
		0.0,									//!< Ki
		1.0,									//!< Kd
		Pid<double>::PID_DIRECT,		//!< Control type
		Pid<double>::DONT_ACCUMULATE_OUTPUT,		//!< Control type
		1000.0,								//!< Update rate (ms)
		-100.0,									//!< Min output
		100.0,								//!< Max output
		0.0									//!< Initial set-point
	);
	
	// Perform one execution
	pidTest.Run(1.0);

	// Output should be still 0 (at least two calls to Run()
	// required before derivative control starts working
	CHECK_CLOSE(0.0, pidTest.output, 0.0001);

	// Perform one execution
	pidTest.Run(1.0);

	// Output should be 0.0, since no change between the two
	// last calls to Run().
	CHECK_CLOSE(0.0, pidTest.output, 0.0001);

	// Perform one execution
	pidTest.Run(0.0);

	// Output should be 1.0
	CHECK_CLOSE(1.0, pidTest.output, 0.0001);
}

TEST(DOnlyNegativeTest)
{
	Pid<double> pidTest;

	// Set-up P only controller, non accumulating
	pidTest.Init(
		0.0,									//!< Kp
		0.0,									//!< Ki
		1.0,									//!< Kd
		Pid<double>::PID_DIRECT,		//!< Control type
		Pid<double>::DONT_ACCUMULATE_OUTPUT,		//!< Control type
		1000.0,								//!< Update rate (ms)
		-100.0,									//!< Min output
		100.0,								//!< Max output
		0.0									//!< Initial set-point
	);
	
	// Perform one execution
	pidTest.Run(-1.0);

	// Output should be still 0 (at least two calls to Run()
	// required before derivative control starts working
	CHECK_CLOSE(0.0, pidTest.output, 0.0001);

	// Perform one execution
	pidTest.Run(-1.0);

	// Output should be 0.0, since no change between the two
	// last calls to Run().
	CHECK_CLOSE(0.0, pidTest.output, 0.0001);

	// Perform one execution
	pidTest.Run(0.0);

	// Output should be -1.0
	CHECK_CLOSE(-1.0, pidTest.output, 0.0001);
}

int main()
{
	return UnitTest::RunAllTests();
}
