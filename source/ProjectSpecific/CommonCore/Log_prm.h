/**
 *  @file
 *  @brief      Automatically generated public parameters for the Log module.
 *
 *  @details    This file was automatically generated on 02/07/2018 10:47:26 by
 *              LogPreprocessor.exe (v1.2.0)  Copyright © 2013-2017 Whirlpool Corporation.
 *
 *              LogPreprocessor.exe attempts to preserve user settings.
 *              The user should test to make sure that changes to this file are handled as expected.
 *
 *  $Header: Program.cs 1.13 2015/10/30 11:22:11EDT Nelson Ferragut II (FERRANJ) Exp  $
 *
 *  @copyright  Copyright 2012 - $Date: 2015/10/30 11:22:11EDT $  Whirlpool Corporation.  All rights reserved - CONFIDENTIAL.
 */
#ifndef LOG_PRM_H_
#define LOG_PRM_H_

//  --- Include Files -------------------------------------------------------------------------------------------------

// -- This Module --

// -- Other Modules --


//=====================================================================================================================
//  --- Public Parameters ---------------------------------------------------------------------------------------------
//=====================================================================================================================

// -- Compiler Directives --


//---------------------------------------------------------------------------------------------------------------------
/**
 * The LOG_MODULE_ENABLE declaration is used to enable or disable the Log module.
 *
 * Valid settings:
 *
 * ENABLED = Module works as intended. Functions are defined and compiled into the code.
 *
 * DISABLED = The module's public interface is replaced with dummy macros so that the system will
 *          compile. However, none of the functionality is present.
 *
 * The Log.h file shall declare LOG_MODULE_ENABLE as ENABLED if it is missing.
 */
#define LOG_MODULE_ENABLE                           (DISABLED)


//---------------------------------------------------------------------------------------------------------------------
/**
 * The LOG_PEEK_BUFFER_FEATURE declaration is used to enable or disable the Log__PeekBuffer()
 * function.
 *
 * ENABLED = The Log__PeekBuffer() function is defined and available to the application.
 *
 * DISABLED = The Log__PeekBuffer() function is not defined.
 *
 * The Log.h file shall declare LOG_PEEK_BUFFER_FEATURE as DISABLED if it is missing.
 */
#define LOG_PEEK_BUFFER_FEATURE                     (DISABLED)


//---------------------------------------------------------------------------------------------------------------------
/**
 * The LOG_RUN_TIME_FILTERING declaration is used to enable or disable the Log__SetMessageFilter()
 * function.
 *
 * ENABLED = The Log__SetMessageFilter() function is defined and available to the application.
 *          Initial message filters are determined at compile-time through the Log_prva.h file,
 *          but they can be modified at run-time through the Log__SetMessageFilter() function.\
 *          This feature requires an extra byte of RAM for each module that generates Log Messages.
 *
 * DISABLED = The Log__SetMessageFilter() function is not defined. Message filters are determined
 *          at compile-time through the Log_prva.h file.
 *
 * The Log.h file shall declare LOG_RUN_TIME_FILTERING as DISABLED if it is missing.
 */
#define LOG_RUN_TIME_FILTERING                      (DISABLED)


// -- Constant Declarations --

/**
 * The maximum number of Log Messages that the Message Queue can hold.
 * Set this to a value in the range [1..255].
 * The Log module will allocate 5 bytes of RAM for each message that the buffer can hold.
 */
#define LOG_MAX_MESSAGES                            (10)


//=====================================================================================================================
//  --- Public Methods ------------------------------------------------------------------------------------------------
//=====================================================================================================================


//=====================================================================================================================
//  --- Automated Parameters ------------------------------------------------------------------------------------------
//=====================================================================================================================


//---------------------------------------------------------------------------------------------------------------------
/**
 * Automated list of Module Identifiers for all the application modules that generate Log Messages.
 *
 * The LogPreprocessor.exe tool searches all project code for any source files that declare
 * Message Identifiers. The search looks for the pattern below where <NAME> is replaced with the
 * name of the module.
 * <pre>
 *      typedef enum
 *      {
 *          // Log messages enumerated here...
 *      } MODULE_<NAME>_LOG_MESSAGE_ID_TYPE;
 * </pre>
 *
 * For example, if a 'Hello' module declares the MODULE_HELLO_LOG_MESSAGE_ID_TYPE type, then the
 * LogPreprocessor.exe tool will add an enumerated value 'MODULE_HELLO' to the LOG_MODULE_ID_TYPE
 * declared below.
 */
