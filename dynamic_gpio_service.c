#include <stdint.h>
#include <string.h>
#include "uuids.h"
#include "dynamic_gpio_service.h"
#include "dynamic_gpio_controller.h"
#include "ble_srv_common.h"
#include "app_error.h"
#include "SEGGER_RTT.h"

#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "pca10040.h"
#include "nordic_common.h"
#include "nrf.h"
#include "nrf_sdh.h"
#include "nrf_sdh_soc.h"


//#define OLD
#define NEW

/**@brief Function for adding GPIO1 characteristic to  GPIO service 
 *
 * @param[in]   p_dynamic_gpio_service         GPIO service structure 
 *
 */
static uint32_t gpio_one_char_add(dynamic_gpio_service_t *p_dynamic_gpio_service)
{
    uint32_t   err_code = 0; // Variable to hold return codes from library and softdevice functions
    
    // : add GPIO1 characteristic UUID
    ble_uuid_t          char_uuid;
    ble_uuid128_t       base_uuid = UUID_BASE;
    char_uuid.uuid                = UUID_CHARACTERISTIC_GPIO1;
    sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
    APP_ERROR_CHECK(err_code);
    
    // : add read/write properties to our characteristic
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read = 1;
    char_md.char_props.write = 1;

    // : configuring Client Characteristic Configuration Descriptor metadata and add to char_md structure
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc                = BLE_GATTS_VLOC_STACK;    
    char_md.p_cccd_md           = &cccd_md;
    char_md.char_props.notify   = 1;
   
    
    // : configure the attribute metadata
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md)); 
    attr_md.vloc        = BLE_GATTS_VLOC_STACK;   
    
    // : set read/write security levels to our characteristic
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    
    // : configure the characteristic value attribute
    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));        
    attr_char_value.p_uuid      = &char_uuid;
    attr_char_value.p_attr_md   = &attr_md;
    
    // : set characteristic length in number of bytes
    attr_char_value.max_len     = 2;
    attr_char_value.init_len    = 2;
    uint8_t value[2]            = {0x00, 0x00};
    attr_char_value.p_value     = value;

    // : add GPIO1 characteristic to the service
    err_code = sd_ble_gatts_characteristic_add(p_dynamic_gpio_service->service_handle,
                                       &char_md,
                                       &attr_char_value,
                                       &p_dynamic_gpio_service->gpio_one_char_handles);
    APP_ERROR_CHECK(err_code);
    
    printf("\r\nService handle: %#x\n\r", p_dynamic_gpio_service->service_handle);
    printf("Char value handle: %#x\r\n", p_dynamic_gpio_service->gpio_one_char_handles.value_handle);//char_handles.value_handle);
    printf("Char cccd handle: %#x\r\n\r\n", p_dynamic_gpio_service->gpio_one_char_handles.cccd_handle);//char_handles.cccd_handle);

    return NRF_SUCCESS;
}

/**@brief Function for adding GPIO2 characteristic to  GPIO service 
 *
 * @param[in]   p_dynamic_gpio_service         GPIO service structure 
 *
 */
static uint32_t gpio_two_char_add(dynamic_gpio_service_t *p_dynamic_gpio_service)
{
    uint32_t   err_code = 0; // Variable to hold return codes from library and softdevice functions
    
    // : add GPIO2 characteristic UUID
    ble_uuid_t          char_uuid;
    ble_uuid128_t       base_uuid = UUID_BASE;
    char_uuid.uuid                = UUID_CHARACTERISTIC_GPIO2;
    sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
    APP_ERROR_CHECK(err_code);
    

    // : add read/write properties to our characteristic
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read = 1;
    char_md.char_props.write = 1;

    
    // : configuring Client Characteristic Configuration Descriptor metadata and add to char_md structure
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc                = BLE_GATTS_VLOC_STACK;    
    char_md.p_cccd_md           = &cccd_md;
    char_md.char_props.notify   = 1;
   
    
    // : configure the attribute metadata
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md)); 
    attr_md.vloc        = BLE_GATTS_VLOC_STACK;   
    
    
    // : set read/write security levels to our characteristic
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    
    
    // : configure the characteristic value attribute
    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));        
    attr_char_value.p_uuid      = &char_uuid;
    attr_char_value.p_attr_md   = &attr_md;
    
    // : set characteristic length in number of bytes
    attr_char_value.max_len     = 2;
    attr_char_value.init_len    = 2;
    uint8_t value[2]            = {0x00, 0x00};
    attr_char_value.p_value     = value;

    // : add GPIO2 characteristic to the service
    err_code = sd_ble_gatts_characteristic_add(p_dynamic_gpio_service->service_handle,
                                       &char_md,
                                       &attr_char_value,
                                       &p_dynamic_gpio_service->gpio_two_char_handles);
    APP_ERROR_CHECK(err_code);
    
    printf("\r\nService handle: %#x\n\r", p_dynamic_gpio_service->service_handle);
    printf("Char value handle: %#x\r\n", p_dynamic_gpio_service->gpio_two_char_handles.value_handle);//char_handles.value_handle);
    printf("Char cccd handle: %#x\r\n\r\n", p_dynamic_gpio_service->gpio_two_char_handles.cccd_handle);//char_handles.cccd_handle);

    return NRF_SUCCESS;
}

