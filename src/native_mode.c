/*
 * native_mode.c
 *
 * Created: 29.07.2022 14:05:52
 *  Author: Ulrich Wappler
 *
 * DJTT - MIDI Fighter Twister - Embedded Software License
 * Copyright (c) 2016: DJ Tech Tools
 * Permission is hereby granted, free of charge, to any person owning or possessing
 * a DJ Tech-Tools MIDI Fighter Twister Hardware Device to view and modify this source
 * code for personal use. Person may not publish, distribute, sublicense, or sell
 * the source code (modified or un-modified). Person may not use this source code
 * or any diminutive works for commercial purposes. The permission to use this source
 * code is also subject to the following conditions:
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,  FITNESS FOR A
 * PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */ 

#include "native_mode.h"
#include "encoders.h"

// native mode MIDI channels
static const uint8_t NATIVE_MODE_MIDI_CHANNEL_ENC_POS = 0;
static const uint8_t NATIVE_MODE_MIDI_CHANNEL_ENC_SWITCH = 1;
static const uint8_t NATIVE_MODE_MIDI_CHANNEL_SIDE_SWITCH = 2;

enum // native mode SysEx commands
{
	NATIVE_MODE_SYSEX_COMMAND_SET_NATIVE_MODE = 0x0,
	NATIVE_MODE_SYSEX_COMMAND_ENC_CONFIG = 0x1,
};

enum // native mode SysEx encoder config commands
{
	NATIVE_MODE_SYSEX_COMMAND_ENC_INDICATOR_CONFIG = 0x0,
	NATIVE_MODE_SYSEX_COMMAND_ENC_SWITCH_CONFIG = 0x1,
};

bool native_mode_active = false;

bool native_mode_is_active()
{
	return native_mode_active;
}

//returns true if changed
bool native_mode_set_active(bool active)
{
	if(active == native_mode_is_active())
		return false; //no change
	native_mode_active = active;
	return true;
}

bool is_valid_enc_index(uint8_t value) { return value >= 0 && value < PHYSICAL_ENCODERS; }
bool is_valid_enc_indicator_display_type(uint8_t value) { return value >= 0 && value < NUM_DISPLAY_TYPES; }
bool is_sysex_byte_valid_bool(uint8_t value) { return value == 0 || value == 1; }
bool sysex_byte_to_bool(uint8_t value) { return value != 0; }
bool is_valid_enc_color(uint8_t value) { return value >= 0 && value <= 0x7F; }

typedef struct {
	uint8_t	display_type;
	uint8_t	has_detent;
	uint8_t	detent_color;
} native_mode_enc_indicator_config_t;
native_mode_enc_indicator_config_t native_mode_enc_indicator_configs[PHYSICAL_ENCODERS];
void native_mode_reset_enc_indicator_config(uint8_t dest_index)
{
	static const native_mode_enc_indicator_config_t native_mode_enc_indicator_config_default = {0, 0, 0};
	native_mode_enc_indicator_configs[dest_index] = native_mode_enc_indicator_config_default;
}
bool native_mode_is_enc_indicator_config_valid(const native_mode_enc_indicator_config_t* const config)
{
	return is_valid_enc_indicator_display_type(config->display_type)
	&& is_sysex_byte_valid_bool(config->has_detent)
	&& is_valid_enc_color(config->detent_color);
}

typedef struct {
	uint8_t color_r;
	uint8_t color_g;
	uint8_t color_b;
} native_mode_enc_switch_config_t;
native_mode_enc_switch_config_t native_mode_enc_switch_configs[PHYSICAL_ENCODERS];
void native_mode_reset_enc_switch_config(uint8_t dest_index)
{
	static const native_mode_enc_switch_config_t native_mode_enc_switch_config_default = {0, 0, 0};
	native_mode_enc_switch_configs[dest_index] = native_mode_enc_switch_config_default;
}
bool native_mode_is_enc_switch_config_valid(const native_mode_enc_switch_config_t* const config)
{
	return is_valid_enc_color(config->color_r)
	&& is_valid_enc_color(config->color_g)
	&& is_valid_enc_color(config->color_b);
}

