//!
//! @file 			BasicTesta.cpp
//! @author 		Geoffrey Hunter <gbmhunter@gmail.com> (www.mbedded.ninja)
//! @edited 		n/a
//! @created		2013-05-27
//! @last-modified 	2014-10-10
//! @brief 			Unit test file for MPid library.
//! @details
//!					See README.rst in repo root dir for more info.

//===== SYSTEM LIBRARIES =====//
//#include <iostream>	//! @debug

//====== USER LIBRARIES =====//
#include "MUnitTest/api/MUnitTestApi.hpp"

//===== USER SOURCE =====//
#include "../api/MPidApi.hpp"

using namespace MbeddedNinja::MPidNs;

namespace MPidTests
{

	MTEST(ZpZiZdCorrectTest)
	{

		double kp = 1.0;
		double ki = 5.0;
		double kd = 6.0;
		double timeStepMs = 1000.0;

		Pid<double> pidTest(
			kp,									//!< Kp
			ki,									//!< Ki
			kd,									//!< Kd
			Pid<double>::ControllerDirection::PID_DIRECT,		//!< Control type
			Pid<double>::OutputMode::VELOCITY_PID,		//!< Control type
			timeStepMs,									//!< Update rate (ms)
			0.0,										//!< Min output
			100.0,										//!< Max output
			0.0											//!< Initial set-point
		);

		// Now check that Kp, Ki, and Kd were calculated correctly

		CHECK_CLOSE(pidTest.GetKp(), kp, 0.0001);
		CHECK_CLOSE(pidTest.GetKi(), ki, 0.0001);
		CHECK_CLOSE(pidTest.GetKd(), kd, 0.0001);

		// Check Zp, Zi, and Zd (time-scaled Ki, Kd, Kp) were calculated correctly
		CHECK_CLOSE(pidTest.GetZp(), kp, 0.0001);
		CHECK_CLOSE(pidTest.GetZi(), ki/(timeStepMs/1000.0), 0.0001);
		CHECK_CLOSE(pidTest.GetZd(), kd*(timeStepMs/1000.0), 0.0001);
	}

	MTEST(ZeroInZeroOutTest)
	{
		Pid<double> pidTest(
			1.0,									//!< Kp
			0.0,									//!< Ki
			0.0,									//!< Kd
			Pid<double>::ControllerDirection::PID_DIRECT,		//!< Control type
			Pid<double>::OutputMode::VELOCITY_PID,		//!< Control type
			10.0,									//!< Update rate (ms)
			0.0,									//!< Min output
			100.0,								//!< Max output
			0.0									//!< Initial set-point
		);

		pidTest.Run(0.0);

		CHECK_CLOSE(pidTest.output, 0, 0.0001);
	}

	MTEST(POnlyPositiveTest)
	{
		Pid<double> pidTest(
			1.0,									//!< Kp
			0.0,									//!< Ki
			0.0,									//!< Kd
			Pid<double>::ControllerDirection::PID_DIRECT,		//!< Control type
			Pid<double>::OutputMode::DONT_ACCUMULATE_OUTPUT,		//!< Control type
			1000.0,								//!< Update rate (ms)
			-100.0,									//!< Min output
			100.0,								//!< Max output
			0.0									//!< Initial set-point
		);

		// Perform one execution
		pidTest.Run(1.0);

		// Output should be -1
		CHECK_CLOSE(pidTest.output, -1.0, 0.0001);
	}

	MTEST(POnlyNegativeTest)
	{
		// Set-up P only controller, non accumulating
		Pid<double> pidTest(
			1.0,									//!< Kp
			0.0,									//!< Ki
			0.0,									//!< Kd
			Pid<double>::ControllerDirection::PID_DIRECT,		//!< Control type
			Pid<double>::OutputMode::DONT_ACCUMULATE_OUTPUT,		//!< Control type
			1000.0,								//!< Update rate (ms)
			-100.0,									//!< Min output
			100.0,								//!< Max output
			0.0									//!< Initial set-point
		);

		// Perform one execution
		pidTest.Run(-1.0);

		// Output should be 1
		CHECK_CLOSE(pidTest.output, 1.0, 0.0001);
	}

