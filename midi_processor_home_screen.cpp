/**
 * @file mc_seven_seg_display.cpp
 * @brief This class implements the Assignment 2-digit seven segment display
 * and the Timecode/BBT 10-digit seven segment display. Per digit decimal
 * point display is not supported
 * 
 * Copyright (c) 2022 rppicomidi
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 */
#include <cstring>
#include <cstdio>
#include "midi_processor_home_screen.h"
//#include "backup_view.h"
//#include "restore_view.h"

rppicomidi::Home_screen::Home_screen(Mono_graphics& screen_, const char* device_label_) :
    View{screen_, screen_.get_clip_rect()},
    label_font{screen.get_font_12()},
    menu{screen, static_cast<uint8_t>(label_font.height*2+4), label_font},
    num_in_cables{0}, num_out_cables{0},
    //is_preset_backup_mode{false},
    settings_flash_view{screen}
{
    set_connected_device(device_label_, num_in_cables, num_out_cables);
    auto item = new View_launch_menu_item(settings_flash_view, "Presets memory...", screen, label_font);
    assert(item);
    menu.add_menu_item(item);
}


void rppicomidi::Home_screen::draw()
{
    screen.clear_canvas();
    screen.center_string_on_two_lines(label_font, device_label, 0);
    if (/*!is_preset_backup_mode && */ (num_in_cables !=0 || num_out_cables != 0)) {
        char preset_text[max_line_length];
        sprintf(preset_text, "Preset:%1x%s",Midi_processor_manager::instance().get_current_preset(), Midi_processor_manager::instance().needs_store()?"[M]":"");
        menu.set_menu_item_text(0,preset_text);
    }
    menu.draw();
}

rppicomidi::Home_screen::Select_result rppicomidi::Home_screen::on_select(View** new_view)
{
    return menu.on_select(new_view);
}

void rppicomidi::Home_screen::set_connected_device(const char* device_label_, uint8_t num_in_cables_, uint8_t num_out_cables_, bool is_active_)
{
    // save the product string
    strncpy(device_label, device_label_, max_device_label);
    device_label[max_device_label] = '\0';
    num_in_cables = num_in_cables_;
    num_out_cables = num_out_cables_;
    for (uint8_t cable=0; cable < num_in_cables; cable++)
        midi_in_setup.push_back(new Midi_processor_setup_screen{screen, label_font, cable, true});
    for (uint8_t cable=0; cable < num_out_cables; cable++)
        midi_out_setup.push_back(new Midi_processor_setup_screen{screen, label_font, cable, false});
    printf("New connection %s %u IN %u OUT\r\n", device_label, num_in_cables, num_out_cables);
    //is_preset_backup_mode = false;
    menu.clear();
    if (num_in_cables !=0 || num_out_cables !=0) {
        preset_view = new Preset_view(screen, screen.get_clip_rect());
        auto preset_item = new View_launch_menu_item(*preset_view, "Presets...", screen, label_font);
        menu.add_menu_item(preset_item);
        for (int port=0; port<num_in_cables; port++) {
            char line[max_line_length+1];
            sprintf(line,"Setup MIDI IN %u...", port+1);
            Menu_item* item = new View_launch_menu_item(*midi_in_setup.at(port),line, screen, label_font);
            menu.add_menu_item(item);
        }
        for (int port=0; port<num_out_cables; port++) {
            char line[max_line_length+1];
            sprintf(line,"Setup MIDI OUT %u...", port+1);
            Menu_item* item = new View_launch_menu_item(*midi_out_setup.at(port),line, screen, label_font);
            menu.add_menu_item(item);
        }
        if (is_active_) {
            // Treat the menu as a sub-view of this view. Do not change view.
            menu.entry();
            draw();
        }
    }
}

#if defined(CFG_TUH_MSC)
void rppicomidi::Home_screen::enter_preset_backup_mode(View& timeset_view)
{
    menu.clear();
    is_preset_backup_mode = true;
    strncpy(device_label, "Save/Restore Presets to Flash Drive", max_device_label);
    device_label[max_device_label] = '\0';
    Menu_item* item = new View_launch_menu_item(timeset_view, "Set Date/Time...", screen, label_font);
    assert(item);
    menu.add_menu_item(item);
    auto backup = new Backup_view(screen);
    item = new View_launch_menu_item(*backup,"Backup...",screen, label_font);
    assert(item);
    menu.add_menu_item(item);
    auto restore = new Restore_view(screen);
    item = new View_launch_menu_item(*restore,"Restore...",screen, label_font);
    assert(item);
    menu.add_menu_item(item);
    item = new View_launch_menu_item(settings_flash_view,"Presets memory...",screen, label_font);
    assert(item);
    menu.add_menu_item(item);
    menu.entry();
}
#endif