#define NATIVE_MODE_SYSEX_COMMAND_DATA_ENC_INDICATOR_CFG_SIZE 4
typedef union {  // Each of these fields is designed to be written to directly from MIDI SysEx Data
	struct {     // - so you can only use 7-bits of these uint8_t's to store data.
		uint8_t		    index;
		native_mode_enc_indicator_config_t config;
	};
	uint8_t bytes[NATIVE_MODE_SYSEX_COMMAND_DATA_ENC_INDICATOR_CFG_SIZE];
} native_mode_sysex_command_data_enc_indicator_config_t;
bool native_mode_parse_sysex_enc_indicator_config(uint8_t length, uint8_t* buffer)
{	
	if(length < NATIVE_MODE_SYSEX_COMMAND_DATA_ENC_INDICATOR_CFG_SIZE)
		return false;
	
	native_mode_sysex_command_data_enc_indicator_config_t config_data = *(native_mode_sysex_command_data_enc_indicator_config_t*)buffer;
	
	if(!is_valid_enc_index(config_data.index))
		return false;
	if(!native_mode_is_enc_indicator_config_valid(&config_data.config))
		return false;
		
	native_mode_enc_indicator_configs[config_data.index] = config_data.config;
	//native_mode_set_enc_indicator_config(index, enc_indicator_cfg_buffer);
	return true;
}

#define NATIVE_MODE_SYSEX_COMMAND_DATA_ENC_SWITCH_CFG_SIZE 4
typedef union {  // Each of these fields is designed to be written to directly from MIDI SysEx Data
	struct {     // - so you can only use 7-bits of these uint8_t's to store data.
		uint8_t		    index;
		native_mode_enc_switch_config_t config;
	};
	uint8_t bytes[NATIVE_MODE_SYSEX_COMMAND_DATA_ENC_SWITCH_CFG_SIZE];
} native_mode_sysex_command_data_enc_swtich_config_t;
bool native_mode_parse_sysex_enc_switch_config(uint8_t length, uint8_t* buffer)
{
	if(length < NATIVE_MODE_SYSEX_COMMAND_DATA_ENC_SWITCH_CFG_SIZE)
		return false;
	
	native_mode_sysex_command_data_enc_swtich_config_t config_data = *(native_mode_sysex_command_data_enc_swtich_config_t*)buffer;
	
	if(!is_valid_enc_index(config_data.index))
		return false;
	if(!native_mode_is_enc_switch_config_valid(&config_data.config))
		return false;
	
	native_mode_enc_switch_configs[config_data.index] = config_data.config;
	return true;
}

void native_mode_reset_encoder_configs()
{
	for(int i = 0; i < PHYSICAL_ENCODERS; i++) {
		native_mode_reset_enc_indicator_config(i);
		native_mode_reset_enc_switch_config(i);
	}
}

void native_mode_handle_sysex_set_native_mode(uint8_t length, uint8_t* buffer)
{
	if(length != 1)
		return;
	const uint8_t value = *buffer;
	if(!is_sysex_byte_valid_bool(value))
		return;
	if(native_mode_set_active(sysex_byte_to_bool(value)))
	{
		if(native_mode_is_active())
			native_mode_reset_encoder_configs();
		refresh_display();
	}
}

