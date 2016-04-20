/*
 * speak.h
 * 
 * Copyright 2016 Hector Perez <perezh@unican.es>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

/* Title: speak
 * 
 * Facilities to speak */
#ifndef __EV3C_SPEAK
#define __EV3C_SPEAK

/*enum: speak_languages
 * 
 * The different available languages
 * 
 * Values:
 * ENGLISH
 * SPANISH */
typedef enum language_enum
{
    ENGLISH, SPANISH
} speak_languages;

/*
   Function: speak_say
   Function that sends the text to the speech engine.

   Parameters:
    text - text for the speech.
    lang - language to use.

   Returns:
      0 on success, -1 on failure.

   See Also:

*/
int speak_say (char * text, speak_languages lang);

#endif


