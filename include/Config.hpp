//!
//! @file 			Config.hpp
//! @author 		Geoffrey Hunter <gbmhunter@gmail.com> (www.cladlab.com)
//! @edited 		n/a
//! @created		2014/03/24
//! @last-modified 	2014/03/24
//! @brief 			Configuration file for the PID library.
//! @details
//!					See README.rst in repo root dir for more info.

//===============================================================================================//
//====================================== HEADER GUARD ===========================================//
//===============================================================================================//

#ifndef __cplusplus
	#error Please build with C++ compiler
#endif

#ifndef CP3ID_CONFIG_H
#define CP3ID_CONFIG_H

//===============================================================================================//
//================================== PRECOMPILER CHECKS =========================================//
//===============================================================================================//

//! @brief		Set to 1 if you want debug code enabled. Set to 0 if you want to save memory.
#define cp3id_config_INCLUDE_DEBUG_CODE			1

#if(cp3id_config_INCLUDE_DEBUG_CODE == 1)
	//! @brief		Size (in bytes) of the debug buffer (only valid if
	//!				cp3id_config_INCLUDE_DEBUG_CODE is set to 1).
	#define cp3id_config_DEBUG_BUFF_SIZE		200
#endif

#endif // #ifndef CP3ID_CONFIG_H

// EOF
