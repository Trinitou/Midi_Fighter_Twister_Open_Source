/*
 * native_mode.h
 *
 * Created: 29.07.2022 14:04:44
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

#ifndef NATIVE_MODE_H_
#define NATIVE_MODE_H_

#include <asf.h>

bool native_mode_is_active();

void native_mode_handle_sysex_command(uint8_t length, uint8_t* buffer);
//returns true if the MIDI event was consumed by native mode
bool native_mode_consume_midi_event(uint8_t type, uint8_t channel, uint8_t number, uint8_t value);

//returns true if the encoder display was updated for native mode
bool native_mode_update_encoder_display_single(uint8_t idx);

//returns true if encoder rotary input was processed by native mode
bool native_mode_process_encoder_input_rotary(uint8_t idx, int16_t delta);
//returns true if encoder switch input was processed by native mode
bool native_mode_process_encoder_input_switch_pressed(uint8_t idx, bool pressed);
//returns true if side switch input was processed by native mode
bool native_mode_process_side_switch_pressed(uint8_t idx, bool pressed);

#endif /* NATIVE_MODE_H_ */
