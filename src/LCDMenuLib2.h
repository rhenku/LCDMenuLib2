/* ******************************************************************************                                                                        
 *                        LCDMenuLib2 (LCDML)                                                                                                       
 * ****************************************************************************** 
 *
 * MIT License
 * 
 * Copyright (c) [2017] [Nils Feldkämper]
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
 *                                      
 * ******************************************************************************
 *   
 * BUG / ISSUES REPORTING                                      
 *    https://github.com/Jomelo/LCDMenuLib2/issues 
 * 
 * ARDUIONO FORUM                                                    
 *     http://forum.arduino.cc/index.php?topic=73816.0  
 *   
 * ****************************************************************************** 
 */

// File header
#ifndef LCDMenuLib2_h
    #define LCDMenuLib2_h
   
    // You can change this parameters 
    #define _LCDML_DISP_cfg_cursor_deep          10  // save the last position of the cursor until layer xx
    #define _LCDML_DISP_cfg_max_rows             10  // max rows which are supported
    #define _LCDML_cfg_use_ram                   0   // set this to 1 when you will use the ram mode
      
      
      
      
    // ####################### // 
    // Do nothing change here
    // ####################### // 
    // Version
    #define _LCDML_VERSION                       "LCDML2 v1.0.1"
        
    // Include arduino ios 
    #include "Arduino.h"

    // Arduino specific settings
    #if ARDUINO >= 160
        #define _LCDMenuLib2_arduino_version         1    // for new arduino version like 1.6.0 or higher
    #else
        #define _LCDMenuLib2_arduino_version         0    // for old arduino version like 1.0.6 or 1.0.5
    #endif

    // ESP specific settings
    #if ( ESP8266 ) || defined ( ESP32 ) || (_LCDML_cfg_use_ram == 1)
        #define _LCDML_ESP
    #endif
    
    // No function constante 
    #define _LCDML_NO_FUNC                      255
    
    // Bit pos control
    #define _LCDML_control_menu_back            7
    #define _LCDML_control_disp_update          6
    #define _LCDML_control_cursor_update        5
    #define _LCDML_control_setup                4
    #define _LCDML_control_update_direct        3
    #define _LCDML_control_search_display       2
    #define _LCDML_control_rollover             1
    #define _LCDML_control_disable_hidden       0
    
    #define _LCDML_funcReg_free7                7
    #define _LCDML_funcReg_free6                6
    #define _LCDML_funcReg_free5                5
    #define _LCDML_funcReg_free4                4
    #define _LCDML_funcReg_free3                3
    #define _LCDML_funcReg_disable_screensaver  2
    #define _LCDML_funcReg_setup                1
    #define _LCDML_funcReg_end                  0

    // Configure arduino flash lib and load it*/
    #ifndef __PROG_TYPES_COMPAT__
        #define __PROG_TYPES_COMPAT__
    #endif

    // Include PGMSPACE
    #ifndef _LCDML_ESP
        #include <avr/pgmspace.h>
    #endif     

    // Include menu class
    #include "LCDMenuLib2_typedef.h"
    #include "LCDMenuLib2_menu.h"
    
    #define _LCDML_button_free                  7
    #define _LCDML_button                       6
    #define _LCDML_button_quit                  5
    #define _LCDML_button_enter                 4
    #define _LCDML_button_up                    3
    #define _LCDML_button_down                  2
    #define _LCDML_button_left                  1
    #define _LCDML_button_right                 0
    
    #define _LCDML_TYPE_f7                      128
    #define _LCDML_TYPE_f6                      64
    #define _LCDML_TYPE_f5                      32
    #define _LCDML_TYPE_f4                      16
    #define _LCDML_TYPE_f3                      8
    #define _LCDML_TYPE_f2                      4
    #define _LCDML_TYPE_dynParam                2
    #define _LCDML_TYPE_default                 1

    // Include macros for this lib
    #include "LCDMenuLib2_macros.h"      
  

