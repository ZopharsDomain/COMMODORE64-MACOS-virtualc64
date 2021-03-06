/*!
 * @header      IEC.h
 * @author      Dirk W. Hoffmann, www.dirkwhoffmann.de
 * @copyright   2006 - 2018 Dirk W. Hoffmann
 */
/*              This program is free software; you can redistribute it and/or modify
 *              it under the terms of the GNU General Public License as published by
 *              the Free Software Foundation; either version 2 of the License, or
 *              (at your option) any later version.
 *
 *              This program is distributed in the hope that it will be useful,
 *              but WITHOUT ANY WARRANTY; without even the implied warranty of
 *              MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *              GNU General Public License for more details.
 *
 *              You should have received a copy of the GNU General Public License
 *              along with this program; if not, write to the Free Software
 *              Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _IEC_INC
#define _IEC_INC

#include "VirtualComponent.h"

class IEC : public VirtualComponent {

private:

	//! True, iff drive is connected to the IEC bus
	bool driveConnected;
	
	//! Current value of the IEC bus atn line	
	bool atnLine;

	//! Current value of the IEC bus clock line	
	bool clockLine;

	//! Current value of the IEC bus data line
	bool dataLine;
	 	
	//! Current value of the atn pin of the connected external device
	bool deviceAtnPin;

	//! True, iff the device atn pin is configured as output
	bool deviceAtnIsOutput;

	//! Current value of the data pin of the connected external device
	bool deviceDataPin;

	//! True, iff the device data pin is configured as output
	bool deviceDataIsOutput;
		
	//! Current value of the clock pin of the connected external device
	bool deviceClockPin;

	//! True, iff the device clock pin is configured as output
	bool deviceClockIsOutput;
	
	//! Current value of the data pin of the connected CIA chip
	bool ciaDataPin;
	
	//! True, iff the CIA data pin is configured as output
	bool ciaDataIsOutput;
	
	//! Current value of the clock pin of the connected CIA chip
	bool ciaClockPin;

	//! True, iff the CIA clock pin is configured as output
	bool ciaClockIsOutput;
	
	//! Current value of the ATN pin of the connected CIA chip
	bool ciaAtnPin;

	//! True, iff the CIA ATN pin is configured as output
	bool ciaAtnIsOutput;

	//! Used to determine if the bus is idle or if data is transferred 
	uint32_t busActivity;
	
	//! Update IEC bus lines depending on the CIA and device pins
	bool _updateIecLines();

public:

	//! Constructor
	IEC();
	
	//! Destructor
	~IEC();
			
	//! Bring the component back to its initial state
	void reset();

    //! Dump current configuration into message queue
    void ping();
	
	//! Dump internal state to console
	void dumpState();
	
	//! Write trace output to console
	void dumpTrace();
	
	//! Connect drive to the IEC bus
	void connectDrive();
	
	//! Disconnect the drive from the IEC bus
	void disconnectDrive();

	//! Returns true, iff a virtual disk drive is connected
	bool driveIsConnected() { return driveConnected; }
		
	//! Change/Update the value of all three bus lines 
	void updateIecLines();
	
    //! Updates the values of the CIA pin variables
	//  This function is to be invoked by the cia chip, only.
	void updateCiaPins(uint8_t cia_data, uint8_t cia_direction);	

    //! Updates the values of the device pin variables
	//  This function is to be invoked by the VC1541 drive, only.
	void updateDevicePins(uint8_t device_data, uint8_t device_direction);	
	    
	bool getAtnLine() { return atnLine; }
	bool getClockLine() { return clockLine; }
	bool getDataLine() { return dataLine; }
    
	//! Is invoked periodically by the run thread
	void execute();
};
	
#endif
