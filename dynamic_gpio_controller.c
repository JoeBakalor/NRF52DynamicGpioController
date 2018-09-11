
#include "nrf_drv_twi.h"
#include "nrf_drv_saadc.h"
#include "nrf_drv_timer.h"
#include "nrf_drv_ppi.h"
#include "nordic_common.h"
#include "app_error.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "dynamic_gpio_controller.h"
#include "dynamic_gpio_service.h"
#include "gpio_defines.h"

#define SAMPLES_IN_BUFFER 12
volatile uint8_t state = 1;

extern dynamic_gpio_service_t dynamic_gpio_service;

static const nrf_drv_timer_t m_timer = NRF_DRV_TIMER_INSTANCE(1);
static nrf_saadc_value_t     m_buffer_pool[2][SAMPLES_IN_BUFFER];
static nrf_ppi_channel_t     m_ppi_channel;
static uint32_t              m_adc_evt_counter;

static uint8_t channel_zero_enabled     = 0;
static uint8_t channel_one_enabled      = 0;
static uint8_t channel_three_enabled    = 0;
static uint8_t channel_five_enabled     = 0;

static uint8_t initialized = 0;

void timer_handler(nrf_timer_event_t event_type, void * p_context)
{

}

void saadc_sampling_event_init(void)
{
    ret_code_t err_code;

    err_code = nrf_drv_ppi_init();
    APP_ERROR_CHECK(err_code);

    nrf_drv_timer_config_t timer_cfg = NRF_DRV_TIMER_DEFAULT_CONFIG;
    timer_cfg.bit_width = NRF_TIMER_BIT_WIDTH_32;

    err_code = nrf_drv_timer_init(&m_timer, &timer_cfg, timer_handler);
    NRF_LOG_INFO("Timer init error : %d", err_code);
    APP_ERROR_CHECK(err_code);

    /* setup m_timer for compare event every 400ms */
    uint32_t ticks = nrf_drv_timer_ms_to_ticks(&m_timer, 400);
    nrf_drv_timer_extended_compare(&m_timer,
                                   NRF_TIMER_CC_CHANNEL0,
                                   ticks,
                                   NRF_TIMER_SHORT_COMPARE0_CLEAR_MASK,
                                   false);
    nrf_drv_timer_enable(&m_timer);

    uint32_t timer_compare_event_addr = nrf_drv_timer_compare_event_address_get(&m_timer,
                                                                                NRF_TIMER_CC_CHANNEL0);
    uint32_t saadc_sample_task_addr   = nrf_drv_saadc_sample_task_get();

    /* setup ppi channel so that timer compare event is triggering sample task in SAADC */

    err_code = nrf_drv_ppi_channel_alloc(&m_ppi_channel);
    APP_ERROR_CHECK(err_code);

    err_code = nrf_drv_ppi_channel_assign(m_ppi_channel,
                                          timer_compare_event_addr,
                                          saadc_sample_task_addr);
    APP_ERROR_CHECK(err_code);
}

void saadc_sampling_event_enable(void)
{
    ret_code_t err_code = nrf_drv_ppi_channel_enable(m_ppi_channel);

    APP_ERROR_CHECK(err_code);
}

/*
CHANNEL 0 = GPIO1
CHANNEL 1 = GPIO4
CHANNEL 3 = GPIO3
CHNNEL  5 = GPIO2
*/

