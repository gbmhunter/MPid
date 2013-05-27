//!
//! @file 		Pid.cpp
//! @author 	Geoffrey Hunter <gbmhunter@gmail.com> (wwww.cladlab.com)
//! @edited 	n/a
//! @date 		2012/11/09
//! @brief 		PID library for embedded systems, written in C++.
//! @details
//!				See README.rst


//===============================================================================================//
//===================================== HEADER GUARD ============================================//
//===============================================================================================//

#ifndef __cplusplus
	#error Please build with C++ compiler
#endif

//===============================================================================================//
//========================================= INCLUDES ============================================//
//===============================================================================================//

// GCC
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


// User
#include "./include/Pid.hpp"
//#include "Global.h"
//#include "Config.h"89
//#include "./FixedPoint/include/Fp.h"		// For fixed point arithmetic (fast PID)
//#include "./Comms/include/UartDebug.h"		// Printing debug messages
//#include "./Pid/include/Pid.h"

//===============================================================================================//
//================================== PRECOMPILER CHECKS =========================================//
//===============================================================================================//



//===============================================================================================//
//======================================== NAMESPACE ============================================//
//===============================================================================================//

namespace Pid
{

	#if(pidENABLE_FP_SUPPORT == 1)
		using namespace Fp;
	#endif

	//===============================================================================================//
	//================================= PUBLIC MEMBER METHODS =======================================//
	//===============================================================================================//

	#if(pidENABLE_FP_SUPPORT == 1)
		void PidFp::Init(
			fp<CDP> kp, fp<CDP> ki, fp<CDP> kd, 
			ctrlDir_t controllerDir, outputMode_t outputMode, fp<CDP> samplePeriodMs, 
			fp<CDP> minOutput, fp<CDP> maxOutput, fp<CDP> setPoint)
		{

			SetOutputLimits(minOutput, maxOutput);		

			 samplePeriodMs = samplePeriodMs;

			 SetControllerDirection(controllerDir);
			 SetTunings(kp, ki, kd);
			setPoint = setPoint;
			prevInput = (int32_t)0;
			prevOutput = (int32_t)0;
			
		}
	#endif

	

	#if(pidENABLE_FP_SUPPORT == 1)
	void PidFp::Run(fp<CDP> input)
	{
		// Compute all the working error variables
		//double input = *_input;
		
		error = setPoint - input;
		
		// Integral calcs
		
		iTerm += (zKi * error);
		// Perform min/max bound checking on integral term
		if(iTerm > outMax) 
			iTerm = outMax;
		else if(iTerm < outMin)
			iTerm = outMin;

		inputChange = (input - prevInput);
		dTerm = -zKd*inputChange;

		// Compute PID Output. Value depends on outputMode
		if(outputMode == DONT_ACCUMULATE_OUTPUT)
		{
			output = zKp*error + iTerm + dTerm;
		}
		else if(outputMode == ACCUMULATE_OUTPUT)
		{
			output = prevOutput + zKp*error + iTerm + dTerm;
		}

		if(output > outMax) 
			output = outMax;
		else if(output < outMin)
			output = outMin;
		
		// Remember input value to next call
		prevInput = input;
		// Remember last output for next call
		prevOutput = output;
		 
	}
	#endif	// #if(pidENABLE_FP_SUPPORT == 1)

	

	#if(pidENABLE_FP_SUPPORT == 1)
	void PidFp::SetTunings(fp<CDP> kp, fp<CDP> ki, fp<CDP> kd)
	{
	   	if (kp < (int32_t)0 || ki < (int32_t)0 || kd < (int32_t)0) 
	   		return;
	 
	 	actualKp = kp; 
		actualKi = ki;
		actualKd = kd;
	   
		fp<CDP> sampleTimeInSec = (samplePeriodMs)/1000;  
	   
	 	// Calculate time-step-scaled PID terms
		zKp = kp;
		zKi = ki * sampleTimeInSec;
		zKd = kd / sampleTimeInSec;
	   
		#if(configPRINT_DEBUG_PID == 1)
			snprintf(_debugBuff, sizeof(_debugBuff), 
				"PID: zKp = %f, zKi = %f, zKd = %f\r\n", 
				Fix2Float<CDP>(zKp.intValue),
				Fix2Float<CDP>(zKi.intValue),
				Fix2Float<CDP>(zKd.intValue));
			UartDebug::PutString(_debugBuff);
		#endif
	 	
		if(controllerDir == PID_REVERSE)
	 	{
	    	zKp = (0 - zKp);
	    	zKi = (0 - zKi);
	    	zKd = (0 - zKd);
		}
		
		#if(configPRINT_DEBUG_PID == 1)
			UartDebug::PutString("PID: Tuning parameters set.\r\n");
		#endif
	}
	#endif	// #if(pidENABLE_FP_SUPPORT == 1)
	
	#if(pidENABLE_FP_SUPPORT == 1)
	void PidFp::SetOutputLimits(fp<CDP> min, fp<CDP> max)
	{
		if(min >= max) 
	   		return;
	   	outMin = min;
	   	outMax = max;
	 
	}
	#endif
	
	#if(pidENABLE_FP_SUPPORT == 1)
	void PidFp::SetControllerDirection(ctrlDir_t controllerDir)
	{
		if(controllerDir != controllerDir)
		{
	   		// Invert control constants
			zKp = (0 - zKp);
	    	zKi = (0 - zKi);
	    	zKd = (0 - zKd);
		}   
	   controllerDir = controllerDir;
	}
	#endif	// #if(pidENABLE_FP_SUPPORT == 1)

} // namespace Pid

// EOF
