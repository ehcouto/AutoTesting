/*
 * Api221FilterMciErr.h
 *
 *  Created on: 12 de dez de 2017
 *      Author: SILVAMC
 */

#ifndef API221FILTERMCIERR_H_
    #define API221FILTERMCIERR_H_


uint32 Api221FilterMciErr__GetMCUErrorList(MOTOR_ENUM_TYPE motor);
uint8 Api221FilterMciErr__GetError(MOTOR_ENUM_TYPE motor, MCI_ERROR_TYPE error);
uint8 Api221FilterMciErr__ClearError(MOTOR_ENUM_TYPE motor);

#endif /* API221FILTERMCIERR_H_ */