/**@brief Function for adding GPIO3 characteristic to  GPIO service 
 *
 * @param[in]   p_dynamic_gpio_service         GPIO service structure 
 *
 */
static uint32_t gpio_three_char_add(dynamic_gpio_service_t *p_dynamic_gpio_service)
{
    uint32_t   err_code = 0; // Variable to hold return codes from library and softdevice functions
    
    // : add GPIO3 characteristic UUID
    ble_uuid_t          char_uuid;
    ble_uuid128_t       base_uuid = UUID_BASE;
    char_uuid.uuid                = UUID_CHARACTERISTIC_GPIO3;
    sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
    APP_ERROR_CHECK(err_code);
    

    // : add read/write properties to our characteristic
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read = 1;
    char_md.char_props.write = 1;

    
    // : configuring Client Characteristic Configuration Descriptor metadata and add to char_md structure
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc                = BLE_GATTS_VLOC_STACK;    
    char_md.p_cccd_md           = &cccd_md;
    char_md.char_props.notify   = 1;
   
    
    // : configure the attribute metadata
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md)); 
    attr_md.vloc        = BLE_GATTS_VLOC_STACK;   
    
    
    // : set read/write security levels to our characteristic
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    
    
    // : configure the characteristic value attribute
    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));        
    attr_char_value.p_uuid      = &char_uuid;
    attr_char_value.p_attr_md   = &attr_md;
    
    // : set characteristic length in number of bytes
    attr_char_value.max_len     = 2;
    attr_char_value.init_len    = 2;
    uint8_t value[2]            = {0x00, 0x00};
    attr_char_value.p_value     = value;

    // : add GPIO2 characteristic to the service
    err_code = sd_ble_gatts_characteristic_add(p_dynamic_gpio_service->service_handle,
                                       &char_md,
                                       &attr_char_value,
                                       &p_dynamic_gpio_service->gpio_three_char_handles);
    APP_ERROR_CHECK(err_code);
    
    printf("\r\nService handle: %#x\n\r", p_dynamic_gpio_service->service_handle);
    printf("Char value handle: %#x\r\n", p_dynamic_gpio_service->gpio_three_char_handles.value_handle);//char_handles.value_handle);
    printf("Char cccd handle: %#x\r\n\r\n", p_dynamic_gpio_service->gpio_three_char_handles.cccd_handle);//char_handles.cccd_handle);

    return NRF_SUCCESS;
}


/**@brief Function for adding GPIO4 characteristic to  GPIO service 
 *
 * @param[in]   p_dynamic_gpio_service         GPIO service structure 
 *
 */
