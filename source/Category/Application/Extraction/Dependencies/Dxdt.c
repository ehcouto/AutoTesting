/**
f *  @file
 *
 *  @brief      Source code for the Dxdt module.
 *
 *  @details    Calculates the slope of a torque signal and posts an event for when a peak or trough has been detected.
 *
 *
    @startuml{Dxdt_state_machine.png}
        title "Dxdt State Machine"

        [*] --> INACTIVE

        INACTIVE --> INITIALIZE_BUFFER : evEnable

        INITIALIZE_BUFFER : enter /\t ClearVariablesForNewCalculations()\n\t\t RegisterInitializeBuffer()
        INITIALIZE_BUFFER : exit / UnregisterInitializeBuffer()
        INITIALIZE_BUFFER --> WRITE_BUFFER : evBufferFull
        INITIALIZE_BUFFER --> INACTIVE : evDisable

        WRITE_BUFFER : enter / RegisterWriteBuffer()
        WRITE_BUFFER : exit / UnregisterWriteBuffer()
        WRITE_BUFFER --> INACTIVE : evDisable

    @enduml

 *
 *  $Header: Dxdt.c 1.5 2016/03/10 14:05:08EST Paul F Spindler (SPINDPF) Exp  $
 *
 *  @copyright  Copyright 2014-$Date: 2016/03/10 14:05:08EST $. Whirlpool Corporation. All rights reserved - CONFIDENTIAL
 */
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------


//-------------------------------------- Include Files ----------------------------------------------------------------
#include "C_Extensions.h"

#include "Dxdt.h"
#include "Dxdt_prv.h"

#include "AverageTorque.h"
#include "Callback.h"
#include "SNRD.h"

#include "string.h"

//=====================================================================================================================
//=== StateEngineCrank (v2.0) Auto-Generated Definitions for Dxdt State Machine
//=====================================================================================================================
// !! StateEngineCrank cannot update this code if you change the header !!


//! Unique names used to identify states of the Dxdt state machine.
typedef enum DXDT_STATE_ENUM
{
    STATE_NONE              = 0,

    STATE_INACTIVE          = 1,                    //!< (initial_state)
    STATE_INITIALIZE_BUFFER = 2,
    STATE_WRITE_BUFFER      = 3,

    STATE_DXDT_END          = 4
} DXDT_STATE_TYPE;

//! The state variable for the Dxdt state machine.
//! State machines with a history pseudostate may write to this variable during initialization.
//! Other state machines must only write to this variable through auto-generated functions.
static DXDT_STATE_TYPE Dxdt_State;

// -- Private Function Prototypes used by the state machine for customization functions --
static void ClearVariablesForNewCalculations(void);
static void RegisterInitializeBuffer(void);
static void RegisterWriteBuffer(void);
static void UnregisterInitializeBuffer(void);
static void UnregisterWriteBuffer(void);

// -- Private Function Prototypes used by the state machine for standard state machine functions --
static void DxdtExecute(void);
static void DxdtInitialize(void);
static void EventBufferFull(void);
static void EventDisable(void);
static void EventEnable(void);

// !! StateEngineCrank cannot update this code if you change the footer !!
// End of StateEngineCrank Auto-Generated Code.


//-------------------------------------- PRIVATE (Variables, Constants & Defines) -------------------------------------

/*
 *   Define the size of the Torque Buffer.
 *   The buffer must be able to hold the number of 10msec readings needed to write to the SNRD module at the specified
 *   time base.
 */
#define TORQUE_BUFFER_MAX_SIZE      ( (SNRD_BUFFER_SIZE - 1) * DXDT_CONFIG_MAX_TIME_BASE + 1 )

#define TORQUE_BUFFER_SIZE			( (SNRD_BUFFER_SIZE - 1) * DXDT_CONFIG_TIME_BASE + 1 )

//! defines the index type for the Torque Buffer
typedef uint8 TORQUE_BUFFER_INDEX_TYPE;

//! variables used to hold the torque readings in a buffer
typedef struct
{
    TORQUE_BUFFER_INDEX_TYPE    Index;                      	// index of the most current Value
    AVERAGE_TORQUE_TYPE         Value[TORQUE_BUFFER_MAX_SIZE];  // torque reading
} TORQUE_BUFFER_TYPE;

//! define the callback control structure module static variable
DEFINE_CALLBACK_CONTROL_STRUCTURE(Dxdt_Callback_Control_Structure, SIZE_OF_CALLBACK_REGISTER);

