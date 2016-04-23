#!/bin/bash -ue           
#  reset_motors.sh
#  
#  Copyright 2016 Hector Perez <perezh@unican.es>
#  Based on the script developed by Bert Lindeman
#  
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.

for m in /sys/class/tacho-motor/motor*
  do
    MOTORPATH="$m"
    if [ -e $MOTORPATH ]; then
      address=`cat $MOTORPATH/address`
      printf "${MOTORPATH##*/} in $address\n"
      
      # report status before reset
      motorstate=`cat $MOTORPATH/state`
      printf "  Old state: $motorstate\n"

      # reset motor
      echo reset > "$MOTORPATH/command"

      # report status after reset
      motorstate=`cat $MOTORPATH/state`
      printf "  New state: $motorstate\n"
    fi
  done