static uint32_t gpio_four_char_add(dynamic_gpio_service_t *p_dynamic_gpio_service)
{
    uint32_t   err_code = 0; // Variable to hold return codes from library and softdevice functions
    
    // : add GPIO3 characteristic UUID
    ble_uuid_t          char_uuid;
    ble_uuid128_t       base_uuid = UUID_BASE;
    char_uuid.uuid                = UUID_CHARACTERISTIC_GPIO4;
    sd_ble_uuid_vs_add(&base_uuid, &char_uuid.type);
    APP_ERROR_CHECK(err_code);
    

    // : add read/write properties to our characteristic
    ble_gatts_char_md_t char_md;
    memset(&char_md, 0, sizeof(char_md));
    char_md.char_props.read = 1;
    char_md.char_props.write = 1;

    
    // : configuring Client Characteristic Configuration Descriptor metadata and add to char_md structure
    ble_gatts_attr_md_t cccd_md;
    memset(&cccd_md, 0, sizeof(cccd_md));
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
    cccd_md.vloc                = BLE_GATTS_VLOC_STACK;    
    char_md.p_cccd_md           = &cccd_md;
    char_md.char_props.notify   = 1;
   
    
    // : configure the attribute metadata
    ble_gatts_attr_md_t attr_md;
    memset(&attr_md, 0, sizeof(attr_md)); 
    attr_md.vloc        = BLE_GATTS_VLOC_STACK;   
    
    
    // : set read/write security levels to our characteristic
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
    
    
    // : configure the characteristic value attribute
    ble_gatts_attr_t    attr_char_value;
    memset(&attr_char_value, 0, sizeof(attr_char_value));        
    attr_char_value.p_uuid      = &char_uuid;
    attr_char_value.p_attr_md   = &attr_md;
    
    // : set characteristic length in number of bytes
    attr_char_value.max_len     = 2;
    attr_char_value.init_len    = 2;
    uint8_t value[2]            = {0x00, 0x00};
    attr_char_value.p_value     = value;

    // : add GPIO2 characteristic to the service
    err_code = sd_ble_gatts_characteristic_add(p_dynamic_gpio_service->service_handle,
                                       &char_md,
                                       &attr_char_value,
                                       &p_dynamic_gpio_service->gpio_four_char_handles);
    APP_ERROR_CHECK(err_code);
    
    printf("\r\nService handle: %#x\n\r", p_dynamic_gpio_service->service_handle);
    printf("Char value handle: %#x\r\n", p_dynamic_gpio_service->gpio_four_char_handles.value_handle);//char_handles.value_handle);
    printf("Char cccd handle: %#x\r\n\r\n", p_dynamic_gpio_service->gpio_four_char_handles.cccd_handle);//char_handles.cccd_handle);

    return NRF_SUCCESS;
}


