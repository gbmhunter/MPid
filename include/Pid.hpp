//!
//! @file 			Pid.h
//! @author 		Geoffrey Hunter <gbmhunter@gmail.com> (www.cladlab.com)
//! @edited 		n/a
//! @created		2012/10/01
//! @last-modified 	2014/03/24
//! @brief 			Header file for Pid.c
//! @details
//!					See README.rst in repo root dir for more info.

//===============================================================================================//
//====================================== HEADER GUARD ===========================================//
//===============================================================================================//

#ifndef __cplusplus
	#error Please build with C++ compiler
#endif

#ifndef PID_H
#define PID_H

//===============================================================================================//
//================================== PRECOMPILER CHECKS =========================================//
//===============================================================================================//

//! @brief		Set to 1 if you want debug information printed
#define pidPRINT_DEBUG			0

//! @brief		Size (in bytes) of the debug buffer (only valid if
//!				pidPRINT_DEBUG is set to 1).
#define pidDEBUG_BUFF_SIZE		200

//===============================================================================================//
//======================================== NAMESPACE ============================================//
//===============================================================================================//

#include <stdint.h>		// uint32_t
#include <stdio.h>		// snprintf

namespace CP3id
{
	
	//! @brief		PID class that uses dataTypes for it's arithmetic
	template <class dataType> class Pid
	{
		public:

			//===============================================================================================//
			//=================================== PUBLIC TYPEDEFS ===========================================//
			//===============================================================================================//
			typedef enum			//!< Enumerates the controller direction modes
			{
				PID_DIRECT,			//!< Direct drive (+error gives +output)
				PID_REVERSE			//!< Reverse driver (+error gives -output)
			} ctrlDir_t;

			//! Used to determine wether the output shouldn't be accumulated (distance control),
			//! or accumulated (velocity control).
			typedef enum
			{
				DONT_ACCUMULATE_OUTPUT,
				ACCUMULATE_OUTPUT,
				DISTANCE_PID = DONT_ACCUMULATE_OUTPUT,
				VELOCITY_PID = ACCUMULATE_OUTPUT
			} outputMode_t;
		
			//! @brief 		Init function
			//! @details   	The parameters specified here are those for for which we can't set up 
			//!    			reliable defaults, so we need to have the user set them.
			void Init(
				dataType kp, 
				dataType ki,
				dataType kd, 
				ctrlDir_t controllerDir,
				outputMode_t outputMode,
				double samplePeriodMs, 
				dataType minOutput,
				dataType maxOutput, 
				dataType setPoint);
			
			//! @brief 		Computes new PID values
			//! @details 	Call once per sampleTimeMs. Output is stored in the pidData structure.
			void Run(dataType input);
			
			void SetOutputLimits(dataType min, dataType max);
		
			//! @details	The PID will either be connected to a direct acting process (+error leads to +output, aka inputs are positive) 
			//!				or a reverse acting process (+error leads to -output, aka inputs are negative)
			void SetControllerDirection(ctrlDir_t controllerDir);
			
			//! @brief		Changes the sample time
			void SetSamplePeriod(uint32_t newSamplePeriodMs);
			
			//! @brief		This function allows the controller's dynamic performance to be adjusted. 
			//! @details	It's called automatically from the init function, but tunings can also
			//! 			be adjusted on the fly during normal operation
			void SetTunings(dataType kp, dataType ki, dataType kd);

			//! @brief		Returns the actual (not time-scaled) proportional constant.
			dataType GetKp();

			//! @brief		Returns the actual (not time-scaled) integral constant.
			dataType GetKi();

			//! @brief		Returns the actual (not time-scaled) derivative constant.
			dataType GetKd();

			//! @brief		Returns the time-scaled (dependent on sample period) proportional constant.
			dataType GetZp();

			//! @brief		Returns the time-scaled (dependent on sample period) integral constant.
			dataType GetZi();

			//! @brief		Returns the time-scaled (dependent on sample period) derivative constant.
			dataType GetZd();

			//! @brief		Prints debug information to the desired output
			void PrintDebug(const char* msg);

			//! @brief 		The set-point the PID control is trying to make the output converge to.
			dataType setPoint;			

			//! @brief		The control output. 
			//! @details	This is updated when Pid_Run() is called.
			dataType output;				
		
		private:
			//#if(pidPRINT_DEBUG == 1)
				char debugBuff[pidDEBUG_BUFF_SIZE];
			//#endif

			//! Time-step scaled proportional constant for quick calculation (equal to actualKp)
			dataType Zp;		
			
			//! Time-step scaled integral constant for quick calculation
			dataType Zi;	
			
			//! Time-step scaled derivative constant for quick calculation
			dataType Zd;					
			
			//! Actual (non-scaled) proportional constant
			dataType Kp;			
			
			//! Actual (non-scaled) integral constant
			dataType Ki;			
			
			//! Actual (non-scaled) derivative constant
			dataType Kd;			
			
			//! Actual (non-scaled) proportional constant
			dataType prevInput;			
			
			//! The change in input between the current and previous value
			dataType inputChange;			
	
			//! @brief		The error between the set-point and actual output (set point - output, positive
			//! 			when actual output is lagging set-point.
			dataType error;
			
			//! @brief 		The output value calculated the previous time Pid_Run() was called.
			//! @details	Used in ACCUMULATE_OUTPUT mode.
			dataType prevOutput;		

			//! @brief		The sample period (in milliseconds) between successive Pid_Run() calls.
			//! @details	The constants with the z prefix are scaled according to this value.
			double samplePeriodMs;	
											
			dataType pTerm;				//!< The proportional term that is summed as part of the output (calculated in Pid_Run())
			dataType iTerm;				//!< The integral term that is summed as part of the output (calculated in Pid_Run())
			dataType dTerm;				//!< The derivative term that is summed as part of the output (calculated in Pid_Run())
			dataType outMin;				//!< The minimum output value. Anything lower will be limited to this floor.
			dataType outMax;				//!< The maximum output value. Anything higher will be limited to this ceiling.	

			//! @brief		Counts the number of times that Run() has be called. Used to stop
			//!				derivative control from influencing the output on the first call.
			//! @details	Safely stops counting once it reaches 2^32-1 (rather than overflowing). 
			uint32_t numTimesRan;

			//! @brief		The controller direction (FORWARD or REVERSE).
			ctrlDir_t controllerDir;

			//! @brief		The output mode (non-accumulating vs. accumulating) for the control loop.
			outputMode_t outputMode;
	};