	MTEST(IOnlyPositiveTest)
	{
		Pid<double> pidTest(
			0.0,									//!< Kp
			10.0,									//!< Ki
			0.0,									//!< Kd
			Pid<double>::ControllerDirection::PID_DIRECT,		//!< Control type
			Pid<double>::OutputMode::DONT_ACCUMULATE_OUTPUT,		//!< Control type
			1000.0,								//!< Update rate (ms)
			-100.0,									//!< Min output
			100.0,								//!< Max output
			0.0									//!< Initial set-point
		);

		// Perform one execution
		pidTest.Run(1.0);

		// Output should be -10
		CHECK_CLOSE(pidTest.output, -10.0, 0.0001);

		// Perform one execution
		pidTest.Run(1.0);

		// Output should be -20
		CHECK_CLOSE(pidTest.output, -20.0, 0.0001);
	}

	MTEST(IOnlyNegativeTest)
	{
		Pid<double> pidTest(
			0.0,									//!< Kp
			10.0,									//!< Ki
			0.0,									//!< Kd
			Pid<double>::ControllerDirection::PID_DIRECT,		//!< Control type
			Pid<double>::OutputMode::DONT_ACCUMULATE_OUTPUT,		//!< Control type
			1000.0,								//!< Update rate (ms)
			-100.0,									//!< Min output
			100.0,								//!< Max output
			0.0									//!< Initial set-point
		);

		// Perform one execution
		pidTest.Run(-1.0);

		// Output should be 10
		CHECK_CLOSE(pidTest.output, 10.0, 0.0001);

		// Perform one execution
		pidTest.Run(-1.0);

		// Output should be 20
		CHECK_CLOSE(pidTest.output, 20.0, 0.0001);
	}

	MTEST(DOnlyPositiveTest)
	{
		Pid<double> pidTest(
			0.0,									//!< Kp
			0.0,									//!< Ki
			1.0,									//!< Kd
			Pid<double>::ControllerDirection::PID_DIRECT,		//!< Control type
			Pid<double>::OutputMode::DONT_ACCUMULATE_OUTPUT,		//!< Control type
			1000.0,								//!< Update rate (ms)
			-100.0,									//!< Min output
			100.0,								//!< Max output
			0.0									//!< Initial set-point
		);

		// Perform one execution
		pidTest.Run(1.0);

		std::cout << "pidTest.output = '" << pidTest.output << "'." << std::endl;

		// Output should be still 0 (at least two calls to Run()
		// required before derivative control starts working
		CHECK_CLOSE(pidTest.output, 0.0, 0.0001);

		// Perform one execution
		pidTest.Run(1.0);

		// Output should be 0.0, since no change between the two
		// last calls to Run().
		CHECK_CLOSE(pidTest.output, 0.0, 0.0001);
	
		// Perform one execution
		pidTest.Run(0.0);
	
		// Output should be 1.0
		CHECK_CLOSE(pidTest.output, 1.0, 0.0001);
	}
	
	MTEST(DOnlyNegativeTest)
	{
		Pid<double> pidTest(
			0.0,									//!< Kp
			0.0,									//!< Ki
			1.0,									//!< Kd
			Pid<double>::ControllerDirection::PID_DIRECT,		//!< Control type
			Pid<double>::OutputMode::DONT_ACCUMULATE_OUTPUT,		//!< Control type
			1000.0,								//!< Update rate (ms)
			-100.0,									//!< Min output
			100.0,								//!< Max output
			0.0									//!< Initial set-point
		);

		// Perform one execution
		pidTest.Run(-1.0);

		//std::cout << "pidTest.output = '" << pidTest.output << "'." << std::endl;
	
		// Output should be still 0 (at least two calls to Run()
		// required before derivative control starts working
		CHECK_CLOSE(pidTest.output, 0.0, 0.0001);
	
		// Perform one execution
		pidTest.Run(-1.0);
	
		// Output should be 0.0, since no change between the two
		// last calls to Run().
		CHECK_CLOSE(pidTest.output, 0.0, 0.0001);
	
		// Perform one execution
		pidTest.Run(0.0);
	
		// Output should be -1.0
		CHECK_CLOSE(pidTest.output, -1.0, 0.0001);
	}

} // namespace MPidTests

// EOF