/*  Handle GPIO Service events */ 
void dynamic_gpio_service_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context)
{
    //printf("\r\nBB SERVICE EVENT\n\r");
    
    if ((p_context == NULL) || (p_ble_evt == NULL))
    {
        NRF_LOG_INFO("Invalid GPIO Service Event");
        return;
    }

    /**/
    const uint8_t *received_data;
    uint8_t received_data_length;

    /**/
    dynamic_gpio_service_t * p_dynamic_gpio_service = (dynamic_gpio_service_t *)p_context;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            //on_connect(p_nus, p_ble_evt);
            break;

        case BLE_GATTS_EVT_WRITE:

            NRF_LOG_INFO("GPIO Service write event !!!!!");
            ble_gatts_evt_write_t const * p_evt_write = &p_ble_evt->evt.gatts_evt.params.write;

            /*=========================== GPIO1 ==============================*/
            if ( (p_evt_write->handle == p_dynamic_gpio_service->gpio_one_char_handles.value_handle) /*&& (p_evt_write->len == 2)*/ )
            {
               received_data = p_evt_write->data;
               received_data_length =  p_evt_write->len;
               
               for (int i = 0; i < received_data_length; i++)
               {
                  NRF_LOG_INFO("%02x", received_data[i]);
               }

               if (received_data[0] == 0)
               {
                  NRF_LOG_INFO("Set GPIO1 as an input");
                  set_gpio_as_input(1);
                  /* SET GPIO1 AS INPUT.  THE CALL REQUIRES PULL DIRECTION.  DOES THE TEKNEST USER CONFIGURE THIS? */
               }
               else if (received_data[0]  == 1)
               {
                  set_gpio_as_output(1);
                  nrf_gpio_pin_write(DGPIO1, received_data[1]);
                  NRF_LOG_INFO("Set GPIO1 as an output and set value to %02x", received_data[1]);
               }
               
                NRF_LOG_INFO("GPIO1 Char written to !!!!!");
            } 
            /*=========================== GPIO2 ==============================*/
            else if ( (p_evt_write->handle == p_dynamic_gpio_service->gpio_two_char_handles.value_handle) /*&& (p_evt_write->len == 2)*/ )
            {
               received_data = p_evt_write->data;
               received_data_length =  p_evt_write->len;
               
               /**/
               for (int i = 0; i < received_data_length; i++)
               {
                  NRF_LOG_INFO("%02x", received_data[i]);
               }

               if (received_data[0] == 0)
               {
                  NRF_LOG_INFO("Set GPIO2 as an input");
                  set_gpio_as_input(2);
               }
               else if (received_data[0]  == 1)
               {
                  set_gpio_as_output(2);
                  nrf_gpio_pin_write(DGPIO2, received_data[1]);
                  NRF_LOG_INFO("Set GPIO2 as an output and set value to %02x", received_data[1]);
               }

              NRF_LOG_INFO("GPIO2 Char written to !!!!!");
            }
            /*=========================== GPIO3 ==============================*/
            else if ( (p_evt_write->handle == p_dynamic_gpio_service->gpio_three_char_handles.value_handle) )
            {
               received_data = p_evt_write->data;
               received_data_length =  p_evt_write->len;
               
               /**/
               for (int i = 0; i < received_data_length; i++)
               {
                  NRF_LOG_INFO("%02x", received_data[i]);
               }

               if (received_data[0] == 0)
               {
                  NRF_LOG_INFO("Set GPIO3 as an input");
                  set_gpio_as_input(3);
               }
               else if (received_data[0]  == 1)
               {
                  set_gpio_as_output(3);
                  nrf_gpio_pin_write(DGPIO3, received_data[1]);
                  NRF_LOG_INFO("Set GPIO3 as an output and set value to %02x", received_data[1]);
               }

              NRF_LOG_INFO("GPIO3 Char written to !!!!!");
            }
            /*=========================== GPIO4 ==============================*/
            else if ( (p_evt_write->handle == p_dynamic_gpio_service->gpio_four_char_handles.value_handle) )
            {
               received_data = p_evt_write->data;
               received_data_length =  p_evt_write->len;
               
               /**/
               for (int i = 0; i < received_data_length; i++)
               {
                  NRF_LOG_INFO("%02x", received_data[i]);
               }

               if (received_data[0] == 0)
               {
                  NRF_LOG_INFO("Set GPIO4 as an input");
                  set_gpio_as_input(4);

               }
               else if (received_data[0]  == 1)
               {
                  set_gpio_as_output(4);
                  nrf_gpio_pin_write(DGPIO4, received_data[1]);
                  NRF_LOG_INFO("Set GPIO4 as an output and set value to %02x", received_data[1]);
               }
              NRF_LOG_INFO("GPIO4 Char written to !!!!!");
            }

            break;

        case BLE_GATTS_EVT_HVN_TX_COMPLETE:
            //on_hvx_tx_complete(p_nus, p_ble_evt);
            break;

        default:
            // No implementation needed.
            break;
    }
}


/**@brief Function for initializing GPIO service.
 *
 * @param[in]   p__environmental_service         environmental service structure
 *
 */
void dynamic_gpio_service_init(dynamic_gpio_service_t *p_dynamic_gpio_service)
{
    // : Declare 16 bit service and 128 bit base UUIDs and add them to BLE stack table     
    uint32_t   err_code;
    ble_uuid_t        service_uuid;
    ble_uuid128_t     base_uuid = UUID_BASE;
    service_uuid.uuid = UUID_SERVICE_DYNAMIC_GPIO;
    err_code = sd_ble_uuid_vs_add(&base_uuid, &service_uuid.type);
    APP_ERROR_CHECK(err_code);

    // : add  GPIO service
    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &service_uuid,
                                        &p_dynamic_gpio_service->service_handle);
    APP_ERROR_CHECK(err_code);

    // : add characteristics to  GPIO service 
    gpio_one_char_add(p_dynamic_gpio_service);
    gpio_two_char_add(p_dynamic_gpio_service);
    gpio_three_char_add(p_dynamic_gpio_service);
    gpio_four_char_add(p_dynamic_gpio_service);
    touch_sensor_char_add(p_dynamic_gpio_service);
}

