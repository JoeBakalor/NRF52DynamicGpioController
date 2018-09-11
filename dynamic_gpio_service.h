
#ifndef BLUEBIRD_GPIO_SERVICE_H__
#define BLUEBIRD_GPIO_SERVICE_H__

#include <stdint.h>
#include "ble.h"
#include "ble_srv_common.h"

/**
 * @brief This structure contains various status information for our service. 
 * It only holds one entry now, but will be populated with more items as we go.
 * The name is based on the naming convention used in Nordic's SDKs. 
 * 'ble’ indicates that it is a Bluetooth Low Energy relevant structure and 
 * ‘os’ is short for Our Service). 
 */

typedef struct
{
    uint16_t   			conn_handle;    	/**< Handle of the current connection (as provided by the BLE stack, is BLE_CONN_HANDLE_INVALID if not in a connection).*/
    uint16_t                    service_handle;     /**< Handle of Senxis Service (as provided by the BLE stack). */
    ble_gatts_char_handles_t    gpio_one_char_handles;
    ble_gatts_char_handles_t    gpio_two_char_handles;
    ble_gatts_char_handles_t    gpio_three_char_handles;
    ble_gatts_char_handles_t    gpio_four_char_handles;
    ble_gatts_char_handles_t    touch_sensor_handles;
    
}dynamic_gpio_service_t;

extern dynamic_gpio_service_t dynamic_gpio_service;

typedef enum 
{
  ANALOG = 0,
  DIGITAL
}gpio_configuration_t;


/**@brief   Function for handling the Bluebirds GPIO Service's BLE events.
 *
 * @details The Nordic UART Service expects the application to call this function each time an
 * event is received from the SoftDevice. This function processes the event if it
 * is relevant and calls the Nordic UART Service event handler of the
 * application if necessary.
 *
 * @param[in] p_ble_evt     Event received from the SoftDevice.
 * @param[in] p_context     Bluebird GPIO Service structure.
 */
void nrf_gpio_service_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context);
/**@brief Custom Service event handler type. */
//typedef void (*ble_cus_evt_handler_t) (ble_cus_t * p_bas, ble_cus_evt_t * p_evt);

/**@brief Bluebird environmental service initialization
 *
 * @param[in]   p_bluebird_environmental_service       Pointer to Our Service structure.
 */
void nrf_gpio_service_init(dynamic_gpio_service_t *p_dynamic_gpio_service;

/**@brief 
 *
 * @param[in]   p_bluebird_environmental_service       Pointer to bluebird environmental Service structure.
 * @param[in]   temperature          				   New temperature value
 */
void nrf_gpio_one_char_update(dynamic_gpio_service_t *p_bluebird_gpio_service,  uint8_t *gpio_value, uint8_t gpio_value_length);


/**@brief Function for updating temperature characteristic value
 *
 * @param[in]   p_bluebird_environmental_service       Pointer to bluebird environmental Service structure.
 * @param[in]   humidity            				   New humidity value
 */
void nrf_gpio_two_char_update(dynamic_gpio_service_t *p_bluebird_gpio_service,  uint8_t *gpio_value, uint8_t gpio_value_length);

/**@brief 
 *
 * @param[in]   p_bluebird_environmental_service       Pointer to bluebird environmental Service structure.
 * @param[in]   temperature          				   New temperature value
 */
void nrf_gpio_three_char_update(dynamic_gpio_service_t *p_bluebird_gpio_service,  uint8_t *gpio_value, uint8_t gpio_value_length);


/**@brief Function for updating temperature characteristic value
 *
 * @param[in]   p_bluebird_environmental_service       Pointer to bluebird environmental Service structure.
 * @param[in]   humidity            				   New humidity value
 */
void nrf_gpio_four_char_update(dynamic_gpio_service_t *p_bluebird_gpio_service,  uint8_t *gpio_value, uint8_t gpio_value_length);


#endif  /* */