void saadc_callback(nrf_drv_saadc_evt_t const * p_event)
{
    if (p_event->type == NRF_DRV_SAADC_EVT_DONE)
    {
        ret_code_t err_code;

        uint8_t update_value[2];

        err_code = nrf_drv_saadc_buffer_convert(p_event->data.done.p_buffer, SAMPLES_IN_BUFFER);
        APP_ERROR_CHECK(err_code);

        int i;
        NRF_LOG_INFO("ADC event number: %d", (int)m_adc_evt_counter);

        //IF CHANNEL X ACTIVE
        //BUFFER = [CHANNEL X, CHANNEL X, CHANNEL X, CHANNEL X, CHANNEL X, CHANNEL X, CHANNEL X, CHANNEL X, CHANNEL X, CHANNEL X, CHANNEL X, CHANNEL X ]

        //IF CHANNEL X AND CHANNEL Y ACTIVE
        //BUFFER = [CHANNEL X, CHANNEL Y, CHANNEL X, CHANNEL Y, CHANNEL X, CHANNEL Y, CHANNEL X, CHANNEL Y, CHANNEL X, CHANNEL Y, CHANNEL X, CHANNEL Y ]

        //IF CHANNEL X, CHANNEL Y, CHANNEL Z ACTIVE
        //BUFFER = [CHANNEL X, CHANNEL Y, CHANNEL Z, CHANNEL X, CHANNEL Y, CHANNEL Z, CHANNEL X, CHANNEL Y, CHANNEL Z, CHANNEL X, CHANNEL Y, CHANNEL Z ]

        //IF CHANNEL X, CHANNEL Y, CHANNEL Z, AND CHANNEL W ACTIVE
        //BUFFER = [CHANNEL X, CHANNEL Y, CHANNEL Z, CHANNEL W, CHANNEL X, CHANNEL Y, CHANNEL Z, CHANNEL W, CHANNEL X, CHANNEL Y, CHANNEL Z, CHANNEL W ]
        
        if (1 == channel_zero_enabled )
        {
          if (1 == channel_one_enabled )
          {
            if (1 == channel_three_enabled)
            {
              if (1 == channel_five_enabled)
              {
                 /* CHANNEL 0, CHANNEL 1, CHANNEL 3, AND CHANNEL 5 ENABLED*/
                for (i = 0; i < SAMPLES_IN_BUFFER; i++)
                {
                    NRF_LOG_INFO("CHANNEL 0, GPIO1: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_one_char_update(&dynamic_gpio_service, update_value, 2);
                    i++;

                    NRF_LOG_INFO("CHANNEL 1, GPIO3: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_three_char_update(&dynamic_gpio_service, update_value, 2);
                    i++;

                    NRF_LOG_INFO("CHANNEL 3, GPIO4: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_four_char_update(&dynamic_gpio_service, update_value, 2);
                    i++;
                    
                    NRF_LOG_INFO("CHANNEL 5, GPIO2: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_two_char_update(&dynamic_gpio_service, update_value, 2);
                }
              }
            }
            else if (1 == channel_five_enabled)
            {
              /* CHANNEL 0, CHANNEL 1, AND CHANNEL 5 ENABLED*/
                for (i = 0; i < SAMPLES_IN_BUFFER; i++)
                {
                    NRF_LOG_INFO("CHANNEL 0, GPIO1: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_one_char_update(&dynamic_gpio_service, update_value, 2);
                    i++;

                    NRF_LOG_INFO("CHANNEL 1, GPIO3: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_three_char_update(&dynamic_gpio_service, update_value, 2);
                    i++;

                    NRF_LOG_INFO("CHANNEL 5, GPIO2: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_two_char_update(&dynamic_gpio_service, update_value, 2);
                }
            }
            else 
            {
              /*  CHANNEL 0 AND CHANNEL 1 ENABLED*/
                    NRF_LOG_INFO("CHANNEL 0, GPIO1: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_one_char_update(&dynamic_gpio_service, update_value, 2);
                    i++;

                    NRF_LOG_INFO("CHANNEL 1, GPIO3: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_three_char_update(&dynamic_gpio_service, update_value, 2);
            }

          }
          else if (1 == channel_three_enabled)
          {
            if (1 == channel_five_enabled)
            {
               /* CHANNEL 0, CHANNEL 3, AND CHANNEL 5 ENABLED */
                for (i = 0; i < SAMPLES_IN_BUFFER; i++)
                {
                    NRF_LOG_INFO("CHANNEL 0, GPIO1: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_one_char_update(&dynamic_gpio_service, update_value, 2);
                    i++;

                    NRF_LOG_INFO("CHANNEL 3, GPIO4: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_four_char_update(&dynamic_gpio_service, update_value, 2);
                    i++;

                    NRF_LOG_INFO("CHANNEL 5, GPIO2: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_two_char_update(&dynamic_gpio_service, update_value, 2);
                }
            }
            else
            {
                /* CHANNEL 0 AND CHANNEL 3 ENABLED */
                for (i = 0; i < SAMPLES_IN_BUFFER; i++)
                {
                    NRF_LOG_INFO("CHANNEL 0, GPIO1: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_one_char_update(&dynamic_gpio_service, update_value, 2);
                    i++;

                    NRF_LOG_INFO("CHANNEL 3, GPIO4: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_four_char_update(&dynamic_gpio_service, update_value, 2);
                }
            }
          }
          else if (1 == channel_five_enabled)
          {
              /* CHANNEL 0 AND CHANNEL 5 ENABLED */
                for (i = 0; i < SAMPLES_IN_BUFFER; i++)
                {
                    NRF_LOG_INFO("CHANNEL 0, GPIO1: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_one_char_update(&dynamic_gpio_service, update_value, 2);
                    i++;

                    NRF_LOG_INFO("CHANNEL 5, GIO2: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_two_char_update(&dynamic_gpio_service, update_value, 2);
                }
          }
          else 
          {
              /*  ONLY CHANNEL 0 IS ENABLED */
                for (i = 0; i < SAMPLES_IN_BUFFER; i++)
                {
                    NRF_LOG_INFO("CHANNEL 0, GPIO1: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_one_char_update(&dynamic_gpio_service, update_value, 2);
                }
          }
        } 
        else if (1 == channel_one_enabled )
        {
          if (1 == channel_three_enabled)
          {
            if (1 == channel_five_enabled)
            {
              /*  CHANNEL 1, CHANNEL 3 AND CHANNEL 5 ARE ENABLED */
                for (i = 0; i < SAMPLES_IN_BUFFER; i++)
                {
                    NRF_LOG_INFO("CHANNEL 1, GPIO3: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_three_char_update(&dynamic_gpio_service, update_value, 2);
                    i++;

                    NRF_LOG_INFO("CHANNEL 3, GPIO4: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_four_char_update(&dynamic_gpio_service, update_value, 2);
                    i++;

                    NRF_LOG_INFO("CHANNEL 5, GPIO2: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_two_char_update(&dynamic_gpio_service, update_value, 2);
                }
            } 
            else 
            {
              /* CHANNEL 1 AND CHANNEL 3 ARE ENABLED */
                for (i = 0; i < SAMPLES_IN_BUFFER; i++)
                {
                    NRF_LOG_INFO("CHANNEL 1, GPIO3: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_three_char_update(&dynamic_gpio_service, update_value, 2);
                    i++;

                    NRF_LOG_INFO("CHANNEL 3, GPIO4: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_four_char_update(&dynamic_gpio_service, update_value, 2);
                }
            }
          }
          else if (1 == channel_five_enabled )
          {
              /*  CHANNEL 1 AND CHANNEL 5 ARE ENABLED */
                for (i = 0; i < SAMPLES_IN_BUFFER; i++)
                {
                    NRF_LOG_INFO("CHANNEL 1, GPIO3: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_three_char_update(&dynamic_gpio_service, update_value, 2);
                    i++;

                    NRF_LOG_INFO("CHANNEL 5, GPIO2: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_two_char_update(&dynamic_gpio_service, update_value, 2);
                }
          }
          else 
          {
              /*  ONLY CHANNEL 1 IS ENABLED */
                for (i = 0; i < SAMPLES_IN_BUFFER; i++)
                {
                    NRF_LOG_INFO("CHANNEL 1, GPIO3: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_three_char_update(&dynamic_gpio_service, update_value, 2);
                }
          }
        }
        else if (1 == channel_three_enabled)
        {
          if (1 == channel_five_enabled)
          {
              /*  CHANNEL 3 AND CHANNEL 5 ARE ENABLED*/
                for (i = 0; i < SAMPLES_IN_BUFFER; i++)
                {
                    NRF_LOG_INFO("CHANNEL 3, GPIO4: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_four_char_update(&dynamic_gpio_service, update_value, 2);
                    i++;

                    NRF_LOG_INFO("CHANNEL 5, GPIO2: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_two_char_update(&dynamic_gpio_service, update_value, 2);
                }
          }
          else 
          {
              /*  ONLY CHANNEL 3 IS ENABLED */
                for (i = 0; i < SAMPLES_IN_BUFFER; i++)
                {
                    NRF_LOG_INFO("CHANNEL 3, GPIO4: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_four_char_update(&dynamic_gpio_service, update_value, 2);
                }
          }
        }
        else if (1 == channel_five_enabled)
        {
          /*  ONLY CHANNEL 5 IS ENABLED, ALL DATA IS FROM CHANNEL 5*/
                for (i = 0; i < SAMPLES_IN_BUFFER; i++)
                {
                    NRF_LOG_INFO("CHANNEL 5, GPIO2: %d", p_event->data.done.p_buffer[i]);
                    update_value[0] = (p_event->data.done.p_buffer[i] & 0xff00) >> 8;
                    update_value[1] = (p_event->data.done.p_buffer[i] & 0x00ff);
                    nrf_gpio_two_char_update(&nrf_gpio_service, update_value, 2);
                }
        }

//        for (i = 0; i < SAMPLES_IN_BUFFER; i++)
//        {
//            NRF_LOG_INFO("%d", p_event->data.done.p_buffer[i]);
//        }
        m_adc_evt_counter++;
    }
}

void saadc_init_channel(void)
{
    ret_code_t err_code;
    uint8_t samples = channel_zero_enabled + channel_one_enabled + channel_three_enabled + channel_five_enabled;

    nrf_saadc_channel_config_t channel_config_zero =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN0);

    nrf_saadc_channel_config_t channel_config_one =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN1);

    nrf_saadc_channel_config_t channel_config_three =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN3);
        
    nrf_saadc_channel_config_t channel_config_five =
        NRF_DRV_SAADC_DEFAULT_CHANNEL_CONFIG_SE(NRF_SAADC_INPUT_AIN5);

    err_code = nrf_drv_saadc_init(NULL, saadc_callback);
    APP_ERROR_CHECK(err_code);

    if(1 == channel_zero_enabled)
    {
      err_code = nrf_drv_saadc_channel_init(0, &channel_config_zero);
      APP_ERROR_CHECK(err_code);
    }

    if(1 == channel_one_enabled)
    {
      err_code = nrf_drv_saadc_channel_init(1, &channel_config_one);
      APP_ERROR_CHECK(err_code);
    }

    if(1 == channel_three_enabled)
    {
      err_code = nrf_drv_saadc_channel_init(3, &channel_config_three);
      APP_ERROR_CHECK(err_code);
    }

    if (1 == channel_five_enabled)
    {
      err_code = nrf_drv_saadc_channel_init(5, &channel_config_five);
      APP_ERROR_CHECK(err_code);
    }

    if (channel_five_enabled | channel_zero_enabled | channel_one_enabled | channel_three_enabled)
    {
      err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[0], SAMPLES_IN_BUFFER);
      APP_ERROR_CHECK(err_code);

      err_code = nrf_drv_saadc_buffer_convert(m_buffer_pool[1], SAMPLES_IN_BUFFER);
      APP_ERROR_CHECK(err_code);
    }
}

void enable_channel(uint8_t channel){

  nrfx_saadc_uninit();

  if (0 == channel)
  {
    channel_zero_enabled = 1;
  } 
  else if (1 == channel)
  {
    channel_one_enabled = 1;
  }
  else if (3 == channel)
  {
    channel_three_enabled = 1;
  }
  else if (5 == channel)
  {
    channel_five_enabled = 1;
  }

  saadc_init_channel();
}

void disable_channel(uint8_t channel){

  nrfx_saadc_uninit();

  if (0 == channel)
  {
    channel_zero_enabled = 0;
  } 
  else if (1 == channel)
  {
    channel_one_enabled = 0;
  }
  else if (3 == channel)
  {
    channel_three_enabled = 0;
  }
  else if (5 == channel)
  {
    channel_five_enabled = 0;
  }

  saadc_init_channel();
}


void set_gpio_as_output(uint8_t GPIO){

    if (1 == GPIO)//AIN0 = 2 = GPIO1
    {
      if (0 == channel_zero_enabled)
      {
        /* ITS ALREADY SET AN OUTPUT */
      }
      else
      {
        channel_zero_enabled = 0;
        disable_channel(0);
        nrf_gpio_cfg_output(DGPIO1);
        nrf_gpio_pin_write(DGPIO1, 1);
        nrf_gpio_pin_write(DGPIO1, 0);
      }
    } 
    else if (4 == GPIO) //AIN3 = 5 = GPIO4  
    { 
      if (0 == channel_three_enabled)
      {
        /* ITS ALREADY SET AN OUTPUT */
      }
      else
      {
        channel_three_enabled = 0;
        disable_channel(3);
        nrf_gpio_cfg_output(DGPIO4);
        nrf_gpio_pin_write(DGPIO4, 1);
        nrf_gpio_pin_write(DGPIO4, 0);

      }
    } 
    else if (3 == GPIO) //AIN1 = 3 = GPIO3
    { 
      if (0 == channel_one_enabled)
      {
        /* ITS ALREADY SET AN OUTPUT */
      }
      else
      {
        channel_one_enabled = 0;
        disable_channel(1);
        nrf_gpio_cfg_output(DGPIO3);
        nrf_gpio_pin_write(DGPIO3, 1);
        nrf_gpio_pin_write(DGPIO3, 0);
      }
    } 
    else if (2 == GPIO) //AIN5 == 29 = GPIO2
    { 
      if (0 == channel_five_enabled)
      {
        /*  ITS ALREADY SET AN OUTPUT */
      }
      else
      {
        channel_five_enabled = 0;
        disable_channel(5);
        nrf_gpio_cfg_output(DGPIO2);
        nrf_gpio_pin_write(DGPIO2, 1);
        nrf_gpio_pin_write(DGPIO2, 0);
      }
    }
}

void set_gpio_as_input(uint8_t GPIO){

    /*  have to double check the GPIO to channel mappings, these are wrong */
    if (1 == GPIO)//AIN0 = 2 = GPIO1
    {
      if (0 == channel_zero_enabled)
      {
        channel_zero_enabled = 1;
        nrf_gpio_cfg_input(DGPIO1, NRF_GPIO_PIN_PULLDOWN);
        enable_channel(0);
      }
      else
      {
         /* ITS ALREADY ENABLED AS IN INPUT */
      }
    } 
    else if (4 == GPIO) //AIN3 = 5 = GPIO4 
    { 
      if (0 == channel_three_enabled)
      {
        channel_three_enabled = 1;
        nrf_gpio_cfg_input(DGPIO4, NRF_GPIO_PIN_PULLDOWN);
        enable_channel(3);
      }
      else
      {
        /* ITS ALREADY ENABLED AS IN INPUT */
      }
    } 
    else if (3 == GPIO) //AIN1 = 3 = GPIO3
    { 
      if (0 == channel_one_enabled)
      {
        channel_one_enabled = 1;
        nrf_gpio_cfg_input(DGPIO3, NRF_GPIO_PIN_PULLDOWN);
        enable_channel(1);
      }
      else
      {
        /* ITS ALREADY ENABLED AS IN INPUT */
      }
    } 
    else if (2 == GPIO) //AIN5 == 29 = GPIO2
    { 
      if (0 == channel_five_enabled)
      {
        channel_five_enabled = 1;
        nrf_gpio_cfg_input(DGPIO2, NRF_GPIO_PIN_PULLDOWN);
        enable_channel(5);
      }
      else
      {
        /* ITS ALREADY ENABLED AS IN INPUT */
      }
    }
}