/**@brief 
 *
 * @param[in]   p_dynamic_gpio_service                  Pointer to  GPIO Service structure.
 * @param[in]   gpio_value          				   New gpio value
 * @param[in]   gpio_configuration_t                   configured as input or output 
 */
uint32_t nrf_gpio_one_char_update(dynamic_gpio_service_t *p_dynamic_gpio_service, uint8_t *gpio_value, uint8_t gpio_value_length)
{
    if (p_dynamic_gpio_service == NULL)
    {
        return NRF_ERROR_NULL;
    }

    //uint8_t gpio_one_update[2] = { gpio_value, gpio_configuration}; 

    uint32_t err_code = NRF_SUCCESS;
    ble_gatts_value_t gatts_value;

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = gpio_value_length;//2;//sizeof(uint8_t);
    gatts_value.offset  = 0;
    gatts_value.p_value = gpio_value;//(uint8_t *)gpio_one_update;

    // Update database.
    err_code = sd_ble_gatts_value_set(p_dynamic_gpio_service->conn_handle,
                                      p_dynamic_gpio_service->gpio_one_char_handles.value_handle,//->battery_level_char_handles.value_handle,//battery_level_handles.value_handle,
                                      &gatts_value);
    if (err_code == NRF_SUCCESS)
    {

    }
    else
    {
        return err_code;
    }

    // Send value if connected and notifying.
    if ((p_dynamic_gpio_service->conn_handle != BLE_CONN_HANDLE_INVALID))// && p_bas->is_notification_supported)
    {
        ble_gatts_hvx_params_t hvx_params;

        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = p_dynamic_gpio_service->gpio_one_char_handles.value_handle;//->battery_level_handles.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = gatts_value.offset;
        hvx_params.p_len  = &gatts_value.len;
        hvx_params.p_data = gatts_value.p_value;

        err_code = sd_ble_gatts_hvx(p_dynamic_gpio_service->conn_handle, &hvx_params);
    }
    else
    {
        err_code = NRF_ERROR_INVALID_STATE;
    }

    return err_code;
}



/**@brief 
 *
 * @param[in]   p__environmental_service       Pointer to  environmental Service structure.
 * @param[in]   temperature          				   New temperature value
 */
uint32_t nrf_gpio_two_char_update(dynamic_gpio_service_t *p_dynamic_gpio_service, uint8_t *gpio_value, uint8_t gpio_value_length)
{
    if (p_dynamic_gpio_service == NULL)
    {
        return NRF_ERROR_NULL;
    }

    //uint8_t gpio_two_update[2] = { gpio_value, gpio_configuration}; 

    uint32_t err_code = NRF_SUCCESS;
    ble_gatts_value_t gatts_value;

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = gpio_value_length;//2;//sizeof(uint8_t);
    gatts_value.offset  = 0;
    gatts_value.p_value = gpio_value;//(uint8_t *)gpio_two_update;

    // Update database.
    err_code = sd_ble_gatts_value_set(p_dynamic_gpio_service->conn_handle,
                                      p_dynamic_gpio_service->gpio_two_char_handles.value_handle,//->battery_level_char_handles.value_handle,//battery_level_handles.value_handle,
                                      &gatts_value);
    if (err_code == NRF_SUCCESS)
    {

    }
    else
    {
        return err_code;
    }

    // Send value if connected and notifying.
    if ((p_dynamic_gpio_service->conn_handle != BLE_CONN_HANDLE_INVALID))// && p_bas->is_notification_supported)
    {
        ble_gatts_hvx_params_t hvx_params;

        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = p_dynamic_gpio_service->gpio_two_char_handles.value_handle;//->battery_level_handles.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = gatts_value.offset;
        hvx_params.p_len  = &gatts_value.len;
        hvx_params.p_data = gatts_value.p_value;

        err_code = sd_ble_gatts_hvx(p_dynamic_gpio_service->conn_handle, &hvx_params);
    }
    else
    {
        err_code = NRF_ERROR_INVALID_STATE;
    }
    //}

    return err_code;
}