	//===============================================================================================//
	//============================ TEMPLATE FUNCTION DEFINITIONS ====================================//
	//===============================================================================================//

	template <class dataType> void Pid<dataType>::Init(
		dataType kp, 
		dataType ki,
		dataType kd, 
		ctrlDir_t controllerDir, 
		outputMode_t outputMode, 
		double samplePeriodMs, 
		dataType minOutput, 
		dataType maxOutput, 
		dataType setPoint)
	{

		this->SetOutputLimits(minOutput, maxOutput);

		this->samplePeriodMs = samplePeriodMs;

		this->SetControllerDirection(controllerDir);
		this->outputMode = outputMode;
		
		// Set tunings with provided constants
		this->SetTunings(kp, ki, kd);
		this->setPoint = setPoint;
		this->prevInput = 0;
		this->prevOutput = 0;

		this->pTerm = 0.0;
		this->iTerm = 0.0;
		this->dTerm = 0.0;
			
	}

	template <class dataType> void Pid<dataType>::Run(dataType input)
	{
		// Compute all the working error variables
		//dataType input = *_input;
		
		this->error = this->setPoint - input;
		
		// PROPORTIONAL CALCS

		this->pTerm = this->Zp*this->error;

		// INTEGRAL CALCS
		
		this->iTerm += (this->Zi * this->error);
		// Perform min/max bound checking on integral term
		if(this->iTerm > this->outMax)
			this->iTerm = this->outMax;
		else if(this->iTerm < this->outMin)
			this->iTerm = this->outMin;

		// DERIVATIVE CALS

		// Only calculate derivative if run once or more already.
		if(this->numTimesRan > 0)
		{
			this->inputChange = (input - this->prevInput);
			this->dTerm = -this->Zd*this->inputChange;
		}

		// Compute PID Output. Value depends on outputMode
		if(this->outputMode == DONT_ACCUMULATE_OUTPUT)
		{
			this->output =  this->pTerm + this->iTerm + this->dTerm;
		}
		else if(this->outputMode == ACCUMULATE_OUTPUT)
		{
			this->output = this->prevOutput + this->pTerm + this->iTerm + this->dTerm;
		}
		
		// Limit output
		if(this->output > this->outMax)
			this->output = this->outMax;
		else if(this->output < this->outMin)
			this->output = this->outMin;
		
		// Remember input value to next call
		this->prevInput = input;
		// Remember last output for next call
		this->prevOutput = this->output;
		  
		// Increment the Run() counter, after checking to make sure it hasn't reached
		// max value.
		if(this->numTimesRan < 0xFFFFFFFF)
			this->numTimesRan++;
	}

