/*
Example for AudioMUX Click

    Date          : Oct 2018.
    Author        : Nemanja Medakovic

Test configuration STM32 :
    
    MCU              : STM32F107VCT6
    Dev. Board       : EasyMx PRO v7 for STM32
    ARM Compiler ver : v6.0.0.0

---

Description :

The application is composed of three sections :

- System Initialization - Initializes peripherals and pins.
- Application Initialization - Initializes I2C interface.
- Application Task - (code snippet) - Initializes AudioMUX device to work with the desired configurations and
  shows a message on uart when playing is started.
  Device initialization will be performed only once and after that AudioMUX will work with the same desired configurations.

*/

#include "Click_AudioMUX_types.h"
#include "Click_AudioMUX_config.h"

uint8_t initCheck;
uint8_t messCnt;

void systemInit()
{
    mikrobus_i2cInit( _MIKROBUS1, &_AUDIOMUX_I2C_CFG[0] );

    mikrobus_logInit( _LOG_USBUART_A, 9600 );
    mikrobus_logWrite( "*** Initializing... ***", _LOG_LINE );

    Delay_ms( 100 );
}

void applicationInit()
{
    audiomux_i2cDriverInit( (T_AUDIOMUX_P)&_MIKROBUS1_GPIO, (T_AUDIOMUX_P)&_MIKROBUS1_I2C, _AUDIOMUX_DEVICE_ADDR );
    Delay_ms( 500 );

    initCheck = 0;
    messCnt   = 0;

    mikrobus_logWrite( "** AudioMUX is initialized **", _LOG_LINE );
    mikrobus_logWrite( "", _LOG_LINE );
}

void applicationTask()
{
    if (initCheck == 0)
    {
        audiomux_selectInput( _AUDIOMUX_IN1_EN, _AUDIOMUX_MUTE_INPUT_OFF, _AUDIOMUX_MIC_GAIN_14DB, _AUDIOMUX_MIC_OFF );
        audiomux_setInputGain( _AUDIOMUX_IN_GAIN_2DB );
        audiomux_writeSingleByte( _AUDIOMUX_SURROUND_ADDR, _AUDIOMUX_MIX_0 );
        audiomux_setVolume( _AUDIOMUX_VOL1_6DB_NEG, _AUDIOMUX_VOL2_0DB, _AUDIOMUX_VOLUME_LEFT_ADDR );
        audiomux_setVolume( _AUDIOMUX_VOL1_6DB_NEG, _AUDIOMUX_VOL2_0DB, _AUDIOMUX_VOLUME_RIGHT_ADDR );
        audiomux_setTrebleBass( _AUDIOMUX_TRE_BASS_4DB_NEG, _AUDIOMUX_TRE_BASS_14DB );
        audiomux_writeSingleByte( _AUDIOMUX_BASS_ALC_ADDR, _AUDIOMUX_MODE2_ADAPTIVE );
        audiomux_muteOuput( _AUDIOMUX_MUTE_OUTPUT_OFF );
        initCheck = 1;

        mikrobus_logWrite( "Playing", _LOG_TEXT );
        Delay_ms( 1000 );
    }

    if (messCnt < 5)
    {
        mikrobus_logWrite( ". ", _LOG_TEXT );
        Delay_ms( 4000 );
        messCnt++;
    }
}

void main()
{
    systemInit();
    applicationInit();

    while (1)
    {
        applicationTask();
    }
}