/**@brief 
 *
 * @param[in]   p__environmental_service       Pointer to  environmental Service structure.
 * @param[in]   temperature          				   New temperature value
 */
uint32_t nrf_gpio_three_char_update(dynamic_gpio_service_t *p_dynamic_gpio_service, uint8_t *gpio_value, uint8_t gpio_value_length)
{
    if (p_dynamic_gpio_service == NULL)
    {
        return NRF_ERROR_NULL;
    }

    //uint8_t gpio_three_update[2] = { gpio_value, gpio_configuration}; 

    uint32_t err_code = NRF_SUCCESS;
    ble_gatts_value_t gatts_value;

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = gpio_value_length;//2;//sizeof(uint8_t);
    gatts_value.offset  = 0;
    gatts_value.p_value = gpio_value;//(uint8_t *)gpio_three_update;

    // Update database.
    err_code = sd_ble_gatts_value_set(p_dynamic_gpio_service->conn_handle,
                                      p_dynamic_gpio_service->gpio_three_char_handles.value_handle,//->battery_level_char_handles.value_handle,//battery_level_handles.value_handle,
                                      &gatts_value);
    if (err_code == NRF_SUCCESS)
    {

    }
    else
    {
        return err_code;
    }

    // Send value if connected and notifying.
    if ((p_dynamic_gpio_service->conn_handle != BLE_CONN_HANDLE_INVALID))// && p_bas->is_notification_supported)
    {
        ble_gatts_hvx_params_t hvx_params;

        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = p_dynamic_gpio_service->gpio_three_char_handles.value_handle;//->battery_level_handles.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = gatts_value.offset;
        hvx_params.p_len  = &gatts_value.len;
        hvx_params.p_data = gatts_value.p_value;

        err_code = sd_ble_gatts_hvx(p_dynamic_gpio_service->conn_handle, &hvx_params);
    }
    else
    {
        err_code = NRF_ERROR_INVALID_STATE;
    }
    //}

    return err_code;
}


/**@brief 
 *
 * @param[in]   p__environmental_service       Pointer to  environmental Service structure.
 * @param[in]   temperature          				   New temperature value
 */
uint32_t nrf_gpio_four_char_update(dynamic_gpio_service_t *p_dynamic_gpio_service, uint8_t *gpio_value, uint8_t gpio_value_length)//gpio_configuration_t gpio_configuration)
{
    if (p_dynamic_gpio_service == NULL)
    {
        return NRF_ERROR_NULL;
    }

    //uint8_t gpio_four_update[2] = { gpio_value, gpio_configuration}; 

    uint32_t err_code = NRF_SUCCESS;
    ble_gatts_value_t gatts_value;

    // Initialize value struct.
    memset(&gatts_value, 0, sizeof(gatts_value));

    gatts_value.len     = gpio_value_length;//2;//sizeof(uint8_t);
    gatts_value.offset  = 0;
    gatts_value.p_value = gpio_value;//(uint8_t *)gpio_four_update;

    // Update database.
    err_code = sd_ble_gatts_value_set(p_dynamic_gpio_service->conn_handle,
                                      p_dynamic_gpio_service->gpio_four_char_handles.value_handle,//->battery_level_char_handles.value_handle,//battery_level_handles.value_handle,
                                      &gatts_value);
    if (err_code == NRF_SUCCESS)
    {

    }
    else
    {
        return err_code;
    }

    // Send value if connected and notifying.
    if ((p_dynamic_gpio_service->conn_handle != BLE_CONN_HANDLE_INVALID))// && p_bas->is_notification_supported)
    {
        ble_gatts_hvx_params_t hvx_params;

        memset(&hvx_params, 0, sizeof(hvx_params));

        hvx_params.handle = p_dynamic_gpio_service->gpio_four_char_handles.value_handle;//->battery_level_handles.value_handle;
        hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
        hvx_params.offset = gatts_value.offset;
        hvx_params.p_len  = &gatts_value.len;
        hvx_params.p_data = gatts_value.p_value;

        err_code = sd_ble_gatts_hvx(p_dynamic_gpio_service->conn_handle, &hvx_params);
    }
    else
    {
        err_code = NRF_ERROR_INVALID_STATE;
    }
    //}

    return err_code;
}