	//! @brief		Sets the PID tunings.
	//! @warning	Make sure samplePeriodMs is set before calling this funciton.
	template <class dataType> void Pid<dataType>::SetTunings(dataType kp, dataType ki, dataType kd)
	{
		if (kp<0 || ki<0 || kd<0)
	   		return;
	 
	   	this->Kp = kp;
		this->Ki = ki;
		this->Kd = kd;
	   
	   // Calculate time-step-scaled PID terms
	   this->Zp = kp;

		// The next bit requires double->dataType casting functionality.
	   this->Zi = ki * (dataType)(this->samplePeriodMs/1000.0);
	   this->Zd = kd / (dataType)(this->samplePeriodMs/1000.0);
	 
	  if(this->controllerDir == PID_REVERSE)
	   {
	      this->Zp = (0 - this->Zp);
	      this->Zi = (0 - this->Zi);
	      this->Zd = (0 - this->Zd);
	   }

		#if(pidPRINT_DEBUG == 1)
			snprintf(debugBuff, 
				sizeof(debugBuff),
				"PID: Tuning parameters set. Kp = %.1f, Ki = %.1f, Kd = %f.1, "
				"Zp = %.1f, Zi = %.1f, Zd = %.1f, with sample period = %.1fms\r\n",
				Kp,
				Ki,
				Kd,
				Zp,
				Zi,
				Zd,
				samplePeriodMs);
			PrintDebug(debugBuff);
		#endif
	}

	template <class dataType> dataType Pid<dataType>::GetKp()
	{
		return this->Kp;
	}

	template <class dataType> dataType Pid<dataType>::GetKi()
	{
		return this->Ki;
	}

	template <class dataType> dataType Pid<dataType>::GetKd()
	{
		return this->Kd;
	}

	template <class dataType> dataType Pid<dataType>::GetZp()
	{
		return this->Zp;
	}

	template <class dataType> dataType Pid<dataType>::GetZi()
	{
		return this->Zi;
	}

	template <class dataType> dataType Pid<dataType>::GetZd()
	{
		return this->Zd;
	}
	
	template <class dataType> void Pid<dataType>::SetSamplePeriod(uint32_t newSamplePeriodMs)
	{
	   if (newSamplePeriodMs > 0)
	   {
	      dataType ratio  = (dataType)newSamplePeriodMs
	                      / (double)this->samplePeriodMs;
	      this->Zi *= ratio;
	      this->Zd /= ratio;
	      this->samplePeriodMs = newSamplePeriodMs;
	   }
	}

	template <class dataType> void Pid<dataType>::SetOutputLimits(dataType min, dataType max)
	{
		if(min >= max) 
	   		return;
	   	this->outMin = min;
	   	this->outMax = max;
	 
	}

	template <class dataType> void Pid<dataType>::SetControllerDirection(ctrlDir_t controllerDir)
	{
		if(controllerDir != this->controllerDir)
		{
	   		// Invert control constants
			this->Zp = (0 - Zp);
	    	this->Zi = (0 - Zi);
	    	this->Zd = (0 - Zd);
		}   
	   this->controllerDir = controllerDir;
	}

	template <class dataType> void Pid<dataType>::PrintDebug(const char* msg)
	{
		// Support for multiple platforms
		#if(__linux)
			printf("%s", (const char*)msg);
		#endif
	}

} // namespace CP3id

#endif // #ifndef PID_H

// EOF