typedef enum LOG_MODULE_ID_ENUM
{
    MODULE_API007APP              = 0,   // 0x00
    MODULE_API007BULKDATA         = 1,   // 0x01
    MODULE_API010_POLL_VAR        = 2,   // 0x02
    MODULE_API011APPCTRL          = 3,   // 0x03
    MODULE_API013_REMOTE_FUNCTION = 4,   // 0x04
    MODULE_API221_MOTION_CTRL     = 5,   // 0x05
    MODULE_ARBITRATION            = 6,   // 0x06
    MODULE_BANDLIMITER            = 7,   // 0x07
    MODULE_CALLBACK               = 8,   // 0x08
    MODULE_EXTR_PARAMS            = 9,   // 0x09
    MODULE_MODE                   = 10,  // 0x0A
    MODULE_MOTION                 = 11,  // 0x0B
    MODULE_MOTOR_ACTIVITY         = 12,  // 0x0C
    MODULE_PARAM_EST              = 13,  // 0x0D
    MODULE_PARAM_EST_EXCITATION   = 14,  // 0x0E
    MODULE_PRODUCTINFO            = 15,  // 0x0F
    MODULE_PULSE                  = 16,  // 0x10
    MODULE_ROTATEXDEG             = 17,  // 0x11
    MODULE_SETTINGFILE            = 18,  // 0x12
    MODULE_SPIN_DRUM              = 19,  // 0x13
    MODULE_STOP_DRUM              = 20,  // 0x14
    MODULE_SYSTEMTIMERS           = 21,  // 0x15
    MODULE_WASH                   = 22,  // 0x16

    //! Number of modules that can add Log Messages to the Message Queue.
    NUMBER_OF_MODULES             = 23
} LOG_MODULE_ID_TYPE;


//---------------------------------------------------------------------------------------------------------------------
/**
 * Automated list of unique identifiers for all application specific Log Messages from all modules.
 *
 * The LogPreprocessor.exe tool searches the code for any source files that declare log messages.
 * Every enumerated message from every module is assigned a unique identifier based on the
 * Module Identifier and the Message Identifier.
 *
 * The debugger will then display the enumeration text instead of the decimal value, making
 * viewing the Log messages much easier during debugging.
 */
