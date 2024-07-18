#include "app.h"
#include "dc_motor.h"
#include "feeding_system.h"
#include "hal_time.h"
#include "pins.h"
#include "sensor.h"
#include "ti_msp_dl_config.h"
#include "uart.h"

HAL::UART _printer_UART;

// UART also for logger
// template<HAL::UART UART, UART_Regs* UART_INSTANCE>
void UartInteruptHandler()
{
    auto pendingInterupt = DL_UART_Main_getPendingInterrupt(UART_PRINTER_INST);

    if (pendingInterupt == DL_UART_MAIN_IIDX_RX) {
        _printer_UART.on_rx_interrupt(DL_UART_Main_receiveData(UART_PRINTER_INST));
    }

    if (pendingInterupt == DL_UART_MAIN_IIDX_TX) {
        DL_UART_Main_transmitData(UART_PRINTER_INST, _printer_UART.on_tx_interrupt());
    }
}

void SysTick_Handler()
{
    HAL::current_millis++;
}

int main()
{
    SYSCFG_DL_init();

    DL_Timer_startCounter(PWM_INST);
    NVIC_ClearPendingIRQ(UART_PRINTER_INST_INT_IRQN);
    NVIC_EnableIRQ(UART_PRINTER_INST_INT_IRQN);

    HAL::pwm_pin aPwmPin(PWM_INST, GPIO_PWM_C0_IDX);
    HAL::pwm_pin bPwmPin(PWM_INST, GPIO_PWM_C1_IDX);

    HAL::gpio_pin _stby_pin(MOTORS_STBY_PORT, MOTORS_STBY_PIN);
    HAL::gpio_pin aIN1Pin(MOTORS_AIN1_PORT, MOTORS_AIN1_PIN);
    HAL::gpio_pin aIN2Pin(MOTORS_AIN2_PORT, MOTORS_AIN2_PIN);
    HAL::gpio_pin bIN1Pin(MOTORS_BIN1_PORT, MOTORS_BIN1_PIN);
    HAL::gpio_pin bIN2Pin(MOTORS_BIN2_PORT, MOTORS_BIN2_PIN);

    HAL::adc_pin hubFSensorPin(ADC_INST, ADC_ADCMEM_0);
    HAL::adc_pin aFeedingSystemFSensorPin(ADC_INST, ADC_ADCMEM_1);
    HAL::adc_pin aMotorSensorPin(ADC_INST, ADC_ADCMEM_2);
    HAL::adc_pin bFeedingSystemFSensorPin(ADC_INST, ADC_ADCMEM_3);
    HAL::adc_pin bMotorSensorPin(ADC_INST, ADC_ADCMEM_4);

    modules::dc_motor aMotor(aPwmPin, _stby_pin, aIN1Pin, aIN2Pin);
    modules::dc_motor bMotor(bPwmPin, _stby_pin, bIN1Pin, bIN2Pin);

    modules::hub_filament_sensor hubFSensor(hubFSensorPin);
    modules::feeding_system_filament_sensor aFeedingSystemFSensor(aFeedingSystemFSensorPin);
    modules::MotorHalSensor aMotorSensor(aMotorSensorPin);

    modules::feeding_system_filament_sensor bFeedingSystemFSensor(bFeedingSystemFSensorPin);
    modules::MotorHalSensor bMotorSensor(bMotorSensorPin);

    modules::feeding_system afeeding_system(aMotor, aFeedingSystemFSensor, aMotorSensor);
    modules::feeding_system bfeeding_system(bMotor, bFeedingSystemFSensor, bMotorSensor);

    modules::feeding_system _feeding_systems[config::TOOL_COUNT] = { afeeding_system, bfeeding_system };

    modules::registers _registers;

    app a(_feeding_systems, hubFSensor, _printer_UART, _registers);

    while (1) {
        a.step();
    }
}