void native_mode_handle_sysex_enc_config(uint8_t length, uint8_t* buffer)
{
	if(!native_mode_is_active())
		return;
	while(length > 0)
	{
		switch(*buffer)
		{
			case NATIVE_MODE_SYSEX_COMMAND_ENC_INDICATOR_CONFIG:
				if(!native_mode_parse_sysex_enc_indicator_config(--length, ++buffer))
					return;
				length -= NATIVE_MODE_SYSEX_COMMAND_DATA_ENC_INDICATOR_CFG_SIZE;
				buffer += NATIVE_MODE_SYSEX_COMMAND_DATA_ENC_INDICATOR_CFG_SIZE;
				break;
			case NATIVE_MODE_SYSEX_COMMAND_ENC_SWITCH_CONFIG:
				if(!native_mode_parse_sysex_enc_switch_config(--length, ++buffer))
					return;
				length -= NATIVE_MODE_SYSEX_COMMAND_DATA_ENC_SWITCH_CFG_SIZE;
				buffer += NATIVE_MODE_SYSEX_COMMAND_DATA_ENC_SWITCH_CFG_SIZE;
				break;
			default:
				return;
		}
	}
	refresh_display();
}

void native_mode_handle_sysex_command(uint8_t length, uint8_t* buffer)
{
	if(length == 0)
		return;
		
	switch(*buffer)
	{
		case NATIVE_MODE_SYSEX_COMMAND_SET_NATIVE_MODE:
			native_mode_handle_sysex_set_native_mode(--length, ++buffer);
			break;
		case NATIVE_MODE_SYSEX_COMMAND_ENC_CONFIG:
			native_mode_handle_sysex_enc_config(--length, ++buffer);
			break;
	}
}

uint8_t native_mode_indicator_value_buffer[PHYSICAL_ENCODERS]; // Holds the 7 bit indicator value (native mode)

bool native_mode_consume_midi_event(uint8_t type, uint8_t channel, uint8_t number, uint8_t value)
{
	if(!native_mode_is_active()
	|| type != SEND_CC
	|| channel != NATIVE_MODE_MIDI_CHANNEL_ENC_POS
	|| number < 0 || number > 0x7F)
	return false;
	
	native_mode_indicator_value_buffer[number] = value;
	return true;
}

#define NATIVE_MODE_BITSHIFT_SYSEX_TO_BYTE 1
bool native_mode_update_encoder_display_single(const uint8_t idx)
{
	if(!native_mode_is_active())
		return false;
	set_encoder_indicator_level(
		idx, native_mode_indicator_value_buffer[idx],
		native_mode_enc_indicator_configs[idx].has_detent,
		native_mode_enc_indicator_configs[idx].display_type,
		native_mode_enc_indicator_configs[idx].detent_color,
		0x7F);
	uint32_t rgb_color =
	((uint32_t)native_mode_enc_switch_configs[idx].color_r << (16 + NATIVE_MODE_BITSHIFT_SYSEX_TO_BYTE))
	| ((uint32_t)native_mode_enc_switch_configs[idx].color_g << (8 + NATIVE_MODE_BITSHIFT_SYSEX_TO_BYTE))
	| ((uint32_t)native_mode_enc_switch_configs[idx].color_b << (0 + NATIVE_MODE_BITSHIFT_SYSEX_TO_BYTE));
	build_rgb(idx, rgb_color, false);
	return true;
}

bool native_mode_process_encoder_input_rotary(const uint8_t idx, const int16_t delta)
{
	if(!native_mode_is_active())
		return false;
	midi_stream_raw_cc(NATIVE_MODE_MIDI_CHANNEL_ENC_POS, idx, 64 + delta);
	return true;
}

bool native_mode_process_encoder_input_switch_pressed(const uint8_t idx, const bool pressed)
{
	if(!native_mode_is_active())
		return false;
	midi_stream_raw_cc(NATIVE_MODE_MIDI_CHANNEL_ENC_SWITCH, idx, pressed ? 0x7F : 0x0);
	return true;
}

bool native_mode_process_side_switch_pressed(uint8_t idx, bool pressed)
{
	if(!native_mode_is_active())
		return false;
	midi_stream_raw_cc(NATIVE_MODE_MIDI_CHANNEL_SIDE_SWITCH, idx, pressed ? 127 : 0);
	return true;
}