typedef enum LOG_MODULE_MESSAGE_ID_ENUM
{
    MODULE_API007APP_INVALID_CRC_FAILED                                    = 1,     // 0x0001
    MODULE_API007BULKDATA_MESSAGE_NONE                                     = 256,   // 0x0100
    MODULE_API010_POLL_VAR_TOO_MUCH_DATA                                   = 513,   // 0x0201
    MODULE_API011APPCTRL_TO_MANY_CONSECUTIVE_REGULATIONS_SETS              = 768,   // 0x0300
    MODULE_API011APPCTRL_TO_MANY_CONSECUTIVE_REGULATIONS_REQUESTS          = 769,   // 0x0301
    MODULE_API011APPCTRL_TO_MANY_CONSECUTIVE_STATUS_REQUESTS               = 770,   // 0x0302
    MODULE_API011APPCTRL_MODULE_TIMER_NOT_CREATED                          = 771,   // 0x0303
    MODULE_API013_REMOTE_FUNCTION_FEEDBACK_MESSAGE_NOT_HANDLED             = 1025,  // 0x0401
    MODULE_API221_MOTION_CTRL_PERIODIC_PUBLICATION_FAILED                  = 1281,  // 0x0501
    MODULE_API221_MOTION_CTRL_FEEDBACK_OPCODE_NOT_SUPPORTED                = 1282,  // 0x0502
    MODULE_API221_MOTION_CTRL_INVALID_SYSTEM_MOTOR                         = 1283,  // 0x0503
    MODULE_API221_MOTION_CTRL_INVALID_CUSTOM_COMMAND                       = 1284,  // 0x0504
    MODULE_API221_MOTION_CTRL_INVALID_STANDARD_COMMAND                     = 1285,  // 0x0505
    MODULE_API221_MOTION_CTRL_MOTOR_STOPPED_DUE_TO_TIMEOUT                 = 1286,  // 0x0506
    MODULE_ARBITRATION_UNINITIALIZED_POINTER                               = 1536,  // 0x0600
    MODULE_ARBITRATION_INVALID_REQUESTOR                                   = 1537,  // 0x0601
    MODULE_BANDLIMITER_MESSAGE_NONE                                        = 1792,  // 0x0700
    MODULE_BANDLIMITER_INVALID_BAND_GET_SELECTION                          = 1793,  // 0x0701
    MODULE_BANDLIMITER_SIGNAL_TO_GET_NOT_IN_BAND                           = 1794,  // 0x0702
    MODULE_BANDLIMITER_SIGNAL_TO_GET_NOT_ENABLED                           = 1795,  // 0x0703
    MODULE_BANDLIMITER_INVALID_BAND_RESET_SELECTION                        = 1796,  // 0x0704
    MODULE_BANDLIMITER_SIGNAL_TO_RESET_NOT_IN_BAND                         = 1797,  // 0x0705
    MODULE_BANDLIMITER_INVALID_BAND_ENABLE_SELECTION                       = 1798,  // 0x0706
    MODULE_BANDLIMITER_SIGNAL_TO_ENABLE_NOT_IN_BAND                        = 1799,  // 0x0707
    MODULE_BANDLIMITER_INVALID_BAND_DISABLE_SELECTION                      = 1800,  // 0x0708
    MODULE_BANDLIMITER_SIGNAL_TO_DISABLE_NOT_IN_BAND                       = 1801,  // 0x0709
    MODULE_BANDLIMITER_CASE_MISSING_FOR_INPUT_SIGNAL                       = 1802,  // 0x070A
    MODULE_CALLBACK_HANDLER_ALREADY_REGISTERED                             = 2048,  // 0x0800
    MODULE_CALLBACK_HANDLER_ALREADY_UNREGISTERED                           = 2049,  // 0x0801
    MODULE_CALLBACK_NO_SPACE_TO_REGISTER_HANDLER                           = 2050,  // 0x0802
    MODULE_CALLBACK_ATTEMPT_TO_REGISTER_NULL_HANDLER                       = 2051,  // 0x0803
    MODULE_CALLBACK_ATTEMPT_TO_UNREGISTER_NULL_HANDLER                     = 2052,  // 0x0804
    MODULE_CALLBACK_CONTROL_IS_NULL                                        = 2053,  // 0x0805
    MODULE_CALLBACK_REGISTRY_CORRUPTED                                     = 2054,  // 0x0806
    MODULE_EXTR_PARAMS_SETTING_FILE_PARAMS_ERROR                           = 2305,  // 0x0901
    MODULE_EXTR_PARAMS_API007_PARAMS_ERROR                                 = 2306,  // 0x0902
    MODULE_MODE_INVALID_SYSTEM_TIMER                                       = 2560,  // 0x0A00
    MODULE_MOTION_MESSAGE_NONE                                             = 2816,  // 0x0B00
    MODULE_MOTION_INVALID_MOTION_BEHAVIOR                                  = 2817,  // 0x0B01
    MODULE_MOTION_MCI_REJECTED_CLEAR_ALL_FAILURE                           = 2818,  // 0x0B02
    MODULE_MOTION_SPIN_MAX_DRUM_RPM_EXCEEDED                               = 2819,  // 0x0B03
    MODULE_MOTION_UNCOMMANDED_MOVEMENT_DETECTED                            = 2820,  // 0x0B04
    MODULE_MOTION_WASH_MAX_DRUM_RPM_EXCEEDED                               = 2821,  // 0x0B05
    MODULE_MOTOR_ACTIVITY_PULSE_TIME_OFF_MULTIPLIER_INVALID                = 3072,  // 0x0C00
    MODULE_MOTOR_ACTIVITY_PULSE_TIME_ON_MULTIPLIER_INVALID                 = 3073,  // 0x0C01
    MODULE_MOTOR_ACTIVITY_UNSUPPORTED_MOTOR_ACTIVITY                       = 3074,  // 0x0C02
    MODULE_PARAM_EST_MESSAGE_NONE                                          = 3328,  // 0x0D00
    MODULE_PARAM_EST_CANNOT_CHANGE_MODEL_WHILE_MODULE_IS_ACTIVE            = 3329,  // 0x0D01
    MODULE_PARAM_EST_CANNOT_GET_PARAMETER_VALUE_SETUP_IS_NULL              = 3330,  // 0x0D02
    MODULE_PARAM_EST_INVALID_MODEL                                         = 3331,  // 0x0D03
    MODULE_PARAM_EST_INVALID_ADAPTATION_TYPE                               = 3332,  // 0x0D04
    MODULE_PARAM_EST_INVALID_KALMAN_VALUE                                  = 3333,  // 0x0D05
    MODULE_PARAM_EST_INVALID_FORGETTING_FACTOR                             = 3334,  // 0x0D06
    MODULE_PARAM_EST_INVALID_FILTER_ORDER                                  = 3335,  // 0x0D07
    MODULE_PARAM_EST_INVALID_PARAMETER_SELECTION                           = 3336,  // 0x0D08
    MODULE_PARAM_EST_MODEL_UPDATE_VIOLATION                                = 3337,  // 0x0D09
    MODULE_PARAM_EST_SETUP_PARAMETERS_NULL                                 = 3338,  // 0x0D0A
    MODULE_PARAM_EST_EXCITATION_MESSAGE_NONE                               = 3584,  // 0x0E00
    MODULE_PARAM_EST_EXCITATION_CANNOT_ENABLE_MODULE_SETUP_ADDRESS_IS_NULL = 3585,  // 0x0E01
    MODULE_PARAM_EST_EXCITATION_INVALID_METHOD                             = 3586,  // 0x0E02
    MODULE_PARAM_EST_EXCITATION_INVALID_TIME                               = 3587,  // 0x0E03
    MODULE_PARAM_EST_EXCITATION_MIN_SPEED_VIOLATION                        = 3588,  // 0x0E04
    MODULE_PARAM_EST_EXCITATION_SETUP_PARAMETER_IS_NULL                    = 3589,  // 0x0E05
    MODULE_PRODUCTINFO_INVALID_PROJECT_DATA                                = 3840,  // 0x0F00
    MODULE_PRODUCTINFO_INVALID_BOARD                                       = 3841,  // 0x0F01
    MODULE_PRODUCTINFO_INVALID_PROJECT                                     = 3842,  // 0x0F02
    MODULE_PULSE_COMMAND_MODIFY_BEHAVIOR_IGNORED_STATE_INVALID             = 4097,  // 0x1001
    MODULE_PULSE_INVALID_BEHAVIOR                                          = 4098,  // 0x1002
    MODULE_ROTATEXDEG_MESSAGE_NONE                                         = 4352,  // 0x1100
    MODULE_ROTATEXDEG_PROFILE_NOT_DEFINED                                  = 4353,  // 0x1101
    MODULE_SETTINGFILE_UNHANDLED_SECTION_ENDIANNESS                        = 4608,  // 0x1200
    MODULE_SETTINGFILE_UNHANDLED_HARDCODED_ENDIANNESS                      = 4609,  // 0x1201
    MODULE_SPIN_DRUM_MESSAGE_NONE                                          = 4864,  // 0x1300
    MODULE_SPIN_DRUM_INVALID_ACCEL_TYPE                                    = 4865,  // 0x1301
    MODULE_SPIN_DRUM_INVALID_DRUM_ACCELERATION_TIME                        = 4866,  // 0x1302
    MODULE_SPIN_DRUM_MCI_REJECTED_COMMAND_TO_RUN                           = 4867,  // 0x1303
    MODULE_SPIN_DRUM_FORCED_DEFAULT_ACCELERATION                           = 4868,  // 0x1304
    MODULE_SPIN_DRUM_LIMITED_TO_MAX_ACCELERATION                           = 4869,  // 0x1305
    MODULE_STOP_DRUM_MESSAGE_NONE                                          = 5120,  // 0x1400
    MODULE_STOP_DRUM_MCI_IS_NOT_STOPPING                                   = 5121,  // 0x1401
    MODULE_STOP_DRUM_MCI_REJECTED_COMMAND_TO_STOP                          = 5122,  // 0x1402
    MODULE_SYSTEMTIMERS_INVALID_HANDLE                                     = 5377,  // 0x1501
    MODULE_WASH_MESSAGE_NONE                                               = 5632,  // 0x1600
    MODULE_WASH_PROFILE_NOT_DEFINED                                        = 5633,  // 0x1601

    //! This is a purposely invalid Module/Message ID used to identify invalid Log Messages.
    MODULE_LOG_INVALID_MESSAGE                                             = 65535
} LOG_MODULE_MESSAGE_ID_TYPE;



#endif  // LOG_PRM_H_