//! module static variables
static struct
{
    uint8               Scan_Rate_Count;                // counter used to create the scan rate defined by DXDT_CONFIG_TIME_BASE
    float32             Slope_Current;                  // current calculated slope of the torque
    AVERAGE_TORQUE_TYPE Torque_Max;                     // maximum calculated torque
    AVERAGE_TORQUE_TYPE Torque_Min;                     // minimum calculated torque
    TORQUE_BUFFER_TYPE  Torque_Buffer;
} Dxdt;

//-------------------------------------- PRIVATE (Function Prototypes) ------------------------------------------------

static float32 CalculateDxDtSlope(void);
static TORQUE_BUFFER_INDEX_TYPE CalculateTorqueBufferIndex(TORQUE_BUFFER_INDEX_TYPE offset);
static void DetectPeaksTroughs(void);
static void IncrementBufferIndex(void);
static void InitializeBuffer(AVERAGE_TORQUE_EVENT_TYPE event);
static void WriteAverageTorqueValueToBuffer(void);
static void WriteBuffer(AVERAGE_TORQUE_EVENT_TYPE event);

//=====================================================================================================================
//-------------------------------------- Public Functions -------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      It Initializes the module Dxdt and its variables.
 */
void Dxdt__Initialize(void)
{
    memset(&Dxdt, 0, sizeof(Dxdt));
    Callback__Initialize(&Dxdt_Callback_Control_Structure);
    ClearVariablesForNewCalculations();
    DxdtInitialize();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Disables the module.
 */
void Dxdt__Disable(void)
{
    EventDisable();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Enables the module.
 */
void Dxdt__Enable(void)
{
    EventEnable();
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Returns the Dxdt data.
 *  @param      data    pointer to the Application defined DXDT_TORQUE_DATA_TYPE
 */
void Dxdt__GetData(DXDT_TORQUE_DATA_TYPE *data)
{
    data->Max   = Dxdt.Torque_Max;
    data->Min   = Dxdt.Torque_Min;
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Registers an event handler with the module
 *
 *  @param      callback_address   The callback function to register
 */
void Dxdt__RegisterEventHandler(DXDT_EVENT_HANDLER_TYPE event_handler)
{
    Callback__Register(&Dxdt_Callback_Control_Structure, (CALLBACK_HANDLER_TYPE)event_handler);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Unregisters an event handler with the module
 *
 *  @param      callback_address    The callback function to register
 */
void Dxdt__UnregisterEventHandler(DXDT_EVENT_HANDLER_TYPE event_handler)
{
    Callback__Unregister(&Dxdt_Callback_Control_Structure, (CALLBACK_HANDLER_TYPE)event_handler);
}

//=====================================================================================================================
//-------------------------------------- Private Functions ------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Calculates the DxDt of the torque signal using the SNRD filter.
 *  @param      reference_index     The tsp buffer index to use as base index  when calculating SNRD
 *
 *  @return     the current slope of the torque in newton meters
 */
static float32 CalculateDxDtSlope(void)
{
    SNRD_DATA_TYPE      snrd_data;
    AVERAGE_TORQUE_TYPE snrd_buffer[SNRD_BUFFER_SIZE];
    uint8               snrd_buf_index;

    // create a copy of the Dxdt.Torque_Buffer.Reading with the most recent element of Dxdt.Torque_Buffer.Reading at index 0
    for ( snrd_buf_index = 0; snrd_buf_index < SNRD_BUFFER_SIZE; snrd_buf_index++ )
    {
        // remove every 5th reading from the Torque Buffer to create a 50msec time base for the values being used
        snrd_buffer[snrd_buf_index] = Dxdt.Torque_Buffer.Value[CalculateTorqueBufferIndex(snrd_buf_index * DXDT_CONFIG_TIME_BASE)];
    }

    snrd_data.Buffer = snrd_buffer;
    snrd_data.Ts_Der = SNRD_TS_DER;
    return (SNRD__CalcDeriv(&snrd_data));
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Calculates the index in the Dxdt.Torque_Buffer.Index using the passed offset.
 *
 *  @param      offset      The offset from the current value of Dxdt.Torque_Buffer.Index
 *
 *  @return     The calculated index in buffer Dxdt.Torque_Buffer.Value
 */
static TORQUE_BUFFER_INDEX_TYPE CalculateTorqueBufferIndex(TORQUE_BUFFER_INDEX_TYPE offset)
{
    return ((Dxdt.Torque_Buffer.Index - offset + TORQUE_BUFFER_SIZE) % TORQUE_BUFFER_SIZE);
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Uses the SNRD filter to determine if the derivative indicates a change in slope.  Change in slope from
 *              + to - indicates a peak, - to + indicates a trough
 */
static void DetectPeaksTroughs(void)
{
    uint8   offset;

    float32 slope_previous = Dxdt.Slope_Current;        // save the current calculated slope value as the previous
    Dxdt.Slope_Current = CalculateDxDtSlope();                  // calculate the current slope

    if (( slope_previous > 0.0f ) &&                                                                 // previous slope was + AND
        ( Dxdt.Slope_Current <= 0.0f ))                                                              // current slope is -
    {                                                                                               // peak detected
        // search the buffer for a peak
        offset = 1;
        Dxdt.Torque_Max = Dxdt.Torque_Buffer.Value[CalculateTorqueBufferIndex(offset)];
        for ( offset = 2; offset < TORQUE_BUFFER_SIZE; offset++ )
        {
            if ( Dxdt.Torque_Buffer.Value[CalculateTorqueBufferIndex(offset)] > Dxdt.Torque_Max )
            {
                Dxdt.Torque_Max = Dxdt.Torque_Buffer.Value[CalculateTorqueBufferIndex(offset)];
            }
            // else the torque value at the current index is not greater than the saved peak, do nothing
        }
        Callback__Notify(&Dxdt_Callback_Control_Structure, (CALLBACK_EVENT_TYPE)DXDT_EV_MAX_TORQUE_DETECTED);
    }
    else if (( slope_previous < 0.0f ) &&                                                            // previous slope was - AND
             ( Dxdt.Slope_Current >= 0.0f ))                                                         // current slope is +
    {                                                                                               // trough detected
        // search the buffer for a trough
        offset = 1;
        Dxdt.Torque_Min = Dxdt.Torque_Buffer.Value[CalculateTorqueBufferIndex(offset)];
        for ( offset = 2; offset < TORQUE_BUFFER_SIZE; offset++ )
        {
            if ( Dxdt.Torque_Buffer.Value[CalculateTorqueBufferIndex(offset)] < Dxdt.Torque_Min )
            {
                Dxdt.Torque_Min = Dxdt.Torque_Buffer.Value[CalculateTorqueBufferIndex(offset)];
            }
            // else the torque value at the current is not less than the saved trough, do nothing
        }
        Callback__Notify(&Dxdt_Callback_Control_Structure, (CALLBACK_EVENT_TYPE)DXDT_EV_MIN_TORQUE_DETECTED);
    }
    // else no peak nor trough detected
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Increments the buffer index and handles overflow.
 */
static void IncrementBufferIndex(void)
{
    Dxdt.Torque_Buffer.Index++;
    if ( Dxdt.Torque_Buffer.Index >= TORQUE_BUFFER_SIZE )
    {
        Dxdt.Torque_Buffer.Index = 0;
    }
    // else Dxdt.Torque_Buffer.Index < TORQUE_BUFFER_SIZE, do nothing
}

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Handles events from the AverageTorque module.
 *
 *  @param      event   The event being posted from the AveragTorque module
 */
static void InitializeBuffer(AVERAGE_TORQUE_EVENT_TYPE event)
{
    switch ( event )
    {
        case AVERAGE_TORQUE_EV_NEW_VALUE_CALCULATED:
            WriteAverageTorqueValueToBuffer();
            // check if the buffer is full
            if ( Dxdt.Torque_Buffer.Index >= TORQUE_BUFFER_SIZE - 1 )
            {                                                           // buffer is full
                DetectPeaksTroughs();
                IncrementBufferIndex();
                Dxdt.Scan_Rate_Count = 0;
                EventBufferFull();
            }
            else // the buffer is still being stuffed, torque peaks and troughs cannot be detected
            {
                IncrementBufferIndex();
            }
            break;
        default:
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      write the average torque value to the buffer.
 */
static void WriteAverageTorqueValueToBuffer(void)
{
    Dxdt.Torque_Buffer.Value[Dxdt.Torque_Buffer.Index] = AverageTorque__GetValue();
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Handles events from the AverageTorque module.
 *
 *  @param      event   The event being posted from the AveragTorque module
 */
static void WriteBuffer(AVERAGE_TORQUE_EVENT_TYPE event)
{
    switch ( event )
    {
        case AVERAGE_TORQUE_EV_NEW_VALUE_CALCULATED:
            WriteAverageTorqueValueToBuffer();
            Dxdt.Scan_Rate_Count++;
            if ( Dxdt.Scan_Rate_Count >= DXDT_CONFIG_TIME_BASE )
            {
                Dxdt.Scan_Rate_Count = 0;
                DetectPeaksTroughs();
            }
            IncrementBufferIndex();
            break;
        default:
            break;
    }
}

//=====================================================================================================================
//  --- Private State Machine Customization Functions -----------------------------------------------------------------
//=====================================================================================================================

//---------------------------------------------------------------------------------------------------------------------
/**
 *  @brief      Clears the module data used to calculate the dxdt data.
 */
static void ClearVariablesForNewCalculations(void)
{
    memset(&Dxdt.Torque_Buffer, 0, sizeof(Dxdt.Torque_Buffer));
    Dxdt.Slope_Current                  = 0.0f;
    Dxdt.Torque_Max                     = SHRT_MIN;
    Dxdt.Torque_Min                     = SHRT_MAX;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *
 */
static void RegisterInitializeBuffer(void)
{
    AverageTorque__RegisterCallback(&InitializeBuffer);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *
 */
static void RegisterWriteBuffer(void)
{
    AverageTorque__RegisterCallback(&WriteBuffer);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *
 */
static void UnregisterInitializeBuffer(void)
{
    AverageTorque__UnregisterCallback(&InitializeBuffer);
}


//---------------------------------------------------------------------------------------------------------------------
/**
 *
 */
static void UnregisterWriteBuffer(void)
{
    AverageTorque__UnregisterCallback(&WriteBuffer);
}


//=====================================================================================================================
//=== StateEngineCrank (v2.0) Auto-Generated Functions for Dxdt State Machine
//=====================================================================================================================
// !! StateEngineCrank cannot update this code if you change the header !!


//---------------------------------------------------------------------------------------------------------------------
/**
 * The main execution function for the Dxdt state machine.
 * This function should be called periodically while the state machine is running.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void DxdtExecute(void)
{
    // None of the states have an Execute function -- nothing to do.
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * The initialization function for the Dxdt state machine.
 * This function resets the state machine to the initial state.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void DxdtInitialize(void)
{
    Dxdt_State = STATE_INACTIVE;
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Ev Buffer Full Event for the Dxdt state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventBufferFull(void)
{
    switch (Dxdt_State)
    {
        case STATE_INITIALIZE_BUFFER:
            UnregisterInitializeBuffer();           // STATE_INITIALIZE_BUFFER exit function.
            Dxdt_State = STATE_WRITE_BUFFER;
            RegisterWriteBuffer();                  // STATE_WRITE_BUFFER entry function.
            break;

        default:
            // Event is ignored for all other values of Dxdt_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Ev Disable Event for the Dxdt state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventDisable(void)
{
    switch (Dxdt_State)
    {
        case STATE_INITIALIZE_BUFFER:
            UnregisterInitializeBuffer();           // STATE_INITIALIZE_BUFFER exit function.
            Dxdt_State = STATE_INACTIVE;
            break;

        case STATE_WRITE_BUFFER:
            UnregisterWriteBuffer();                // STATE_WRITE_BUFFER exit function.
            Dxdt_State = STATE_INACTIVE;
            break;

        default:
            // Event is ignored for all other values of Dxdt_State.
            break;
    }
}


//---------------------------------------------------------------------------------------------------------------------
/**
 * Process the Ev Enable Event for the Dxdt state machine.
 *
 *              >>> DO NOT MODIFY THIS FUNCTION <<<
 *
 * This function was auto-generated and must not be edited manually.
 * Change the UML used to generate this function and regenerate the source code.
 */
static void EventEnable(void)
{
    switch (Dxdt_State)
    {
        case STATE_INACTIVE:
            Dxdt_State = STATE_INITIALIZE_BUFFER;
            ClearVariablesForNewCalculations();     // STATE_INITIALIZE_BUFFER entry function.
            RegisterInitializeBuffer();             // STATE_INITIALIZE_BUFFER entry function.
            break;

        default:
            // Event is ignored for all other values of Dxdt_State.
            break;
    }
}


// !! StateEngineCrank cannot update this code if you change the footer !!
// End of StateEngineCrank Auto-Generated Code.
