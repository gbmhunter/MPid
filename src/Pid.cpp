//!
//! @file 		Pid.cpp
//! @author 	Geoffrey Hunter <gbmhunter@gmail.com>
//! @edited 	n/a
//! @date 		09/11/2012
//! @brief 		PID library for embedded systems, written in C++.
//! @details
//!		<b>Last Modified:			</b> 07/05/13					\n
//!		<b>Version:					</b> v1.0.0						\n
//!		<b>Company:					</b> CladLabs					\n
//!		<b>Project:					</b> Free code libraries		\n
//!		<b>Language:				</b> C++						\n
//!		<b>Compiler:				</b> GCC						\n
//! 	<b>uC Model:				</b> PSoC5						\n
//!		<b>Computer Architecture:	</b> ARM						\n
//! 	<b>Operating System:		</b> n/a						\n
//!		<b>Documentation Format:	</b> Doxygen					\n
//!		<b>License:					</b> GPLv3						\n
//!
//!		See Pid.hpp for more information.


//===============================================================================================//
//===================================== HEADER GUARD ============================================//
//===============================================================================================//

#ifndef __cplusplus
	#error Please build with C++ compiler
#endif

#ifdef __cplusplus
extern "C" {
#endif

//===============================================================================================//
//========================================= INCLUDES ============================================//
//===============================================================================================//

// GCC
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


// User
#include "Global.h"
#include "Config.h"89
#include "./FixedPoint/include/Fp.h"		// For fixed point arithmetic (fast PID)
#include "./Comms/include/UartDebug.h"		// Printing debug messages
#include "./Pid/include/Pid.h"

//===============================================================================================//
//================================== PRECOMPILER CHECKS =========================================//
//===============================================================================================//

#ifndef configPRINT_DEBUG_PID
	#error Please define the switch configPRINT_DEBUG_PID
#endif

//===============================================================================================//
//======================================== NAMESPACE ============================================//
//===============================================================================================//

namespace Pid
{

	using namespace Fp;


	#if(configPRINT_DEBUG_PID == 1)
		char _debugBuff[100];
	#endif


	//===============================================================================================//
	//================================= PUBLIC MEMBER METHODS =======================================//
	//===============================================================================================//

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

	void PidDbl::Init(
		double kp, double ki, double kd, 
		ctrlDir_t controllerDir, outputMode_t outputMode, double samplePeriodMs, 
		double minOutput, double maxOutput, double setPoint)
	{

		SetOutputLimits(minOutput, maxOutput);		

	    samplePeriodMs = samplePeriodMs;

	    SetControllerDirection(controllerDir);
		outputMode = outputMode;
	    SetTunings(kp, ki, kd);
		setPoint = setPoint;
		prevInput = 0;
		prevOutput = 0;
			
	}

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


	void PidDbl::Run(double input)
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
		
		// Limit output
		if(output > outMax) 
			output = outMax;
		else if(output < outMin)
			output = outMin;
		
		// Remember input value to next call
		prevInput = input;
		// Remember last output for next call
		prevOutput = output;
		  
	}

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

	
	void PidDbl::SetTunings(double kp, double ki, double kd)
	{
	   	if (kp<0 || ki<0 || kd<0) 
	   		return;
	 
	 	actualKp = kp; 
		actualKi = ki;
		actualKd = kd;
	   
	   double sampleTimeInSec = ((double)samplePeriodMs)/1000.0;  
	   
	   // Calculate time-step-scaled PID terms
	   zKp = kp;
	   zKi = ki * sampleTimeInSec;
	   zKd = kd / sampleTimeInSec;
	 
	  if(controllerDir == PID_REVERSE)
	   {
	      zKp = (0 - zKp);
	      zKi = (0 - zKi);
	      zKd = (0 - zKd);
	   }
	}

	
	void PidDbl::SetSamplePeriod(uint32_t newSamplePeriodMs)
	{
	   if (newSamplePeriodMs > 0)
	   {
	      double ratio  = (double)newSamplePeriodMs
	                      / (double)samplePeriodMs;
	      zKi *= ratio;
	      zKd /= ratio;
	      samplePeriodMs = newSamplePeriodMs;
	   }
	}
	 
	void PidFp::SetOutputLimits(fp<CDP> min, fp<CDP> max)
	{
		if(min >= max) 
	   		return;
	   	outMin = min;
	   	outMax = max;
	 
	}
	 
	void PidDbl::SetOutputLimits(double min, double max)
	{
		if(min >= max) 
	   		return;
	   	outMin = min;
	   	outMax = max;
	 
	}

	
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

	
	void PidDbl::SetControllerDirection(ctrlDir_t controllerDir)
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
} // namespace Pid

// EOF