//# Lcd Menu Lib
//# =======================
    class LCDMenuLib2
    {
        private:
            // object pointer
            LCDMenuLib2_menu *rootMenu;
            LCDMenuLib2_menu *curMenu;
            LCDMenuLib2_menu *activMenu;
            
            // callback functions
            LCDML_FuncPtr       callback_menuControl;
            LCDML_FuncPtr       callback_contentUpdate;     // Update Content            
            LCDML_FuncPtr       callback_contentClear;      // Clear Content 
            LCDML_FuncPtr_pu8   cb_screensaver;
        
            // private variables            
            uint8_t reg1;                                    // */                                      
            uint8_t cols;                                    // display cols 
            uint8_t rows;                                    // display rows 
            uint8_t layer_save[_LCDML_DISP_cfg_cursor_deep]; // save cursor position for every layer        
            uint8_t curloc;                                  // current corsor position          
            uint8_t scroll;                                  // current scroll position 
            uint8_t scroll_save[_LCDML_DISP_cfg_cursor_deep];// save cursor position for every layer 
            uint8_t cursor_pos;                              // save the last cursor position when a menue element is called 
            uint8_t child_cnt;                               // how many childs exists on next layer 
            uint8_t layer;                                   // containes the current layer 
            uint8_t parents[_LCDML_DISP_cfg_cursor_deep];    // save last parent 
            uint8_t button;                                  // button variable 
            uint8_t control;                                 // control bits 
            uint8_t funcReg;                                 // control bits            

            unsigned long menu_timer;
            unsigned long menu_default_time;
            
            unsigned long screensaver_timer;
            unsigned long screensaver_default_time;
            
            // private methods                                                             
            void    MENU_goInto();                                                    // activate the menu under the cursor     
            void    MENU_goBack();                                                    // move to the parent menu
            void    MENU_setCursor();                                                 // set the cursor to the current position in the menu 
            void    MENU_doScroll();                                                  // scroll the menu             
            void    MENU_goMenu(LCDMenuLib2_menu &m);                                  // go to a menu element                          
            boolean MENU_selectElementDirect(LCDMenuLib2_menu &p_m, LCDML_FuncPtr_pu8 p_search);
            uint8_t MENU_countChilds();                                               // how many childs exists on next layer             
            uint8_t MENU_curlocCorrection();                                          // correction of the cursor position with hidden button
            void    BT_control();
            void    FUNC_call(); 
                      
        public:  
               
            // constructor
            LCDMenuLib2(LCDMenuLib2_menu &p_r ,const uint8_t p_rows, const uint8_t p_cols, LCDML_FuncPtr contentUpdate, LCDML_FuncPtr contentClear, LCDML_FuncPtr menuControl);
              
            // public methods
            void loop();
            void loop_menu();
            void loop_control();
            
             // Menu           
            LCDMenuLib2_menu * MENU_getObj();
            void    MENU_display(uint8_t update=0);   
            void    MENU_goRoot();                    
            uint8_t MENU_getLayer();                  
            uint8_t MENU_getCursorPos();              
            uint8_t MENU_getCursorPosAbs();           
            uint8_t MENU_getChilds();                 
            uint8_t MENU_getParentId();               
            uint8_t MENU_getParentId(uint8_t p_layer);
            uint8_t MENU_getScroll(void);
            void    MENU_enRollover(void);
            void    MENU_disRollover(void);
            
            // BT = Button
            boolean BT_setup();     
            void    BT_enter();     
            void    BT_up();        
            void    BT_down();      
            void    BT_left();      
            void    BT_right();     
            void    BT_quit();     
            void    BT_resetAll();  
            void    BT_resetEnter();
            void    BT_resetUp();   
            void    BT_resetDown(); 
            void    BT_resetLeft(); 
            void    BT_resetRight();
            boolean BT_checkAny();  
            boolean BT_checkEnter();
            boolean BT_checkUp();                      
            boolean BT_checkDown();                    
            boolean BT_checkLeft();                    
            boolean BT_checkRight();                   
            
            // Display              
            void    DISP_menuUpdate();                 
            void    DISP_clear();                      
            boolean DISP_checkMenuUpdate();            
            boolean DISP_checkMenuCursorUpdate();      
            uint8_t DISP_getMenuContentId(uint8_t n);  
                       
            // Menu function
            boolean FUNC_setup();                      
            boolean FUNC_loop();                       
            boolean FUNC_close();                      
            void    FUNC_goBackToMenu(uint8_t e=0);    
            uint8_t FUNC_getID();                                   
            void    FUNC_setLoopInterval(unsigned long t);
            void    FUNC_disableScreensaver();
              
            // Timmer
            boolean TIMER_ms(unsigned long &var, unsigned long t);  
            void    TIMER_msReset(unsigned long &var);              
            boolean TIMER_us(unsigned long &var, unsigned long t);  
            void    TIMER_usReset(unsigned long &var);              
            
            // Other
            boolean OTHER_jumpToFunc(LCDML_FuncPtr_pu8 p_search);      

            // Screensaver
            void    SCREEN_enable(LCDML_FuncPtr_pu8 function, unsigned long t); 
            void    SCREEN_disable();                                           
            void    SCREEN_resetTimer();                                        
            void    SCREEN_start();
    };
#endif


